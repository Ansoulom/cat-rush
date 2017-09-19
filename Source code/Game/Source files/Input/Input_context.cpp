#include "Input_context.h"
#include "Game_core.h"


namespace Game
{
	namespace Input
	{
		Context::Context() : action_mappings{}, state_mappings{}, range_mappings{}, actions{}, states{}, ranges{}
		{
		}


		void Context::add_mapping(Button button, Action action)
		{
			action_mappings[button] = action;
		}


		void Context::add_mapping(Button button, State state)
		{
			state_mappings[button] = state;
		}


		void Context::add_mapping(Axis axis, Range range)
		{
			range_mappings[axis] = range;
		}


		// TODO: Check if key exists in map
		bool Context::get_action(Action action) const
		{
			if(actions.find(action) == actions.end()) return false;
			return actions.at(action);
		}


		bool Context::get_state(State state) const
		{
			if(states.find(state) == states.end()) return false;
			return states.at(state);
		}


		double Context::get_range(Range range) const
		{
			if(ranges.find(range) == ranges.end()) return 0.0;
			return ranges.at(range);
		}


		void Context::handle_event(const SDL_Event& event)
		{
			switch(event.type)
			{
				case SDL_KEYDOWN:
				{
					auto button = Button{event.key.keysym.sym};
					auto action_iter = action_mappings.find(button);
					if(action_iter != action_mappings.end())
					{
						actions[action_iter->second] = true;
					}
					auto state_iter = state_mappings.find(button);
					if(state_iter != state_mappings.end())
					{
						states[state_iter->second] = true;
					}
					break;
				}
				case SDL_KEYUP:
				{
					auto button = Button{event.key.keysym.sym};
					auto state_iter = state_mappings.find(button);
					if(state_iter != state_mappings.end())
					{
						states[state_iter->second] = false;
					}
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					auto button = Button{event.button.button};
					auto action_iter = action_mappings.find(button);
					if(action_iter != action_mappings.end())
					{
						actions[action_iter->second] = true;
					}
					auto state_iter = state_mappings.find(button);
					if(state_iter != state_mappings.end())
					{
						states[state_iter->second] = true;
					}
					break;
				}
				case SDL_MOUSEBUTTONUP:
				{
					auto button = Button{event.button.button};
					auto state_iter = state_mappings.find(button);
					if(state_iter != state_mappings.end())
					{
						states[state_iter->second] = false;
					}
					break;
				}
				case SDL_CONTROLLERBUTTONDOWN:
				{
					auto button = Button{static_cast<SDL_GameControllerButton>(event.cbutton.button)};
					auto action_iter = action_mappings.find(button);
					if(action_iter != action_mappings.end())
					{
						actions[action_iter->second] = true;
					}
					auto state_iter = state_mappings.find(button);
					if(state_iter != state_mappings.end())
					{
						states[state_iter->second] = true;
					}
					break;
				}
				case SDL_CONTROLLERBUTTONUP:
				{
					auto button = Button{static_cast<SDL_GameControllerButton>(event.cbutton.button)};
					auto state_iter = state_mappings.find(button);
					if(state_iter != state_mappings.end())
					{
						states[state_iter->second] = false;
					}
					break;
				}
				case SDL_MOUSEMOTION:
				{
					auto x_axis = Axis::mouse_x;
					auto x_iter = range_mappings.find(x_axis);
					if(x_iter != range_mappings.end())
					{
						auto normalized = (event.motion.x - Game_core::screen_width / 2) / (Game_core::screen_width / 2.0);
						ranges[x_iter->second] = normalized;
					}
					auto y_axis = Axis::mouse_y;
					auto y_iter = range_mappings.find(y_axis);
					if(y_iter != range_mappings.end())
					{
						auto normalized = (event.motion.y - Game_core::screen_height / 2) / (Game_core::screen_height / 2.0);
						ranges[x_iter->second] = normalized;
					}
					break;
				}
				case SDL_MOUSEWHEEL:
				{
					auto x_axis = Axis::mouse_scroll_x;
					auto x_iter = range_mappings.find(x_axis);
					if(x_iter != range_mappings.end())
					{
						auto normalized = event.wheel.x > 0
											? static_cast<double>(event.wheel.x) / std::numeric_limits<Sint32>::max()
											: static_cast<double>(-event.wheel.x) / std::numeric_limits<Sint32>::min();
						ranges[x_iter->second] = normalized;
					}
					auto y_axis = Axis::mouse_scroll_y;
					auto y_iter = range_mappings.find(y_axis);
					if(y_iter != range_mappings.end())
					{
						auto normalized = event.wheel.y > 0
											? static_cast<double>(event.wheel.y) / std::numeric_limits<Sint32>::max()
											: static_cast<double>(-event.wheel.y) / std::numeric_limits<Sint32>::min();
						ranges[y_iter->second] = normalized;
					}
					break;
				}
				case SDL_CONTROLLERAXISMOTION:
				{
					Axis axis;
					switch(event.caxis.axis)
					{
						case SDL_CONTROLLER_AXIS_LEFTX:
							axis = Axis::controller_stick_left_x;
							break;
						case SDL_CONTROLLER_AXIS_LEFTY:
							axis = Axis::controller_stick_left_y;
							break;
						case SDL_CONTROLLER_AXIS_RIGHTX:
							axis = Axis::controller_stick_right_x;
							break;
						case SDL_CONTROLLER_AXIS_RIGHTY:
							axis = Axis::controller_stick_right_y;
							break;
						case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
							axis = Axis::controller_trigger_left;
							break;
						case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
							axis = Axis::controller_trigger_right;
							break;
						default:
							return;
					}

					auto i_axis = Axis{axis};
					auto iter = range_mappings.find(i_axis);
					if(iter != range_mappings.end())
					{
						auto normalized = event.caxis.value > 0 ? event.caxis.value / 32767.0 : event.caxis.value / 32768.0;
						ranges[iter->second] = normalized;
					}
					break;
				}
				default:
					break;
			}
		}


		void Context::clear_actions()
		{
			actions.clear();
		}


		void Context::reset()
		{
			actions.clear();
			states.clear();
			ranges.clear();
		}
	}
}
