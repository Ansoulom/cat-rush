#pragma once

#include <functional>
#include <vector>


namespace Game
{
	namespace Communication
	{
		template<typename... Args>
		class Event
		{
		public:
			void add_listener(std::function<void(Args...)> function);
			// TODO: Come up with magical way to also remove listeners...

			void notify(Args ... args);
		private:
			std::vector<std::function<void(Args...)>> listeners_;
		};


		template<typename... Args>
		void Event<Args...>::add_listener(std::function<void(Args...)> function)
		{
			listeners_.emplace_back(function);
		}


		template<typename... Args>
		void Event<Args...>::notify(Args ... args)
		{
			for(auto& function : listeners_)
			{
				function(args...);
			}
		}
	}
}
