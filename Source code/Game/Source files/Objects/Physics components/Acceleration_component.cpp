#include "Acceleration_component.h"
#include "Movement_component.h"


namespace Game
{
	namespace Objects
	{
		Acceleration_component::Acceleration_component(
			Game_object& game_object,
			Geometry::Vector<double> acceleration,
			Movement_component& movement_component)
			: Component{game_object}, acceleration_{acceleration}, movement_component_{&movement_component} {}


		void Acceleration_component::update(Timer::Seconds time_passed)
		{
			movement_component_->velocity() += acceleration_;
		}


		std::string Acceleration_component::component_type() const
		{
			return type();
		}


		std::string Acceleration_component::type()
		{
			return "Acceleration_component";
		}


		Acceleration_component* Acceleration_component::from_json(
			const Io::json& json,
			Game_object& game_object,
			const Component_type_map& component_map)
		{
			return new Acceleration_component{
				game_object,
				{json.at("x_acceleration").get<double>(), json.at("y_acceleration").get<double>()},
				component_map.get_component<Movement_component>()
			};
		}


		Io::json Acceleration_component::to_json() const
		{
			return {
				{"type", type()},
				{"requires", std::vector<std::string>{Movement_component::type()}},
				{"x_acceleration", acceleration_.get_x()},
				{"y_acceleration", acceleration_.get_y()}
			};
		}


		const Component::Deserializer Acceleration_component::add_to_map{
			type(),
			from_json
		};
	}
}
