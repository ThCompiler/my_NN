#pragma once
#include <chrono>
#include <random>

namespace math {
	class Randomizer
	{
	public:
		Randomizer() = delete;
		~Randomizer() = delete;

		static const float default_min_parametr;
		static const float default_max_parametr;

		static float get_float_dist_zero_to_one();

		static float get_float_dist_negative_one_to_positive_one();

		static long get_long_rand(long min_val, long max_val);

	private:
		static std::mt19937 _engine;

		static std::uniform_real_distribution<float> _dist_zero_to_one;

		static std::uniform_real_distribution<float> _dist_negative_one_to_positive_one;
	};
}