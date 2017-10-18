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
		Player_input_component::Player_input_component(
			Game_object& container,
			Movement_component& movement_component,
			double max_speed)
			: Component{container},
			  direction_{Direction::right},
			  max_speed_{max_speed},
			  movement_component_{&movement_component} { }


		void Player_input_component::handle_input(Timer::Seconds time_passed, const Input::Input_handler& input)
		{
			const auto x_state_input = input.get_state(Input::State::movement_left, "game") * -1 + input.get_state(
				Input::State::movement_right,
				"game");
			auto range_input = input.get_processed_vector(Input::Range::movement_x, Input::Range::movement_y, "game");
			const auto x_input = Math::clamp_value(x_state_input + range_input.get_x(), -1.0, 1.0);

			movement_component_->velocity().set_x(x_input * max_speed_);
			// TODO: Make this work with vertical movement

			if(std::abs(x_input) > 0.0)
				update_rotation(static_cast<Direction>(Math::sign(x_input)));
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


		void Player_input_component::update_rotation(Direction direction)
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
