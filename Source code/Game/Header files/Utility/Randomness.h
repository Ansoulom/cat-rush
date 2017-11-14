#pragma once

#include <random>
#include <functional>


namespace Game
{
	namespace Random
	{
		template<typename Integer_type = int>
		class Random_integer_generator
		{
		public:
			Random_integer_generator(Integer_type min, Integer_type max);

			Integer_type operator()() const;

		private:
			std::function<Integer_type()> r_;
		};


		template<typename Integer_type>
		Random_integer_generator<Integer_type>::Random_integer_generator(Integer_type min, Integer_type max) : r_{
			bind(std::uniform_int_distribution<Integer_type>{min, max}, std::default_random_engine{})
		} {}


		template<typename Integer_type>
		Integer_type Random_integer_generator<Integer_type>::operator()() const
		{
			return r_();
		}


		template<typename Floating_type = double>
		class Random_floating
		{
		public:
			Random_floating(Floating_type min, Floating_type max);

			Floating_type operator()() const;

		private:
			std::function<Floating_type()> r_;
		};


		template<typename Floating_type>
		Random_floating<Floating_type>::Random_floating(Floating_type min, Floating_type max) : r_{
			bind(std::uniform_real_distribution<Floating_type>{min, max}, std::default_random_engine{})
		} {}


		template<typename Floating_type>
		Floating_type Random_floating<Floating_type>::operator()() const
		{
			return r_();
		}
	}
}
