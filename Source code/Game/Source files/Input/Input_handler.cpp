#include "Input_handler.h"
#include "Logger.h"
#include <string>
#include <algorithm>
#include <filesystem>
#include "tinyxml2.h"


namespace Game
{
	namespace Input
	{
		Input_handler::Input_handler()
			: keyboard_contexts_{},
			  controller_contexts_{},
			  active_contexts_{},
			  controllers_{},
			  active_controller_{},
			  controller_mode_{},
			  time_passed_{}
		{
			for(auto i = 0; i < SDL_NumJoysticks(); ++i)
			{
				if(SDL_IsGameController(i))
				{
					add_controller(i);
				}
			}
			initialize_contexts();
		}


		Input_handler::~Input_handler() { }


		void Input_handler::set_active_contexts(const std::initializer_list<std::string> context_names)
		{
			active_contexts_ = context_names;
		}


		void Input_handler::handle_event(const SDL_Event& event)
		{
			switch(event.type)
			{
				case SDL_CONTROLLERDEVICEADDED:
					add_controller(event.cdevice.which);
					break;
				case SDL_CONTROLLERDEVICEREMOVED:
					remove_controller(event.cdevice.which);
					break;
				case SDL_CONTROLLERBUTTONDOWN:
					activate_controller(SDL_GameControllerFromInstanceID(event.cbutton.which));
					forward_event(event);
					break;
				case SDL_CONTROLLERBUTTONUP:
				{
					if(controller_mode_ && SDL_GameControllerFromInstanceID(event.cbutton.which) == active_controller_)
					{
						forward_event(event);
					}
					break;
				}
				case SDL_CONTROLLERAXISMOTION:
				{
					auto controller = SDL_GameControllerFromInstanceID(event.caxis.which);
					if(abs(event.caxis.value) >= hard_controller_deadzone)
					{
						activate_controller(controller);
					}
					if(controller_mode_ && controller == active_controller_)
					{
						forward_event(event);
					}
					break;
				}
				case SDL_KEYDOWN: // Same effect, pass through
					if(event.key.repeat > 0) break;
				case SDL_MOUSEWHEEL:
				case SDL_MOUSEBUTTONDOWN:
					activate_keyboard();
					forward_event(event);
					break;
				case SDL_KEYUP: // Same effect, pass through
				case SDL_MOUSEBUTTONUP:
					if(!controller_mode_)
					{
						forward_event(event);
					}
					break;
				case SDL_MOUSEMOTION:
					if(abs(event.motion.xrel) >= hard_mouse_deadzone * time_passed_.count() || abs(event.motion.yrel) >=
						hard_mouse_deadzone * time_passed_.count())
					{
						activate_keyboard();
					}
					if(!controller_mode_)
					{
						forward_event(event);
					}
					break;
				default:
					break;
			}
		}


		void Input_handler::update(Timer::Seconds time_passed)
		{
			for(auto context_name : active_contexts_)
			{
				(controller_mode_ ? controller_contexts_ : keyboard_contexts_).at(context_name).clear_actions();
			}
			this->time_passed_ = time_passed;
		}


		bool Input_handler::get_action(Action action, std::string context) const
		{
			return (controller_mode_ ? controller_contexts_ : keyboard_contexts_).at(context).get_action(action);
		}


		bool Input_handler::get_state(State state, std::string context) const
		{
			return (controller_mode_ ? controller_contexts_ : keyboard_contexts_).at(context).get_state(state);
		}


		double Input_handler::get_range(Range range, std::string context) const
		{
			return (controller_mode_ ? controller_contexts_ : keyboard_contexts_).at(context).get_range(range);
		}


		Geometry::Vector<double> Input_handler::get_processed_vector(Range x, Range y, std::string context) const
		{
			auto v = Geometry::Vector<double>{get_range(x, context), get_range(y, context)};
			if(v.get_magnitude() >= vector_deadzone)
			{
				v.normalize();
				v.set_magnitude(v.get_magnitude() * ((v.get_magnitude() - vector_deadzone) / (1.0 - vector_deadzone)));
			}
			else
			{
				v = {0, 0};
			}
			return v;
		}


		void Input_handler::initialize_contexts()
		{
			auto map_file = canonical(std::filesystem::path{"Preferences/Keybindings.xml"});

			// TODO: Make all of this more safe
			tinyxml2::XMLDocument map_doc{};
			if(map_doc.LoadFile(map_file.string().c_str()) == tinyxml2::XML_SUCCESS)
			{
				auto contexts_list = map_doc.FirstChildElement("contexts");
				if(contexts_list)
				{
					auto current_context = contexts_list->FirstChildElement();
					while(current_context)
					{
						auto context_name = current_context->Value();
						if(context_name)
						{
							auto mapping = current_context->FirstChildElement("mapping");
							while(mapping)
							{
								auto key = mapping->FirstChildElement("key");
								auto value = mapping->FirstChildElement("value");
								if(key && value)
								{
									auto key_type = key->Attribute("type");
									auto value_type = value->Attribute("type");
									if(key_type && value_type)
									{
										auto key_name = key->GetText();
										auto value_name = value->GetText();
										if(key_name && value_name)
										{
											if(key_type == std::string{"key"})
											{
												auto button = Button{SDL_GetKeyFromName(key_name)};
												if(value_type == std::string{"action"})
													keyboard_contexts_[context_name].add_mapping(
														button,
														get_action_from_name(value_name));
												else if(value_type == std::string{"state"})
													keyboard_contexts_[context_name].add_mapping(
														button,
														get_state_from_name(value_name));
											}
											else if(key_type == std::string{"mouse_button"})
											{
												auto button = Button{get_mouse_button_from_name(key_name)};
												if(value_type == std::string{"action"})
													keyboard_contexts_[context_name].add_mapping(
														button,
														get_action_from_name(value_name));
												else if(value_type == std::string{"state"})
													keyboard_contexts_[context_name].add_mapping(
														button,
														get_state_from_name(value_name));
											}
											else if(key_type == std::string{"controller_button"})
											{
												auto button = Button{SDL_GameControllerGetButtonFromString(key_name)};
												if(value_type == std::string{"action"})
													controller_contexts_[context_name].add_mapping(
														button,
														get_action_from_name(value_name));
												else if(value_type == std::string{"state"})
													controller_contexts_[context_name].add_mapping(
														button,
														get_state_from_name(value_name));
											}
											else if(key_type == std::string{"mouse_axis"})
											{
												auto axis = get_mouse_axis_from_name(key_name);
												if(value_type == std::string{"range"})
													keyboard_contexts_[context_name].add_mapping(
														axis,
														get_range_from_name(value_name));
											}
											else if(key_type == std::string{"mouse_wheel"})
											{
												auto axis = get_mouse_wheel_axis_from_name(key_name);
												if(value_type == std::string{"range"})
													keyboard_contexts_[context_name].add_mapping(
														axis,
														get_range_from_name(value_name));
											}
											else if(key_type == std::string{"controller_axis"})
											{
												auto axis = get_controller_axis_from_name(key_name);
												if(value_type == std::string{"range"})
													controller_contexts_[context_name].add_mapping(
														axis,
														get_range_from_name(value_name));
											}
										}
									}
								}
								mapping = mapping->NextSiblingElement("mapping");
							}
						}
						current_context = current_context->NextSiblingElement();
					}
				}
			}
		}


		void Input_handler::add_controller(int index)
		{
			auto controller = std::unique_ptr<SDL_GameController, Sdl_deleter>{SDL_GameControllerOpen(index)};
			if(controller)
			{
				controllers_.emplace_back(move(controller));
			}
			else
			{
				Logger::log(
					std::string{"Could not open gamecontroller "} + std::to_string(index) + std::string{": "} + SDL_GetError());
			}
		}


		void Input_handler::remove_controller(SDL_JoystickID id)
		{
			auto controller = SDL_GameControllerFromInstanceID(id);
			if(controller)
			{
				if(active_controller_ == controller)
				{
					active_controller_ = nullptr;
					activate_keyboard();
				}
				controllers_.erase(
					remove_if(
						controllers_.begin(),
						controllers_.end(),
						[controller](std::unique_ptr<SDL_GameController, Sdl_deleter>& c) { return c.get() == controller; }));
			}
		}


		void Input_handler::activate_controller(SDL_GameController* controller)
		{
			if(!controller_mode_ || active_controller_ != controller)
			{
				reset_controller_contexts();
				active_controller_ = controller;
				controller_mode_ = true;
			}
		}


		void Input_handler::activate_keyboard()
		{
			if(controller_mode_)
			{
				reset_keyboard_contexts();
				controller_mode_ = false;
			}
		}


		void Input_handler::forward_event(const SDL_Event& event)
		{
			for(auto context : active_contexts_)
			{

				(controller_mode_ ? controller_contexts_ : keyboard_contexts_).at(context).handle_event(event);
			}
		}


		void Input_handler::reset_controller_contexts()
		{
			for(auto context_name : active_contexts_)
			{
				controller_contexts_.at(context_name).reset();
			}
		}


		void Input_handler::reset_keyboard_contexts()
		{
			for(auto context_name : active_contexts_)
			{
				keyboard_contexts_.at(context_name).reset();
			}
		}


		Uint8 Input_handler::get_mouse_button_from_name(std::string name)
		{
			if(name == "Left")
			{
				return SDL_BUTTON_LEFT;
			}
			if(name == "Middle")
			{
				return SDL_BUTTON_MIDDLE;
			}
			if(name == "Right")
			{
				return SDL_BUTTON_RIGHT;
			}
			if(name == "4")
			{
				return SDL_BUTTON_X1;
			}
			if(name == "5")
			{
				return SDL_BUTTON_X2;
			}
			return 0;
		}


		Axis Input_handler::get_controller_axis_from_name(std::string name)
		{
			if(name == "Left_x")
			{
				return Axis::controller_stick_left_x;
			}
			if(name == "Left_y")
			{
				return Axis::controller_stick_left_y;
			}
			if(name == "Right_x")
			{
				return Axis::controller_stick_right_x;
			}
			if(name == "Right_y")
			{
				return Axis::controller_stick_right_y;
			}
			if(name == "Trigger_left")
			{
				return Axis::controller_trigger_left;
			}
			if(name == "Trigger_right")
			{
				return Axis::controller_trigger_right;
			}
			return Axis{};
		}


		Action Input_handler::get_action_from_name(std::string name)
		{
			if(name == "attack") return Action::attack;
			if(name == "jump") return Action::jump;
			if(name == "start") return Action::start;
			if(name == "exit") return Action::exit;
			return Action{};
		}


		State Input_handler::get_state_from_name(std::string name)
		{
			if(name == "movement_up") return State::movement_up;
			if(name == "movement_left") return State::movement_left;
			if(name == "movement_down") return State::movement_down;
			if(name == "movement_right") return State::movement_right;
			return State{};
		}


		Range Input_handler::get_range_from_name(std::string name)
		{
			if(name == "movement_x") return Range::movement_x;
			if(name == "movement_y") return Range::movement_y;
			return Range{};
		}


		Axis Input_handler::get_mouse_axis_from_name(std::string name)
		{
			if(name == "X") return Axis::mouse_x;
			if(name == "Y") return Axis::mouse_y;
			return Axis{};
		}


		Axis Input_handler::get_mouse_wheel_axis_from_name(std::string name)
		{
			if(name == "X") return Axis::mouse_scroll_x;
			if(name == "Y") return Axis::mouse_scroll_y;
			return Axis{};
		}
	}
}
