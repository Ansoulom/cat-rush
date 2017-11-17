#pragma once


#include "Component.h"
#include "Collision_system.h"


namespace Game
{
	namespace Objects
	{
		class Noodles_ai_component : public Component
		{
			class State;
		public:
			explicit Noodles_ai_component(
				Game_object& game_object,
				Timer::Seconds min_noodle_rain_time,
				Timer::Seconds max_noodle_rain_time,
				Timer::Seconds time_between_noodles,
				Geometry::Vector<double> noodles_spawn_start,
				Geometry::Vector<double> noodles_spawn_end,
				int number_of_chopsticks,
				Timer::Seconds time_between_chopsticks,
				Geometry::Vector<double> chopsticks_spawn_start,
				Geometry::Vector<double> chopsticks_spawn_end);

			// Serialization
			static Noodles_ai_component* from_json(
				const Io::json& j,
				Game_object& game_object,
				const Component_type_map& component_map);
			Io::json to_json() const override;
			std::string component_type() const override;
			static std::string type();

			// Overriden functions
			void handle_input(Timer::Seconds time_passed, const Input::Input_handler& input) override;

			// Boss functions
			void switch_state(std::unique_ptr<State>&& state);

		private:
			// Required for serialization
			static const Deserializer add_to_map;


			Timer::Seconds min_noodle_rain_time_, max_noodle_rain_time_;
			Timer::Seconds time_between_noodle_projectiles_;
			Geometry::Vector<double> noodles_spawn_start_, noodles_spawn_end_;
			int num_chopstick_projectiles_;
			Timer::Seconds time_between_chopstick_projectiles_;
			Geometry::Vector<double> chopsticks_spawn_start_, chopsticks_spawn_end_;

			std::unique_ptr<State> state_;


			// Internal states

			class State
			{
			public:
				explicit State(Noodles_ai_component& boss);
				virtual ~State();

				virtual void receive(const Events::Message& message);
				virtual void exit() = 0;
				virtual void enter() = 0;
				virtual bool handle_input(Timer::Seconds time_passed) = 0;

			protected:
				Noodles_ai_component* boss_;
			};


			class Noodle_proj_state : public State
			{
			public:
				Noodle_proj_state(
					Noodles_ai_component& boss,
					Timer::Seconds time_in_state,
					Timer::Seconds time_between_projectiles,
					Geometry::Vector<double> spawn_start,
					Geometry::Vector<double> spawn_end);

				void enter() override;
				void exit() override;
				bool handle_input(Timer::Seconds time_passed) override;

				void choose_new_state();

			private:
				void create_projectile();

				Geometry::Vector<double> randomize_position();

				Timer state_timer_;
				Timer projectile_timer_;
				Geometry::Vector<double> spawn_start_;
				Geometry::Vector<double> spawn_end_;
			};


			class Chopstick_rain_state : public State
			{
			public:
				Chopstick_rain_state(
					Noodles_ai_component& boss,
					Timer::Seconds time_between_projectiles,
					int number_of_projectiles,
					Geometry::Vector<double> spawn_start,
					Geometry::Vector<double> spawn_end);

				void enter() override;
				void exit() override;
				bool handle_input(Timer::Seconds time_passed) override;

				void choose_new_state();

			private:
				void create_projectile();

				Timer projectile_timer_;
				const int num_projectiles_;
				Geometry::Vector<double> spawn_start_;
				Geometry::Vector<double> spawn_end_;
				int projectiles_shot_{};
			};
		};
	}
}
