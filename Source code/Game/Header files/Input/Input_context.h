#pragma once

#include <unordered_map>
#include "Input_types.h"


namespace Game
{
	namespace Input
	{
		class Context
		{
		public:
			Context();

			void add_mapping(Button button, Action action);
			void add_mapping(Button button, State state);
			void add_mapping(Axis axis, Range range);
			bool get_action(Action action) const;
			bool get_state(State state) const;
			double get_range(Range range) const;
			void handle_event(const SDL_Event& event);
			void clear_actions();
			void reset();

		private:
			std::unordered_map<Button, Action> action_mappings;
			std::unordered_map<Button, State> state_mappings;
			std::unordered_map<Axis, Range> range_mappings;
			std::unordered_map<Action, bool> actions;
			std::unordered_map<State, bool> states;
			std::unordered_map<Range, double> ranges;
		};
	}
}