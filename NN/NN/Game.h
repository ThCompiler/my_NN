#pragma once
#include <SFML/Graphics.hpp>
#include "Snake.h"
#include <chrono>

namespace game {
    const long window_size = 400;
    const long number_feed = 50;

    class Game {
    public:
        
        Game(Move_snake shake);

        void move(std::vector<float> inputs, std::vector<long> right);
        
        void draw(sf::RenderWindow& window);

        bool end_game();

        long get_score() const;

        void set_color(sf::Color color = sf::Color::Blue);

        Move_snake& get_snake();
    private:

        Move_snake _snake;

        long _last_point_snake;

        std::chrono::time_point<std::chrono::steady_clock> _last_chek 
                                        = std::chrono::steady_clock::now();
    };
}