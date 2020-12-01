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

        void _simulate_game();

        void _restart();

        bool _check_save();

        std::vector<sf::Vector2f> _generate_feed(long size) const;

        std::vector<Agent> _agents;

        std::vector<game::Game> _games;

        sf::RenderWindow _window;

        std::vector<sf::Vector2f> _feeds;

        long long _number_of_generation;
    };
}