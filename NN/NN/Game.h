#pragma once
#include <SFML/Graphics.hpp>
#include "Snake.h"
#include <chrono>

namespace game {
    const long window_size = 400;
    const long number_feed = 50;

    class Game {
    public:
        
        Game(Snake shake, const std::vector<sf::Vector2f>& feeds);

        void move();
        
        void draw(sf::RenderWindow& window, bool with_feed = false);

        bool end_game();

        long get_score() const;

        void set_color(sf::Color color = sf::Color::Blue);
    private:

        Snake _snake;

        long _last_point_snake;

        std::chrono::time_point<std::chrono::steady_clock> _last_chek 
                                        = std::chrono::steady_clock::now();
        std::vector<sf::Vector2f> _feeds;
    };
}