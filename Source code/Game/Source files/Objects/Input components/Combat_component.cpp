#include "Combat_component.h"
#include "Collider_component.h"
#include "World.h"


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


		Projectile_component::
			Projectile_component(Game_object& game_object, int damage, std::string hit_layer) : Component{game_object},
																								damage_{damage}, hit_layer_{hit_layer} {}


		Projectile_component* Projectile_component::from_json(const Io::json& json, Game_object& game_object,
															  const Component_type_map& component_map)
		{
			return new Projectile_component{game_object, json.at("damage").get<int>(), json.at("hit_layer").get<std::string>()};
		}


		Io::json Projectile_component::to_json() const
		{
			return {{"type", type()}, {"damage", damage_}, {"hit_layer", hit_layer_}};
		}


		std::string Projectile_component::type()
		{
			return "Projectile_component";
		}


		std::string Projectile_component::component_type() const
		{
			return type();
		}


		void Projectile_component::receive(const Events::Message& message)
		{
			visit([this](const auto& msg) { handle_event(msg); }, message);
		}


		void Projectile_component::handle_event(const Events::Collided& message)
		{
			if(message.layer == hit_layer_)
			{
				if(auto combat_comp = message.collided_with.game_object().find_component<Combat_component>())
				{
					combat_comp->damage(damage_);
				}
			}
			game_object().world().destroy_object(game_object());
		}


		const Component::Deserializer Projectile_component::add_to_map{type(), from_json};
	}
}
