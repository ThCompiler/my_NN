#pragma once
#include "GeneticAlgorithm.h"
#include "Game.h"

namespace genetic {
    const long size_of_population = 50;

    class Evolution {
    public:
        Evolution();

        Evolution(std::string path1, std::string path2);

        void evolution();

        void save() const;

        static bool do_evolution;

        void load(std::string path);

        ~Evolution();
    private:

        std::vector<float> _generate_inputs() const;

        std::vector<long> _generate_answers(long target, const std::vector<float>& inputs, int targets) const;

        void _simulate_game(std::vector<std::vector<float>>inputs, std::vector<std::vector<long>> answers, std::vector<long> targets);

        void _restart(bool save_score = false);

        bool _check_save();

        std::vector<Agent> _agents;

        std::vector<game::Game> _games;

        sf::RenderWindow _window;

        long long _number_of_generation;
    };
}