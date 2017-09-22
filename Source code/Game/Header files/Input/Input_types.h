#pragma once

#include <SDL.h>
#include <memory>
#include <functional>


namespace Game
{
	namespace Input
	{
		class Button;
	}
}


namespace std
{
	template<>
	struct hash<Game::Input::Button>
	{
		size_t operator()(const Game::Input::Button& button) const;
	};
}


namespace Game
{
	namespace Input
	{
		// Virtual input types

		enum class Action
		{
		};


		enum class Range
		{
			movement_x,
			movement_y
		};


		enum class State
		{
			movement_up,
			movement_left,
			movement_down,
			movement_right
		};


		// Hardware input types
		
		enum class Axis
		{
			mouse_x, mouse_y, mouse_scroll_x, mouse_scroll_y, controller_stick_left_x, controller_stick_left_y, controller_stick_right_x, controller_stick_right_y, controller_trigger_left, controller_trigger_right
		};


		// TODO: Maybe use std::variant instead
		class Button // Hardware button.
		{
		public:
			enum class Type // Different devices that can have buttons.
			{
				keyboard,
				mouse,
				gamepad
			};


			explicit Button(SDL_Keycode keyboard_button); // Constructs a keyboard button.
			explicit Button(Uint8 mouse_button); // Constructs a mouse button.
			explicit Button(SDL_GameControllerButton controller_button); // Constructs a game controller button.

			bool operator==(const Button& other) const;

		private:
			Type type_;
			SDL_Keycode keyboard_button_;
			Uint8 mouse_button_;
			SDL_GameControllerButton gamepad_button_;

			friend size_t std::hash<Button>::operator()(const Button& button) const;
		};
	}
}


namespace std
{
	template<>
	struct hash<Game::Input::Button::Type>
	{
		size_t operator()(const Game::Input::Button::Type& button_type) const
		{
			return static_cast<size_t>(button_type);
		}
	};


	template<>
	struct hash<SDL_GameControllerButton>
	{
		size_t operator()(const SDL_GameControllerButton& button) const
		{
			return static_cast<size_t>(button);
		}
	};


	inline size_t hash<Game::Input::Button>::operator()(const Game::Input::Button& button) const
	{
		auto type_hash = hash<Game::Input::Button::Type>()(button.type_);
		auto keyboard_button_hash = hash<SDL_Keycode>()(button.keyboard_button_);
		auto mouse_button_hash = hash<Uint8>()(button.mouse_button_);
		auto gamepad_button_hash = hash<SDL_GameControllerButton>()(button.gamepad_button_);

		return ((((type_hash ^ (keyboard_button_hash << 1)) >> 1) ^ (mouse_button_hash << 1)) >> 1) ^ (gamepad_button_hash << 1);
	}


	template<>
	struct hash<Game::Input::Range>
	{
		size_t operator()(const Game::Input::Range& range) const
		{
			return static_cast<size_t>(range);
		}
	};


	template<>
	struct hash<Game::Input::State>
	{
		size_t operator()(const Game::Input::State& state) const
		{
			return static_cast<size_t>(state);
		}
	};


	template<>
	struct hash<Game::Input::Action>
	{
		size_t operator()(const Game::Input::Action& action) const
		{
			return static_cast<size_t>(action);
		}
	};


	template<>
	struct hash<Game::Input::Axis>
	{
		size_t operator()(const Game::Input::Axis& axis) const
		{
			return static_cast<size_t>(axis);
		}
	};
}
