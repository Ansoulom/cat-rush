#include "Game_object.h"
#include "Component.h"
#include <queue>
#include <unordered_set>


namespace Game
{
	namespace Objects
	{
		Game_object::Game_object(Geometry::Vector<double> position, Core::World& world, std::optional<std::string> name)
			: position_{position}, name_{name}, world_{world}, destroyed_{false} { }


		Game_object::~Game_object()
		{
			destroyed_dispatcher_.dispatch(*this);
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
			for(auto& component : components_)
			{
				component->handle_input(time_passed, input);
			}
		}


		void Game_object::destroy()
		{
			/*destroyed_dispatcher_.dispatch(*this);
			destroyed_dispatcher_.clear();
			components_.clear();
			component_map_.clear();*/
			destroyed_ = true;
		}


		std::optional<std::string> Game_object::name() const
		{
			return name_;
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
			component_map_.add_component(*component);
			components_.emplace_back(move(component));
		}


		Core::World& Game_object::world()
		{
			return world_;
		}


		void Game_object::add_destroy_listener(Communication::Receiver<Game_object&> listener)
		{
			destroyed_dispatcher_.add_receiver(listener);
		}


		Io::json Game_object::pop_component_json(std::vector<Io::json>& component_pool, const std::string& type)
		{
			// Check if the required object even exists in the json file
			const auto json_iter = find_if(
				component_pool.begin(),
				component_pool.end(),
				[&type](const Io::json& j) { return j.at("type").get<std::string>() == type; });
			if(json_iter == component_pool.end())
			{
				throw std::runtime_error{
					std::string{"Could not find component "} + type + " in level file, but another component depends on it!"
				};
			}

			const auto j = std::move(*json_iter);
			component_pool.erase(json_iter);

			return j;
		}


		Component_type_map Game_object::create_component_dependencies(
			Component_type_map& created_components,
			std::vector<Io::json>& component_pool,
			const Io::json& component_json,
			std::unordered_set<std::string>& dependent_types,
			Game_object& game_object)
		{
			// Find and create components that are depended on
			auto dependency_map = Component_type_map{};
			auto req_iter = component_json.find("requires");
			if(req_iter != component_json.end())
			{
				for(auto req : req_iter.value())
				{
					const auto req_type = req.get<std::string>();
					if(!created_components.contains(req_type))
					{
						create_component(created_components, component_pool, req_type, dependent_types, game_object);
					}
					dependency_map.add_component(created_components.get_component(req_type));
				}
			}
			return dependency_map;
		}


		void Game_object::create_component(
			Component_type_map& created_components,
			std::vector<Io::json>& component_pool,
			const std::string& component_type,
			std::unordered_set<std::string>& dependent_types,
			Game_object& game_object)
		{
			// Check if there are circular dependencies in the json file
			if(dependent_types.find(component_type) != dependent_types.end())
			{
				throw std::runtime_error{"Circular dependency in object file!"};
			}
			dependent_types.insert(component_type);

			const auto j = pop_component_json(component_pool, component_type);

			const auto dependency_map = create_component_dependencies(
				created_components,
				component_pool,
				j,
				dependent_types,
				game_object);

			// Create the actual component and add to the game object
			auto component = std::unique_ptr<Component>{Component::from_json(j, game_object, dependency_map)};
			created_components.add_component(*component);
			game_object.add_component(move(component));

			dependent_types.erase(component_type);
		}


		Game_object* Game_object::from_json(const Io::json& json, Core::World& world)
		{
			const auto name_iter = json.find("name");
			const auto name = name_iter == json.end() || name_iter->is_null() ?
								  std::optional<std::string>{} :
								  std::optional<std::string>{name_iter->get<std::string>()};
			const auto x = json.at("x_position").get<double>();
			const auto y = json.at("y_position").get<double>();
			const auto object = new Game_object{{x, y}, world, name};

			// Load all components while managing dependencies between components
			auto load_queue = json.at("components").get<std::vector<Io::json>>();
			auto created_components = Component_type_map{};
			auto dependency_chain = std::unordered_set<std::string>{};
			while(load_queue.size() > 0)
			{
				create_component(
					created_components,
					load_queue,
					load_queue[load_queue.size() - 1].at("type").get<std::string>(),
					dependency_chain,
					*object);
			}

			return object;
		}


		Io::json Game_object::to_json() const
		{
			auto components_json = Io::json{};
			for(auto& component : components_)
			{
				components_json.emplace_back(component->to_json());
			}
			return {
				{"x_position", position_.get_x()}, {"y_position", position_.get_y()}, {"components", components_json},
				{"name", name_.has_value() ? Io::json{name_.value()} : Io::json{nullptr}}
			};
		}
	}
}
