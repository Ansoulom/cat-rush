#include "Combat_component.h"


namespace Game
{
	namespace Objects
	{
		// Combat component

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
			if(health_ <= 0)
			{
				health_ = 0;
				health_changed_dispatcher_.dispatch(health_);
				die();
				return;
			}
			health_changed_dispatcher_.dispatch(health_);
		}


		void Combat_component::die()
		{
			game_object().send(Events::Died{});
			died_dispatcher_.dispatch();
		}


		void Combat_component::add_health_changed_receiver(Communication::Receiver<int>& receiver)
		{
			health_changed_dispatcher_.add_receiver(receiver);
		}


		void Combat_component::add_died_receiver(Communication::Receiver<>& receiver)
		{
			died_dispatcher_.add_receiver(receiver);
		}


		// Boss death component

		Boss_death_component::Boss_death_component(Game_object& game_object) : Component{game_object} {}


		void Boss_death_component::receive(const Events::Message& message)
		{
			visit([this](const auto& msg) { handle_event(msg); }, message);
		}


		Boss_death_component* Boss_death_component::from_json(const Io::json& json, Game_object& game_object,
															  const Component_type_map& component_map)
		{
			return new Boss_death_component{game_object};
		}


		Io::json Boss_death_component::to_json() const
		{
			return {{"type", type()}};
		}


		std::string Boss_death_component::type()
		{
			return "Boss_death_component";
		}


		std::string Boss_death_component::component_type() const
		{
			return type();
		}


		void Boss_death_component::handle_event(const Events::Died& message)
		{
			// Should this even be here?
		}


		// Player death component

		Player_death_component::Player_death_component(Game_object& game_object) : Component{game_object} {}


		Player_death_component* Player_death_component::from_json(const Io::json& json, Game_object& game_object,
																  const Component_type_map& component_map)
		{
			return new Player_death_component{game_object};
		}


		Io::json Player_death_component::to_json() const
		{
			return {{"type", type()}};
		}


		std::string Player_death_component::type()
		{
			return "Player_death_component";
		}


		std::string Player_death_component::component_type() const
		{
			return type();
		}
	}
}
