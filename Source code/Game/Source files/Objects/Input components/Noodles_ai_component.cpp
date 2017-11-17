#include "Noodles_ai_component.h"
#include "Game_object.h"
#include "Randomness.h"
#include "World.h"


namespace Game
{
	namespace Objects
	{
		Noodles_ai_component::Noodles_ai_component(
			Game_object& game_object,
			const Timer::Seconds min_noodle_rain_time,
			const Timer::Seconds max_noodle_rain_time,
			const Timer::Seconds time_between_noodles,
			const Geometry::Vector<double> noodles_spawn_start,
			const Geometry::Vector<double> noodles_spawn_end,
			const int number_of_chopsticks,
			const Timer::Seconds time_between_chopsticks,
			const Geometry::Vector<double> chopsticks_spawn_start,
			const Geometry::Vector<double> chopsticks_spawn_end)
			: Component{game_object},
			  min_noodle_rain_time_{min_noodle_rain_time},
			  max_noodle_rain_time_{max_noodle_rain_time},
			  time_between_noodle_projectiles_{time_between_noodles},
			  noodles_spawn_start_{noodles_spawn_start},
			  noodles_spawn_end_{noodles_spawn_end},
			  num_chopstick_projectiles_{number_of_chopsticks},
			  time_between_chopstick_projectiles_{time_between_chopsticks},
			  chopsticks_spawn_start_{chopsticks_spawn_start},
			  chopsticks_spawn_end_{chopsticks_spawn_end},
			  state_{
				  new Noodle_proj_state{
					  *this,
					  Timer::Seconds{Random::random_floating(min_noodle_rain_time.count(), max_noodle_rain_time.count())},
					  time_between_noodles,
					  noodles_spawn_start,
					  noodles_spawn_end
				  }
			  }
		{
			state_->enter();
		}


		// TODO: Get rid of magic numbers


		Noodles_ai_component* Noodles_ai_component::from_json(
			const Io::json& j,
			Game_object& game_object,
			const Component_type_map& component_map)
		{
			return new Noodles_ai_component{
				game_object,
				Timer::Seconds{j.at("min_noodles_rain_time").get<double>()},
				Timer::Seconds{j.at("max_noodles_rain_time").get<double>()},
				Timer::Seconds{j.at("time_between_noodles").get<double>()},
				Geometry::Vector<double>{j.at("noodles_spawn_start_x").get<double>(), j.at("noodles_spawn_start_y").get<double>()},
				Geometry::Vector<double>{j.at("noodles_spawn_end_x").get<double>(), j.at("noodles_spawn_end_y").get<double>()},
				j.at("number_of_chopsticks").get<int>(),
				Timer::Seconds{j.at("time_between_chopsticks").get<double>()},
				Geometry::Vector<double>{
					j.at("chopsticks_spawn_start_x").get<double>(),
					j.at("chopsticks_spawn_start_y").get<double>()
				},
				Geometry::Vector<double>{
					j.at("chopsticks_spawn_end_x").get<double>(),
					j.at("chopsticks_spawn_end_y").get<double>()
				},
			};
		}


		Io::json Noodles_ai_component::to_json() const
		{
			return {
				{"type", type()},
				{"min_noodles_rain_time", min_noodle_rain_time_.count()},
				{"max_noodles_rain_time", max_noodle_rain_time_.count()},
				{"time_between_noodles", time_between_noodle_projectiles_.count()},
				{"noodles_spawn_start_x", noodles_spawn_start_.get_x()},
				{"noodles_spawn_start_y", noodles_spawn_start_.get_y()},
				{"noodles_spawn_end_x", noodles_spawn_end_.get_x()},
				{"noodles_spawn_end_y", noodles_spawn_end_.get_y()},
				{"number_of_chopsticks", num_chopstick_projectiles_},
				{"time_between_chopsticks", time_between_chopstick_projectiles_.count()},
				{"chopsticks_spawn_start_x", chopsticks_spawn_start_.get_x()},
				{"chopsticks_spawn_start_y", chopsticks_spawn_start_.get_y()},
				{"chopsticks_spawn_end_x", chopsticks_spawn_end_.get_x()},
				{"chopsticks_spawn_end_y", chopsticks_spawn_end_.get_y()}
			};
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

		Noodles_ai_component::State::State(Noodles_ai_component& boss)
			: boss_{&boss} {}


		Noodles_ai_component::State::~State() {}


		void Noodles_ai_component::State::receive(const Events::Message& message) {}


		// Noodle_proj_state

		Noodles_ai_component::Noodle_proj_state::
			Noodle_proj_state(
				Noodles_ai_component& boss,
				const Timer::Seconds time_in_state,
				const Timer::Seconds time_between_projectiles,
				const Geometry::Vector<double> spawn_start,
				const Geometry::Vector<double> spawn_end)
			: State{boss},
			  state_timer_{time_in_state},
			  projectile_timer_{time_between_projectiles},
			  spawn_start_{spawn_start},
			  spawn_end_{spawn_end} {}


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
				projectile_timer_.reset_to_surplus();
				create_projectile();
			}

			return false;
		}


		void Noodles_ai_component::Noodle_proj_state::choose_new_state()
		{
			boss_->switch_state(
				std::make_unique<Chopstick_rain_state>(
					*boss_,
					boss_->time_between_chopstick_projectiles_,
					boss_->num_chopstick_projectiles_,
					boss_->chopsticks_spawn_start_,
					boss_->chopsticks_spawn_end_));
		}


		void Noodles_ai_component::Noodle_proj_state::create_projectile()
		{
			boss_->game_object().world().add_object_from_prefab("Noodle_projectile", randomize_position());
		}


		Geometry::Vector<double> Noodles_ai_component::Noodle_proj_state::randomize_position()
		{
			return {
				Random::random_floating(spawn_start_.get_x(), spawn_end_.get_x()),
				Random::random_floating(spawn_start_.get_y(), spawn_end_.get_y())
			};
		}


		// Chopstick_rain_state

		Noodles_ai_component::Chopstick_rain_state::Chopstick_rain_state(
			Noodles_ai_component& boss,
			const Timer::Seconds time_between_projectiles,
			const int number_of_projectiles,
			const Geometry::Vector<double> spawn_start,
			const Geometry::Vector<double> spawn_end)
			: State{boss},
			  projectile_timer_{time_between_projectiles},
			  num_projectiles_{number_of_projectiles},
			  spawn_start_{spawn_start},
			  spawn_end_{spawn_end} {}


		void Noodles_ai_component::Chopstick_rain_state::enter() {}


		void Noodles_ai_component::Chopstick_rain_state::exit() {}


		bool Noodles_ai_component::Chopstick_rain_state::handle_input(Timer::Seconds time_passed)
		{
			if(projectile_timer_.update(time_passed))
			{
				projectile_timer_.reset_to_surplus();
				create_projectile();
				if(++projectiles_shot_ >= num_projectiles_)
				{
					choose_new_state();
				}
			}
			return false;
		}


		void Noodles_ai_component::Chopstick_rain_state::choose_new_state()
		{
			boss_->switch_state(
				std::make_unique<Noodle_proj_state>(
					*boss_,
					Timer::Seconds{Random::random_floating(boss_->min_noodle_rain_time_.count(), boss_->max_noodle_rain_time_.count())},
					Timer::Seconds{boss_->time_between_noodle_projectiles_},
					boss_->noodles_spawn_start_,
					boss_->noodles_spawn_end_));
		}


		void Noodles_ai_component::Chopstick_rain_state::create_projectile()
		{
			boss_->game_object().world().add_object_from_prefab(
				"Chopstick_projectile",
				spawn_start_ + (spawn_end_ - spawn_start_) / static_cast<double>(num_projectiles_) * static_cast<double>(
					projectiles_shot_));
		}
	}
}
