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
			explicit Noodles_ai_component(Game_object& game_object);

			// Serialization
			static Noodles_ai_component* from_json(const Io::json& j, Game_object& game_object,
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
				Noodle_proj_state(Noodles_ai_component& boss, Timer::Seconds time_in_state, Timer::Seconds time_between_projectiles);

				void enter() override;
				void exit() override;
				bool handle_input(Timer::Seconds time_passed) override;

				void choose_new_state();

			private:
				void create_projectile();

				static Geometry::Vector<double> randomize_position();

				Timer state_timer_;
				Timer projectile_timer_;
			};


			class Chopstick_rain_state : public State
			{
			public:
				Chopstick_rain_state(Noodles_ai_component& boss, Timer::Seconds time_between_projectiles, int number_of_projectiles, Geometry::Vector<double> start_position, double projectile_spacing);

				void enter() override;
				void exit() override;
				bool handle_input(Timer::Seconds time_passed) override;

				void choose_new_state();

			private:
				void create_projectile();

				Timer projectile_timer_;
				const int num_projectiles_;
				int projectiles_shot_{};
				Geometry::Vector<double> start_position_;
				double projectile_spacing_;
			};
		};
	}
}
