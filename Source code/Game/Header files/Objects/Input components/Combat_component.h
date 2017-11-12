#pragma once

#include "Component.h"
#include "Game_object.h"


namespace Game
{
	namespace Objects
	{
		class Combat_component : public Component
		{
		public:
			Combat_component(Game_object& game_object, int max_health);

			// Serialization
			static Combat_component* from_json(const Io::json& json, Game_object& game_object, const Component_type_map& component_map);
			Io::json to_json() const override;
			static std::string type();
			std::string component_type() const override;

			int max_health() const;
			int current_health() const;

			void damage(int damage);
			void die();

			void add_health_changed_receiver(Communication::Receiver<int>& receiver); // TODO: Make this possible to do on const objects
			void add_died_receiver(Communication::Receiver<>& receiver);

		private:
			static const Deserializer add_to_map;

			int max_health_;
			int health_{max_health_};

			Communication::Dispatcher<int> health_changed_dispatcher_;
			Communication::Dispatcher<> died_dispatcher_;
		};


		class Boss_death_component : public Component
		{
		public:
			explicit Boss_death_component(Game_object& game_object);

			void receive(const Events::Message& message) override;

			// Serialization
			static Boss_death_component* from_json(const Io::json& json, Game_object& game_object, const Component_type_map& component_map);
			Io::json to_json() const override;
			static std::string type();
			std::string component_type() const override;
			
		private:
			template<typename T>
			void handle_event(const T& message) {}


			void handle_event(const Events::Died& message);

			static const Deserializer add_to_map;
		};


		class Player_death_component : public Component
		{
		public:
			explicit Player_death_component(Game_object& game_object);

			// Serialization
			static Player_death_component* from_json(const Io::json& json, Game_object& game_object, const Component_type_map& component_map);
			Io::json to_json() const override;
			static std::string type();
			std::string component_type() const override;

		private:
			static const Deserializer add_to_map;
		};
	}
}
