#pragma once

#include <memory>
#include "Timer.h"
#include <json.hpp>
#include <Vector.h>
#include <Component.h>
#include <Communication.h>
#include <unordered_set>
#include <optional>


namespace Game
{
	namespace Core
	{
		class World;
	}
}


namespace Game
{
	namespace Objects
	{
		class Game_object
		{
		public:
			explicit Game_object(Geometry::Vector<double> position, Core::World& world, std::optional<std::string> name = {});
			~Game_object();

			Game_object(const Game_object& other) = delete;
			Game_object(Game_object&& other) noexcept = delete;
			Game_object& operator=(const Game_object& other) = delete;
			Game_object& operator=(Game_object&& other) noexcept = delete; // May not have to delete them anyway

			std::optional<std::string> name() const;
			Geometry::Vector<double>& position();
			Geometry::Vector<double> position() const;
			void send(const Events::Message& message);
			void add_component(std::unique_ptr<Component>&& component);
			Core::World& world();
			void add_destroy_listener(Communication::Receiver<Game_object&> function);
			// Will return null if component does not exist
			template<typename T>
			T* find_component();
			template<typename T>
			const T* find_component() const;

			// Only call from enclosing world
			void handle_input(Timer::Seconds time_passed, const Input::Input_handler& input);
			void update(Timer::Seconds time_passed);

			// Serialization and deserialization
			static Game_object* from_json(const Io::json& json, Core::World& world);
			Io::json to_json() const;

		private:
			static void create_component(
				Component_type_map& created_components,
				std::vector<Io::json>& component_pool,
				const std::string& component_type,
				std::unordered_set<std::string>& dependent_types,
				Game_object& game_object);
			static Component_type_map create_component_dependencies(
				Component_type_map& created_components,
				std::vector<Io::json>& component_pool,
				const Io::json& component_json,
				std::unordered_set<std::string>& dependent_types,
				Game_object& game_object);
			static Io::json pop_component_json(std::vector<Io::json>& component_pool, const std::string& type);

			Geometry::Vector<double> position_;
			std::optional<std::string> name_;
			std::vector<std::unique_ptr<Component>> components_{};
			Component_type_map component_map_{};
			Core::World& world_;

			Communication::Dispatcher<Game_object&> destroyed_dispatcher_; // TODO: Make this more legit
		};


		template<typename T>
		T* Game_object::find_component()
		{
			return component_map_.find_component<T>();
		}


		template<typename T>
		const T* Game_object::find_component() const
		{
			return component_map_.find_component<T>();
		}
	}
}
