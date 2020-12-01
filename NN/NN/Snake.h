#pragma once
#include "Agent.h"
#include <SFML/Graphics.hpp>

namespace game {

    const long size = 10;

    enum class move_target {
        LEFT = 0,
        RIGHT = 1,
        UP = 2,
        DOWN = 3
    };

    struct cube {
    public:
        cube(sf::Vector2f coords, move_target target);

        move_target move(move_target target_next);

        move_target get_target() const;

        void draw(sf::RenderWindow& window, sf::Color color) const;

        bool introsects(const cube& cube) const;

        sf::Vector2f get_coords() const;

    private:
        move_target _move_target = move_target::UP;

        sf::Vector2f _coords;
    };

    class Snake {
    public:
        static const long number_of_input_neuron;

        Snake(genetic::Agent& agent, sf::Color color = sf::Color::Blue);

        void move(const sf::Vector2f& feed, long height, long width);

        void kill();

        void add_score();

        sf::Vector2f get_coords() const;

        void draw(sf::RenderWindow& window) const;

        bool live() const;

        long get_score() const;

        bool check_introsekt(sf::Vector2f coords) const;

        void set_color(sf::Color color = sf::Color::Blue);
    private:
        
        move_target _think(const sf::Vector2f& feed, long height, long width) const;

        std::vector<float> _create_inputs(const sf::Vector2f& feed, long height, long width) const;

        std::vector<float> _create_distance_to_obstacle(long height, long width) const;

        genetic::Agent& _agent;

        bool _alive = true;
        bool _add_cubes = false;

        std::vector<cube> _cubes;

        sf::Color _my_color;
    };
}