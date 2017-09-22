#include "Input_types.h"


namespace Game
{
	namespace Input
	{
		Button::Button(SDL_Keycode keyboard_button) : type_{ Type::keyboard }, keyboard_button_{ keyboard_button }, mouse_button_{}, gamepad_button_{}
		{
		}


		Button::Button(Uint8 mouse_button) : type_{ Type::mouse }, keyboard_button_{}, mouse_button_{ mouse_button }, gamepad_button_{}
		{
		}


		Button::Button(SDL_GameControllerButton controller_button) : type_{ Type::gamepad }, keyboard_button_{}, mouse_button_{}, gamepad_button_{ controller_button }
		{
		}


		bool Button::operator==(const Button& other) const
		{
			return type_ == other.type_ && keyboard_button_ == other.keyboard_button_ && mouse_button_ == other.mouse_button_ && gamepad_button_ == other.gamepad_button_;
		}
	}
}
