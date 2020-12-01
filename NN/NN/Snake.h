#pragma once
#include "Agent.h"
#include <SFML/Graphics.hpp>

namespace game {

    const long size = 10;

    enum class move_target {
        LEFT = 0,
        RIGHT = 1,
        UP = 2,
        DOWN = 3,
        INCORRECT
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

    class Move_snake {
    public:
        static const long number_of_input_neuron;

        Move_snake(genetic::Agent& agent, sf::Color color = sf::Color::Blue);

        move_target move(long height, long width, std::vector<float> inputs);

        void kill();

        void add_score();

        sf::Vector2f get_coords() const;

        void draw(sf::RenderWindow& window) const;

        bool live() const;

        long get_score() const;

        bool check_introsekt(sf::Vector2f coords) const;

        void set_color(sf::Color color = sf::Color::Blue);

        long get_count_of_step() const;

        bool generate_snake(long lenght);

        void set_coords(sf::Vector2f coords);

        std::vector<cube>& get_snake();

        void set_snake(std::vector<cube>& snake);

        void set_target(move_target target);

        move_target get_target();
    private:
        
        move_target _think(long height, long width) const;

        std::vector<float> _create_inputs(long height, long width) const;

        std::vector<float> _create_distance_to_obstacle(long height, long width) const;

        move_target _target_for_generate_next_cube(move_target last) const;

        move_target _check_target(move_target target) const;

        genetic::Agent& _agent;

        bool _alive = true;
        bool _add_cubes = false;

        std::vector<cube> _cubes;

        sf::Color _my_color;

        long _counter_of_step;
    };
}