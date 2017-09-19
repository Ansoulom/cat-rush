#include "Player_input_component.h"
#include "Input_handler.h"
#include "Vector.h"
#include "Math_functions.h"
#include "Game_object.h"


namespace Game
{
	namespace Objects
	{
		Player_input_component::Player_input_component(Game_object* container) : Component{container},
																				smooth_input{Geometry::Vector<double>(0, 0)}, angle{0}, last_input_angle{0}, direction{0}
		{
		}


		void Player_input_component::handle_input(Timer::Seconds time_passed, const Input::Input_handler& input)
		{
			const auto x_state_input = input.get_state(Input::State::movement_left, "game") * -1 + input.get_state(
				Input::State::movement_right, "game");
			const auto y_state_input = input.get_state(Input::State::movement_up, "game") * -1 + input.get_state(
				Input::State::movement_down, "game");
			auto range_input = input.get_processed_vector(Input::Range::movement_x, Input::Range::movement_y, "game");
			auto input_vector = Geometry::Vector<double>{
				x_state_input + range_input.get_x(), y_state_input + range_input.get_y()
			};
			input_vector.normalize();

			game_object->send(Events::Change_velocity_normalized{input_vector});

			update_rotation(input_vector, time_passed, *game_object);
		}


		IO::json Player_input_component::to_json() const
		{
			return {{"type", "player"}};
		}


		void Player_input_component::update_rotation(Geometry::Vector<double> input, Timer::Seconds time_passed,
													Game_object& object)
		{
			auto input_smoothing_movement = 1.0 / rotation_input_smoothing_time.count() * time_passed.count();
			auto input_smoothing_direction = input - smooth_input;
			if(input_smoothing_movement > input_smoothing_direction.get_magnitude())
				input_smoothing_movement =
					input_smoothing_direction.get_magnitude();
			input_smoothing_direction.set_magnitude(1.0);
			smooth_input += input_smoothing_direction * input_smoothing_movement;

			if(input.get_magnitude() > 0.0)
			{
				last_input_angle = smooth_input.get_angle();
			}

			auto rotation_angle = last_input_angle - angle;
			if(rotation_angle > 180.0 || rotation_angle < -180.0)
			{
				rotation_angle += 360.0;
			}

			auto rotation_speed = 180.0 / max_rotation_time.count() * Math::sign(rotation_angle) * time_passed.count();
			if(abs(rotation_speed) > abs(rotation_angle))
			{
				rotation_speed = rotation_angle;
			}

			angle += rotation_speed;
			angle = Math::clamp_angle(angle);

			game_object->send(Events::Rotation_changed{angle});
		}


		Player_input_component* Player_input_component::from_json(const IO::json& json, Game_object* game_object,
																const Component_loader& loader)
		{
			return new Player_input_component{game_object};
		}
	}
}
