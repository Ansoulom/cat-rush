#include "Player_input_component.h"
#include "Input_handler.h"
#include "Vector.h"
#include "Math_functions.h"
#include "Game_object.h"


namespace Game
{
	namespace Objects
	{
		Player_input_component::Player_input_component(Game_object& container) : Component{container}, direction_{Direction::right}
		{
		}


		void Player_input_component::handle_input(Timer::Seconds time_passed, const Input::Input_handler& input)
		{
			const auto x_state_input = input.get_state(Input::State::movement_left, "game") * -1 + input.get_state(
				Input::State::movement_right, "game");
			auto range_input = input.get_processed_vector(Input::Range::movement_x, Input::Range::movement_y, "game");
			const auto x_input = Math::clamp_value(x_state_input + range_input.get_x(), -1.0, 1.0);

			game_object().send(Events::Change_velocity_normalized{ {x_input, 0.0} }); // TODO: Make this work with vertical movement

			update_rotation(static_cast<Direction>(Math::sign(x_input)));
		}


		IO::json Player_input_component::to_json() const
		{
			return {{"type", "Player_input_component"}};
		}


		void Player_input_component::update_rotation(Direction direction)
		{
			direction_ = direction;

			game_object().send(Events::Direction_changed{direction});
		}


		Player_input_component* Player_input_component::from_json(const IO::json& json, Game_object& game_object)
		{
			return new Player_input_component{game_object};
		}


		const Component::Deserializer Player_input_component::add_to_map{ "Player_input_component", from_json };
	}
}
