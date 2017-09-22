#include "Game_object.h"
#include <Animated_graphics_component.h>
#include <Static_graphics_component.h>
#include <Player_input_component.h>
#include <Movement_component.h>


namespace Game
{
	namespace Objects
	{
		Game_object::Game_object(Geometry::Vector<double> position) : position_{position}
		{
		}


		Game_object::~Game_object()
		{
			destroyed_event_.notify(*this);
		}


		/*Game_object::Game_object(const Game_object& other) : position{other.position}, components{}
		{
			for(auto& component : other.components)
			{
				components.emplace_back(std::unique_ptr<Component>{component->clone()});
			}
		}


		Game_object::Game_object(Game_object&& other) noexcept : position{other.position}, components{move(other.components)} // Make sure that other lists are not confused by new memory address
		{
		}


		Game_object& Game_object::operator=(const Game_object& other)
		{
			position = other.position;
			components.clear(); // Make sure to remove from other lists
			for(auto& component : other.components)
			{
				components.emplace_back(std::unique_ptr<Component>{component->clone()});
			}

			return *this;
		}


		Game_object& Game_object::operator=(Game_object&& other) noexcept
		{
			position = other.position;
			components = move(other.components); // Remove components from other lists

			return *this;
		}/**/

		void Game_object::update(Timer::Seconds time_passed)
		{
			for(auto& component : components_)
			{
				component->update(time_passed);
			}
		}


		void Game_object::handle_input(Timer::Seconds time_passed, const Input::Input_handler& input)
		{
			for (auto& component : components_)
			{
				component->handle_input(time_passed, input);
			}
		}


		Geometry::Vector<double> Game_object::get_position() const
		{
			return position_;
		}


		void Game_object::set_position(Geometry::Vector<double> position)
		{
			this->position_ = position;
		}


		void Game_object::send(const Events::Message& message)
		{
			for(auto& component : components_)
			{
				component->receive(message);
			}
		}


		void Game_object::add_component(std::unique_ptr<Component>&& component)
		{
			components_.emplace_back(move(component));
		}


		void Game_object::add_destroy_listener(std::function<void(Game_object&)> function)
		{
			destroyed_event_.add_listener(function);
		}


		Game_object* Game_object::from_json(const IO::json& json, const Component_loader& loader)
		{
			const auto x = json.at("x_position").get<double>();
			const auto y = json.at("y_position").get<double>();

			auto object = new Game_object{{x, y}};

			for(const auto component_json : json.at("components"))
			{
				object->add_component(std::unique_ptr<Component>{Component::from_json(component_json, object, loader)});
			}

			return object;
		}


		IO::json Game_object::to_json() const
		{
			auto components_json = nlohmann::json{};
			for(auto& component : components_)
			{
				components_json.emplace_back(component->to_json());
			}
			return {{"x_position", position_.get_x()}, {"y_position", position_.get_y()}, {"components", components_json}};
		}
	}
}
