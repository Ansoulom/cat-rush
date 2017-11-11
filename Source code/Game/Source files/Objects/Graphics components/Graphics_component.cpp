#include "Graphics_component.h"


namespace Game
{
	namespace Objects
	{
		Graphics_component::Graphics_component(Game_object& game_object) : Component{game_object} { }


		Graphics_component::~Graphics_component()
		{
			destroy_event_.dispatch(*this);
		}


		void Graphics_component::receive(const Events::Message& message)
		{
			// TODO: Implement
		}


		void Graphics_component::add_destroy_listener(Communication::Receiver<Graphics_component&> listener)
		{
			destroy_event_.add_receiver(listener);
		}
	}
}
