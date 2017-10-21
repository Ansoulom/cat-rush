#pragma once

#include <json.hpp>
#include "Component.h"
#include <chrono>


namespace Game
{
	namespace Objects
	{
		class Movement_component;
		class Player_input_component;


		class Player_input_component : public Component
		{
			class Player_state;
		public:
			Player_input_component(Game_object& game_object, Movement_component& movement_component, double max_speed, Collider_component& collider_component);

			static Player_input_component* from_json(
				const Io::json& json,
				Game_object& game_object,
				const Component_type_map& component_map);
			Io::json to_json() const override;
			std::string component_type() const override;
			static std::string type();

			void receive(const Events::Message& message) override;
			void handle_input(Timer::Seconds time_passed, const Input::Input_handler& input) override;

			void jump();
			void switch_state(std::unique_ptr<Player_state>&& state);
			static double get_x_input(const Input::Input_handler& input);

		private:
			static const Deserializer add_to_map;

			void update_rotation(Direction_x direction);

			Direction_x direction_;
			double max_speed_;
			std::unique_ptr<Player_state> state_;
			std::string old_state_;
			Movement_component* movement_component_;
			Collider_component* collider_component_;


			// Internal states

			class Player_state
			{
			public:
				explicit Player_state(Player_input_component& player);
				virtual ~Player_state();

				virtual std::string state() const = 0;
				virtual void receive(const Events::Message& message);
				virtual void exit() = 0;
				virtual void enter() = 0;
				virtual bool handle_input(Timer::Seconds time_passed, const Input::Input_handler& input) = 0;

			protected:
				Player_input_component* player_;
			};


			class Air_state : public Player_state
			{
			public:
				explicit Air_state(Player_input_component& player);

				void receive(const Events::Message& message) override;

				std::string state() const override;
				void enter() override;
				void exit() override;
				bool handle_input(Timer::Seconds time_passed, const Input::Input_handler& input) override;

			private:
				template<typename T>
				void handle_event(const T& message) {}


				void handle_event(const Events::Collided& message);
			};


			class Ground_state : public Player_state
			{
			public:
				explicit Ground_state(Player_input_component& player);

				bool handle_input(Timer::Seconds time_passed, const Input::Input_handler& input) override;
			};


			class Walk_state : public Ground_state
			{
			public:
				explicit Walk_state(Player_input_component& player);

				void enter() override;
				void exit() override;
				std::string state() const override;
				bool handle_input(Timer::Seconds time_passed, const Input::Input_handler& input) override;
			};


			class Idle_state : public Ground_state
			{
			public:
				explicit Idle_state(Player_input_component& player);

				void enter() override;
				void exit() override;
				std::string state() const override;
				bool handle_input(Timer::Seconds time_passed, const Input::Input_handler& input) override;
			};


			class Crouch_state : public Ground_state
			{
			public:
				explicit Crouch_state(Player_input_component& player);

				void enter() override;
				void exit() override;
				std::string state() const override;
				bool handle_input(Timer::Seconds time_passed, const Input::Input_handler& input) override;
			};
		};
	}
}
