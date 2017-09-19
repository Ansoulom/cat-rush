#include "Graphics_component.h"


namespace Game
{
	namespace Objects
	{
		Graphics_component::Graphics_component(Game_object* game_object) : Component{game_object}
		{
		}


		Graphics_component::~Graphics_component()
		{
			destroy_event.notify(*this);
			// TODO: Make sure to unregister from renderer, also make sure that move works correctly
		}


		void Graphics_component::receive(const Events::Message& message)
		{
			// TODO: Implement
		}


		void Graphics_component::add_destroy_listener(std::function<void(Graphics_component&)> function)
		{
			destroy_event.add_listener(function);
		}
	}
}
