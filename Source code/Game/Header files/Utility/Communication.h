#pragma once

#include <functional>
#include <vector>


namespace Game
{
	namespace Communication
	{
		// Observer with templates and function callbacks

		template<typename... Args>
		class Dispatcher;


		// Takes a function argument with Args arguments. Not meant to be inherited from.
		template<typename... Args>
		class Receiver
		{
		public:
			explicit Receiver(std::function<void(Args ...)> function);
			~Receiver();

			void receive(Args ... args);

		private:
			std::function<void(Args ...)> function_;
			std::vector<Dispatcher<Args...>*> dispatchers_;

			friend class Dispatcher<Args...>;
		};


		// To be used together with Subjects of the same template type
		template<typename... Args>
		class Dispatcher
		{
		public:
			~Dispatcher();

			void add_receiver(Receiver<Args...>& receiver);
			void remove_receiver(Receiver<Args...>& receiver);
			void clear();
			void dispatch(Args ... args);

		private:
			std::vector<Receiver<Args...>*> receivers_;
		};


		template<typename... Args>
		Receiver<Args...>::Receiver(std::function<void(Args ...)> function) : function_{function} { }


		// Remove receiver from all subjects when it gets destroyed
		template<typename ... Args>
		Receiver<Args...>::~Receiver()
		{
			while(dispatchers_.size() > 0)
			{
				dispatchers_[0]->remove_receiver(*this);
			}
		}


		template<typename ... Args>
		void Receiver<Args...>::receive(Args ... args)
		{
			function_(args...);
		}


		template<typename ... Args>
		Dispatcher<Args...>::~Dispatcher()
		{
			for(auto receiver : receivers_)
			{
				receiver->dispatchers_.erase(std::remove(receiver->dispatchers_.begin(), receiver->dispatchers_.end(), this),
											 receiver->dispatchers_.end());
			}
		}


		template<typename ... Args>
		void Dispatcher<Args...>::add_receiver(Receiver<Args...>& receiver)
		{
			receivers_.push_back(&receiver);
			receiver.dispatchers_.push_back(this);
		}


		template<typename ... Args>
		void Dispatcher<Args...>::remove_receiver(Receiver<Args...>& receiver)
		{
			receivers_.erase(std::remove(receivers_.begin(), receivers_.end(), &receiver), receivers_.end());
			receiver.dispatchers_.erase(std::remove(receiver.dispatchers_.begin(), receiver.dispatchers_.end(), this),
										receiver.dispatchers_.end());
		}


		template<typename ... Args>
		void Dispatcher<Args...>::clear()
		{
			for (auto receiver : receivers_)
			{
				receiver->dispatchers_.erase(std::remove(receiver->dispatchers_.begin(), receiver->dispatchers_.end(), this),
					receiver->dispatchers_.end());
			}
			receivers_.clear();
		}


		template<typename ... Args>
		void Dispatcher<Args...>::dispatch(Args ... args)
		{
			for(auto observer : receivers_)
			{
				observer->receive(args...);
			}
		}
	}
}
