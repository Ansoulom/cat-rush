#pragma once

#include "Component.h"


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

			void damage(int damage);

		private:
			static const Deserializer add_to_map;

			int max_health_;
			int health_{max_health_};
		};
	}
}
