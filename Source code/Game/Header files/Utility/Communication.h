#pragma once

#include <functional>
#include <vector>


namespace Game
{
	namespace Communication
	{
		// Observer with templates and function callbacks

		template <typename... Args>
		class Subject;

		// Takes a function argument with Args arguments. Not meant to be inherited from.
		template <typename... Args>
		class Observer
		{
		public:
			Observer(std::function<void(Args ...)> function);
			~Observer();

			void on_notify(Args ... args);

		private:
			std::function<void(Args ...)> function_;
			std::vector<Subject<Args...>*> subjects_;

			friend class Subject<Args...>;
		};


		// To be used together with Subjects of the same template type
		template <typename... Args>
		class Subject
		{
		public:
			~Subject();

			void add_observer(Observer<Args...>& observer);
			void remove_observer(Observer<Args...>& observer);
			void notify(Args ... args);

		private:
			std::vector<Observer<Args...>*> observers_;
		};


		template <typename... Args>
		Observer<Args...>::Observer(std::function<void(Args ...)> function) : function_{ function }
		{
		}


		// Remove observer from all subjects when it gets destroyed
		template <typename ... Args>
		Observer<Args...>::~Observer()
		{
			while (subjects_.size() > 0)
			{
				subjects_[0]->remove_observer(*this);
			}
		}


		template <typename ... Args>
		void Observer<Args...>::on_notify(Args ... args)
		{
			function_(args...);
		}


		template <typename ... Args>
		Subject<Args...>::~Subject()
		{
			for (auto observer : observers_)
			{
				observer->subjects_.erase(std::remove(observer->subjects_.begin(), observer->subjects_.end(), this),
					observer->subjects_.end());
			}
		}


		template <typename ... Args>
		void Subject<Args...>::add_observer(Observer<Args...>& observer)
		{
			observers_.push_back(&observer);
			observer.subjects_.push_back(this);
		}


		template <typename ... Args>
		void Subject<Args...>::remove_observer(Observer<Args...>& observer)
		{
			observers_.erase(std::remove(observers_.begin(), observers_.end(), &observer), observers_.end());
			observer.subjects_.erase(std::remove(observer.subjects_.begin(), observer.subjects_.end(), this),
				observer.subjects_.end());
		}


		template <typename ... Args>
		void Subject<Args...>::notify(Args ... args)
		{
			for (auto observer : observers_)
			{
				observer->on_notify(args...);
			}
		}
	}
}
