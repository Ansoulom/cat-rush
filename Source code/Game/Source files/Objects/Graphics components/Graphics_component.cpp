#include "Graphics_component.h"
#include "World.h"


namespace Game
{
	namespace Objects
	{
		Graphics_component::Graphics_component(Game_object& game_object) : Component{game_object}
		{
			game_object.world().component_loader().register_component(*this);
		}


		Graphics_component::~Graphics_component()
		{
			destroy_event_.dispatch(*this);
		}


		void Graphics_component::receive(const Events::Message& message)
		{
			visit([this](const auto& msg) { handle_event(msg); }, message);
		}


		void Graphics_component::add_destroy_receiver(Communication::Receiver<Graphics_component&>& receiver)
		{
			destroy_event_.add_receiver(receiver);
		}


		void Graphics_component::add_moved_receiver(
			Communication::Receiver<Graphics_component&, Geometry::Vector<double>>& receiver)
		{
			moved_event_.add_receiver(receiver);
		}


		void Graphics_component::handle_event(const Events::Position_changed& message)
		{
			moved_event_.dispatch(*this, message.start_position);
		}
	}
}
