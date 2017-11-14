#include "Noodles_ai_component.h"
#include "Game_object.h"
#include "Randomness.h"
#include "World.h"
#include "Collider_component.h"
#include "Static_graphics_component.h"
#include "Combat_component.h"
#include "Movement_component.h"
#include "Acceleration_component.h"


namespace Game
{
	namespace Objects
	{
		Noodles_ai_component::Noodles_ai_component(Game_object& game_object) : Component{game_object},
																			   state_{new Noodle_proj_state{*this, Timer::Seconds{3.0}, Timer::Seconds{0.2}}}
		{
			state_->enter();
		}


		// TODO: Get rid of magic numbers


		Noodles_ai_component* Noodles_ai_component::from_json(const Io::json& j, Game_object& game_object,
															  const Component_type_map& component_map)
		{
			return new Noodles_ai_component{game_object};
		}


		Io::json Noodles_ai_component::to_json() const
		{
			return {{"type", type()}};
		}


		std::string Noodles_ai_component::component_type() const
		{
			return type();
		}


		std::string Noodles_ai_component::type()
		{
			return "Noodles_ai_component";
		}


		void Noodles_ai_component::handle_input(Timer::Seconds time_passed, const Input::Input_handler& input)
		{
			while(state_->handle_input(time_passed)) {}
		}


		void Noodles_ai_component::switch_state(std::unique_ptr<State>&& state)
		{
			state_->exit();
			state_ = move(state);
			state_->enter();
		}


		const Component::Deserializer Noodles_ai_component::add_to_map{type(), from_json};


		// State

		Noodles_ai_component::State::State(Noodles_ai_component& boss) : boss_{&boss} {}


		Noodles_ai_component::State::~State() {}


		void Noodles_ai_component::State::receive(const Events::Message& message) {}


		// Noodle_proj_state

		Noodles_ai_component::Noodle_proj_state::
			Noodle_proj_state(Noodles_ai_component& boss, Timer::Seconds time_in_state,
							  Timer::Seconds time_between_projectiles) : State{boss},
																		 state_timer_{time_in_state}, projectile_timer_{time_between_projectiles} {}


		void Noodles_ai_component::Noodle_proj_state::enter() {}


		void Noodles_ai_component::Noodle_proj_state::exit() {}


		bool Noodles_ai_component::Noodle_proj_state::handle_input(Timer::Seconds time_passed)
		{
			if(state_timer_.update(time_passed))
			{
				choose_new_state();
				return false;
			}
			if(projectile_timer_.update(time_passed))
			{
				create_projectile();
				projectile_timer_.reset_to_surplus();
			}

			return false;
		}


		void Noodles_ai_component::Noodle_proj_state::choose_new_state()
		{
			boss_->switch_state(std::make_unique<Chopstick_rain_state>(*boss_, Timer::Seconds{3.0}));
			// TODO: Get rid of magic number
		}


		void Noodles_ai_component::Noodle_proj_state::create_projectile()
		{
			// TODO: Put this in its own function
			auto file = boost::filesystem::path{std::string{"Prefabs/Noodle_projectile.json"}};
			std::ifstream in{file.string()};
			if(!in.is_open())
			{
				throw std::runtime_error{std::string{"Could not open level file"} + file.string()};
			}

			auto object_json = Io::json{};
			in >> object_json;
			auto projectile = std::unique_ptr<Game_object>{
				Game_object::from_json(object_json, boss_->game_object().world())
			};
			projectile->position() = randomize_position(); // TODO: Make sure that a position changed event fires
			boss_->game_object().world().add_object(move(projectile));
		}


		Geometry::Vector<double> Noodles_ai_component::Noodle_proj_state::randomize_position()
		{
			static auto random_gen = Random::Random_floating<double>{-0.8, 0.8};
			return {random_gen(), -0.45};
		}


		// Chopstick_rain_state

		Noodles_ai_component::Chopstick_rain_state::Chopstick_rain_state(Noodles_ai_component& boss,
																		 Timer::Seconds time_in_state) : State{boss}, state_timer_{time_in_state} {}


		void Noodles_ai_component::Chopstick_rain_state::enter() {}


		void Noodles_ai_component::Chopstick_rain_state::exit() {}


		bool Noodles_ai_component::Chopstick_rain_state::handle_input(Timer::Seconds time_passed)
		{
			if(state_timer_.update(time_passed))
			{
				choose_new_state();
			}
			return false;
		}


		void Noodles_ai_component::Chopstick_rain_state::choose_new_state()
		{
			boss_->switch_state(std::make_unique<Noodle_proj_state>(*boss_, Timer::Seconds{3.0}, Timer::Seconds{0.2}));
		}
	}
}
