#include "Game_object.h"
#include "Component.h"


namespace Game
{
	namespace Objects
	{
		Game_object::Game_object(Geometry::Vector<double> position, Core::World& world) : position_{position}, world_{world}
		{
		}


		Game_object::~Game_object()
		{
			destroyed_event_.notify(*this);
		}


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


		Geometry::Vector<double>& Game_object::position()
		{
			return position_;
		}


		Geometry::Vector<double> Game_object::position() const
		{
			return position_;
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


		Core::World& Game_object::world()
		{
			return world_;
		}


		void Game_object::add_destroy_listener(std::function<void(Game_object&)> function)
		{
			destroyed_event_.add_listener(function);
		}


		Game_object* Game_object::from_json(const IO::json& json, Core::World& world)
		{
			const auto x = json.at("x_position").get<double>();
			const auto y = json.at("y_position").get<double>();

			auto object = new Game_object{{x, y}, world};

			for(const auto component_json : json.at("components"))
			{
				object->add_component(std::unique_ptr<Component>{Component::from_json(component_json, *object)});
			}

			return object;
		}


		IO::json Game_object::to_json() const
		{
			auto components_json = IO::json{};
			for(auto& component : components_)
			{
				components_json.emplace_back(component->to_json());
			}
			return {{"x_position", position_.get_x()}, {"y_position", position_.get_y()}, {"components", components_json}};
		}
	}
}
