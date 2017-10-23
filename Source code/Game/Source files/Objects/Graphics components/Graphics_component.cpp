#include "Graphics_component.h"


namespace Game
{
	namespace Objects
	{
		Graphics_component::Graphics_component(Game_object& game_object) : Component{game_object} { }


		Graphics_component::~Graphics_component()
		{
			destroy_event_.notify(*this);
		}


		void Graphics_component::receive(const Events::Message& message)
		{
			// TODO: Implement
		}


		void Graphics_component::add_destroy_listener(std::function<void(Graphics_component&)> function)
		{
			destroy_event_.add_listener(function);
		}
	}
}
