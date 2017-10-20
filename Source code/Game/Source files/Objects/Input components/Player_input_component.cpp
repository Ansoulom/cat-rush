#include "Player_input_component.h"
#include "Input_handler.h"
#include "Vector.h"
#include "Math_functions.h"
#include "Game_object.h"
#include <valarray>


namespace Game
{
	namespace Objects
	{
		Player::Player_state::Player_state(Player_input_component& player) : player_{&player} { }


		Player::Player_state::~Player_state() {}


		Player::Air_state::Air_state(Player_input_component& player) : Player_state{player} { }


		void Player::Air_state::receive(const Events::Message& message)
		{
			visit([this](const auto& msg) { handle_event(msg); }, message);
		}


		std::string Player::Air_state::state() const
		{
			return "Jumping";
		}


		void Player::Player_state::receive(const Events::Message& message) { }


		void Player::Air_state::enter() {}


		void Player::Air_state::exit() {}


		void Player::Air_state::handle_input(Timer::Seconds time_passed, const Input::Input_handler& input) {}


		void Player::Air_state::handle_event(const Events::Collided& message)
		{
			if(message.axis == Axis::y && message.movement_direction == Direction::positive)
			{
				player_->switch_state(std::make_unique<Idle_state>(*player_));
			}
		}


		Player::Grounded_state::Grounded_state(Player_input_component& player) : Player_state{player} {}


		void Player::Grounded_state::handle_input(Timer::Seconds time_passed, const Input::Input_handler& input)
		{
			if(input.get_action(Input::Action::jump, "game"))
			{
				player_->jump();
			}
			else if(input.get_state(Input::State::movement_down, "game") || input.get_processed_vector(Input::Range::movement_x, Input::Range::movement_y, "game").get_y() > 0.5)
			{
				player_->switch_state(std::make_unique<Crouch_state>(*player_));
			}
		}


		Player::Run_state::Run_state(Player_input_component& player) : Grounded_state{player} {}


		std::string Player::Run_state::state() const
		{
			return "Walking";
		}


		void Player::Run_state::handle_input(Timer::Seconds time_passed, const Input::Input_handler& input)
		{
			const auto x_input = Player_input_component::get_x_input(input);
			if (abs(x_input) == 0.0)
			{
				player_->switch_state(std::make_unique<Idle_state>(*player_));
			}
			else
			{
				movement_component_->velocity().set_x(x_input * max_speed_);

				if (std::abs(x_input) > 0.0)
					update_rotation(static_cast<Direction_x>(Math::sign(x_input)));
			}
		}


		Player::Idle_state::Idle_state(Player_input_component& player) : Grounded_state{player} {}


		std::string Player::Idle_state::state() const
		{
			return "Idle";
		}


		void Player::Idle_state::handle_input(Timer::Seconds time_passed, const Input::Input_handler& input)
		{
			const auto x_input = Player_input_component::get_x_input(input);
			if(abs(x_input) > 0.0)
			{
				player_->switch_state(std::make_unique<Run_state>(*player_));
			}
		}


		Player::Crouch_state::Crouch_state(Player_input_component& player) : Grounded_state{player} {}


		std::string Player::Crouch_state::state() const
		{
			return "Crouching";
		}


		void Player::Crouch_state::handle_input(Timer::Seconds time_passed, const Input::Input_handler& input)
		{
			if (!input.get_state(Input::State::movement_down, "game") && input.get_processed_vector(Input::Range::movement_x, Input::Range::movement_y, "game").get_y() <= 0.5)
			{
				player_->switch_state(std::make_unique<Idle_state>(*player_));
			}
		}


		Player_input_component::Player_input_component(
			Game_object& container,
			Movement_component& movement_component,
			double max_speed)
			: Component{container},
			  direction_{Direction_x::right},
			  max_speed_{max_speed},
			  movement_component_{&movement_component} { }


		void Player_input_component::handle_input(Timer::Seconds time_passed, const Input::Input_handler& input)
		{
			const auto x_input = get_x_input(input);

			movement_component_->velocity().set_x(x_input * max_speed_);

			if(std::abs(x_input) > 0.0)
				update_rotation(static_cast<Direction_x>(Math::sign(x_input)));
		}


		void Player_input_component::switch_state(std::unique_ptr<Player::Player_state>&& state)
		{
			state_->exit();
			state_ = move(state);
			state_->enter();
		}


		double Player_input_component::get_x_input(const Input::Input_handler& input)
		{
			const auto x_state_input = input.get_state(Input::State::movement_left, "game") * -1 + input.get_state(
				Input::State::movement_right,
				"game");
			auto range_input = input.get_processed_vector(Input::Range::movement_x, Input::Range::movement_y, "game");
			return Math::clamp_value(x_state_input + range_input.get_x(), -1.0, 1.0);
		}


		Io::json Player_input_component::to_json() const
		{
			return {
				{"type", type()},
				{"requires", std::vector<std::string>{Movement_component::type()}, {"max_speed", max_speed_}}
			};
		}


		std::string Player_input_component::component_type() const
		{
			return type();
		}


		std::string Player_input_component::type()
		{
			return "Player_input_component";
		}


		void Player_input_component::jump()
		{
			movement_component_->velocity().set_y(-20); // TODO: Get rid of magic number
			switch_state(std::make_unique<Player::Air_state>());
		}


		void Player_input_component::update_rotation(Direction_x direction)
		{
			direction_ = direction;

			game_object().send(Events::Direction_changed{direction});
		}


		Player_input_component* Player_input_component::from_json(
			const Io::json& json,
			Game_object& game_object,
			const Component_type_map& component_map)
		{
			return new Player_input_component{
				game_object,
				component_map.get_component<Movement_component>(),
				json.at("max_speed").get<double>()
			};
		}


		const Component::Deserializer Player_input_component::add_to_map{type(), from_json};
	}
}
