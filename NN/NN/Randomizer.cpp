#include "Randomizer.h"

namespace math {
    std::mt19937 Randomizer::_engine = std::mt19937((std::mt19937::result_type)
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
            ).count());

    std::uniform_real_distribution<float> Randomizer::_dist_zero_to_one = std::uniform_real_distribution<float>(0.0f, 1.0f);

    std::uniform_real_distribution<float> Randomizer::_dist_negative_one_to_positive_one
                = std::uniform_real_distribution<float>(Randomizer::default_min_parametr
                                                        , Randomizer::default_max_parametr);

    const float Randomizer::default_min_parametr = -1.0f;
    const float Randomizer::default_max_parametr = 1.0f;

    float Randomizer::get_float_dist_zero_to_one() {
        return Randomizer::_dist_zero_to_one(_engine);
    }

    float Randomizer::get_float_dist_negative_one_to_positive_one() {
        return Randomizer::_dist_negative_one_to_positive_one(_engine);
    }

    long Randomizer::get_long_rand(long min_val, long max_val) {
        std::uniform_int_distribution<long> dist(min_val, max_val);

        return dist(_engine);
    }
}