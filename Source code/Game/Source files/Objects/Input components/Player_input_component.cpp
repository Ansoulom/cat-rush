#include "Player_input_component.h"
#include "Input_handler.h"
#include "Vector.h"
#include "Math_functions.h"
#include "Game_object.h"
#include <valarray>
#include <chrono>
#include "Movement_component.h"
#include "Collider_component.h"
#include "World.h"
#include "Combat_component.h"


namespace Game
{
	namespace Objects
	{
		// Player_input_component

		Player_input_component::Player_input_component(
			Game_object& container,
			Movement_component& movement_component,
			double max_speed,
			Collider_component& collider_component,
			Geometry::Rectangle<double> stand_hitbox,
			Geometry::Rectangle<double> crouch_hitbox)
			: Component{container},
			  direction_{Direction_x::right},
			  max_speed_{max_speed},
			  state_{std::make_unique<Idle_state>(*this)},
			  old_state_{state_->state()},
			  movement_component_{&movement_component},
			  collider_component_{&collider_component},
			  stand_hitbox_{stand_hitbox},
			  crouch_hitbox_{crouch_hitbox}
		{
			state_->enter();
		}


		void Player_input_component::attack()
		{
			attacking_ = true;
			old_state_ = "Attacking";
			game_object().send(Events::State_changed{old_state_});

			// EXPERIMENTAL TODO: Remove magic numbers and shit
			const auto damage = 3;
			const auto offset = Geometry::Vector<double>{static_cast<int>(direction_) * 0.2f, 0.0};
			const auto width = 0.2f;
			const auto height = 0.2f;
			auto collisions = game_object().world().collision_system().get_collisions(
				Geometry::Rectangle<double>{game_object().position() + offset, width, height},
				"enemies");
			for(auto collider : collisions)
			{
				if(auto combat_component = collider->game_object().find_component<Combat_component>())
				{
					combat_component->damage(damage);
				}
			}
		}


		void Player_input_component::handle_input(Timer::Seconds time_passed, const Input::Input_handler& input)
		{
			while(state_->handle_input(time_passed, input)) {}
			if(!attacking_ && state_->state() != old_state_)
			{
				old_state_ = state_->state();
				game_object().send(Events::State_changed{old_state_});
			}
		}


		void Player_input_component::switch_state(std::unique_ptr<Player_state>&& state)
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
				{
					"requires",
					std::vector<std::string>{Movement_component::type()},
					{"max_speed", max_speed_},
					{"stand_hitbox", stand_hitbox_.to_json()}
				}
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


		void Player_input_component::receive(const Events::Message& message)
		{
			state_->receive(message);
			visit([this](const auto& msg) { handle_event(msg); }, message);
		}


		void Player_input_component::jump()
		{
			movement_component_->velocity().set_y(-3.0); // TODO: Get rid of magic number
			switch_state(std::make_unique<Air_state>(*this));
		}


		void Player_input_component::handle_event(const Events::Animation_finished& message)
		{
			if(message.animation == "Attacking") attacking_ = false;
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
			const auto stand = std::unique_ptr<Geometry::Rectangle<double>>{
				Geometry::Rectangle<double>::from_json(json.at("stand_hitbox"))
			};
			const auto crouch = std::unique_ptr<Geometry::Rectangle<double>>{
				Geometry::Rectangle<double>::from_json(json.at("crouch_hitbox"))
			};

			return new Player_input_component{
				game_object,
				component_map.get_component<Movement_component>(),
				json.at("max_speed").get<double>(),
				component_map.get_component<Collider_component>(),
				Geometry::Rectangle<double>{*stand},
				Geometry::Rectangle<double>{*crouch}
			};
		}


		const Component::Deserializer Player_input_component::add_to_map{type(), from_json};


		// Player_state

		Player_input_component::Player_state::Player_state(Player_input_component& player) : player_{&player} { }


		Player_input_component::Player_state::~Player_state() {}


		void Player_input_component::Player_state::receive(const Events::Message& message) { }


		// Air_state

		Player_input_component::Air_state::Air_state(Player_input_component& player) : Player_state{player} { }


		void Player_input_component::Air_state::receive(const Events::Message& message)
		{
			visit([this](const auto& msg) { handle_event(msg); }, message);
		}


		std::string Player_input_component::Air_state::state() const
		{
			return "Jumping";
		}


		void Player_input_component::Air_state::enter() {}


		void Player_input_component::Air_state::exit() {}


		bool Player_input_component::Air_state::handle_input(Timer::Seconds time_passed, const Input::Input_handler& input)
		{
			const auto x_input = get_x_input(input);
			player_->movement_component_->velocity().set_x(x_input * player_->max_speed_);
			if(!player_->attacking_ && input.get_action(Input::Action::attack, "game"))
			{
				player_->attack();
			}
			return false;
		}


		void Player_input_component::Air_state::handle_event(const Events::Collided& message)
		{
			if(message.axis == Axis::y && message.movement_direction == Direction::positive)
			{
				player_->switch_state(std::make_unique<Idle_state>(*player_));
			}
		}


		// Ground_state

		Player_input_component::Ground_state::Ground_state(Player_input_component& player) : Player_state{player} {}


		bool Player_input_component::Ground_state::handle_input(
			Timer::Seconds time_passed,
			const Input::Input_handler& input)
		{
			if(input.get_action(Input::Action::jump, "game"))
			{
				player_->jump();
				return true;
			}

			return false;
		}


		// Walk_state

		Player_input_component::Walk_state::Walk_state(Player_input_component& player) : Ground_state{player} {}


		void Player_input_component::Walk_state::enter() {}


		void Player_input_component::Walk_state::exit() {}


		std::string Player_input_component::Walk_state::state() const
		{
			return "Walking";
		}


		bool Player_input_component::Walk_state::handle_input(Timer::Seconds time_passed, const Input::Input_handler& input)
		{
			if(!player_->attacking_ && input.get_action(Input::Action::attack, "game"))
			{
				player_->attack();
			}

			// Crouch check
			if(!player_->attacking_ && input.get_state(Input::State::movement_down, "game") || input.get_processed_vector(
				Input::Range::movement_x,
				Input::Range::movement_y,
				"game").get_y() > 0.5)
			{
				player_->switch_state(std::make_unique<Crouch_state>(*player_));
				return true;
			}

			const auto x_input = get_x_input(input);
			player_->movement_component_->velocity().set_x(x_input * player_->max_speed_);

			// Idle check
			if(abs(x_input) == 0.0)
			{
				player_->switch_state(std::make_unique<Idle_state>(*player_));
				return true;
			}

			player_->update_rotation(static_cast<Direction_x>(Math::sign(x_input)));

			return Ground_state::handle_input(time_passed, input);
		}


		// Idle_state

		Player_input_component::Idle_state::Idle_state(Player_input_component& player) : Ground_state{player} {}


		void Player_input_component::Idle_state::enter()
		{
			player_->movement_component_->velocity().set_x(0.0);
		}


		void Player_input_component::Idle_state::exit() {}


		std::string Player_input_component::Idle_state::state() const
		{
			return "Idle";
		}


		bool Player_input_component::Idle_state::handle_input(Timer::Seconds time_passed, const Input::Input_handler& input)
		{
			if(!player_->attacking_ && input.get_action(Input::Action::attack, "game"))
			{
				player_->attack();
			}

			// Crouch check
			if(!player_->attacking_ && input.get_state(Input::State::movement_down, "game") || input.get_processed_vector(
				Input::Range::movement_x,
				Input::Range::movement_y,
				"game").get_y() > 0.5)
			{
				player_->switch_state(std::make_unique<Crouch_state>(*player_));
				return true;
			}

			// Walking check
			const auto x_input = get_x_input(input);
			if(abs(x_input) > 0.0)
			{
				player_->switch_state(std::make_unique<Walk_state>(*player_));
				return true;
			}

			return Ground_state::handle_input(time_passed, input);
		}


		// Crouch_state

		Player_input_component::Crouch_state::Crouch_state(Player_input_component& player) : Ground_state{player} {}


		void Player_input_component::Crouch_state::enter()
		{
			player_->movement_component_->velocity().set_x(0.0);
			player_->collider_component_->set_shape(player_->crouch_hitbox_);
		}


		void Player_input_component::Crouch_state::exit()
		{
			player_->collider_component_->set_shape(player_->stand_hitbox_);
		}


		std::string Player_input_component::Crouch_state::state() const
		{
			return "Crouching";
		}


		bool Player_input_component::Crouch_state::handle_input(Timer::Seconds time_passed, const Input::Input_handler& input)
		{
			// Stopped crouching check
			if(!input.get_state(Input::State::movement_down, "game") && input.get_processed_vector(
				Input::Range::movement_x,
				Input::Range::movement_y,
				"game").get_y() <= 0.5)
			{
				player_->switch_state(std::make_unique<Idle_state>(*player_));
				return true;
			}

			return Ground_state::handle_input(time_passed, input);
		}
	}
}
