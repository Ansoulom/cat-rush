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
			explicit Combat_component(Game_object& game_object, int max_health);

			// Serialization
			static Combat_component* from_json(const Io::json& json, Game_object& game_object, const Component_type_map& component_map);
			Io::json to_json() const override;
			static std::string type();
			std::string component_type() const override;

			int max_health() const;
			int current_health() const;

			void damage(int damage);

			void add_health_changed_listener(Communication::Receiver<int>& listener); // TODO: Make this possible to do on const objects

		private:
			static const Deserializer add_to_map;

			int max_health_;
			int health_{max_health_};

			Communication::Dispatcher<int> health_changed_event_;
		};
	}
}
