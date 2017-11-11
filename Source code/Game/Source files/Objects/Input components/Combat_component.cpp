#include "Combat_component.h"


namespace Game
{
	namespace Objects
	{
		Combat_component::Combat_component(Game_object& game_object, int max_health)
			: Component{game_object}, max_health_{max_health} {}


		Combat_component* Combat_component::from_json(
			const Io::json& json,
			Game_object& game_object,
			const Component_type_map& component_map)
		{
			return new Combat_component{game_object, json.at("max_health").get<int>()};
		}


		Io::json Combat_component::to_json() const
		{
			return {{"type", type()}, {"max_health", max_health_}};
		}


		std::string Combat_component::type()
		{
			return "Combat_component";
		}


		std::string Combat_component::component_type() const
		{
			return type();
		}


		int Combat_component::max_health() const
		{
			return max_health_;
		}


		int Combat_component::current_health() const
		{
			return health_;
		}


		const Component::Deserializer Combat_component::add_to_map{type(), from_json};


		void Combat_component::damage(int damage)
		{
			health_ -= damage;
			if (health_ <= 0)
			{
				health_ = 0;
				// TODO: Make it die
			}
			health_changed_event_.dispatch(health_);
		}


		void Combat_component::add_health_changed_listener(Communication::Receiver<int>& listener)
		{
			health_changed_event_.add_receiver(listener);
		}
	}
}
