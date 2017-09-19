#include "Input_types.h"


namespace Game
{
	namespace Input
	{
		Button::Button(SDL_Keycode keyboard_button) : type{ Type::keyboard }, keyboard_button{ keyboard_button }, mouse_button{}, gamepad_button{}
		{
		}


		Button::Button(Uint8 mouse_button) : type{ Type::mouse }, keyboard_button{}, mouse_button{ mouse_button }, gamepad_button{}
		{
		}


		Button::Button(SDL_GameControllerButton controller_button) : type{ Type::gamepad }, keyboard_button{}, mouse_button{}, gamepad_button{ controller_button }
		{
		}


		bool Button::operator==(const Button& other) const
		{
			return type == other.type && keyboard_button == other.keyboard_button && mouse_button == other.mouse_button && gamepad_button == other.gamepad_button;
		}
	}
}
