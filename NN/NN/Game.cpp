#include "Game.h"

namespace game {
    Game::Game(Move_snake shake)
        : _snake(shake)
        , _last_point_snake(0) {}

    void game::Game::move(std::vector<float> inputs, std::vector<long> right) {
        if (end_game()) return;
        long ans = (long)_snake.move(window_size, window_size, inputs);
        for (auto a : right) {
            if (ans == a) {
                _snake.add_score();
                break;
            }
            else {
                long a = 0;
            }
        }
    }

    void Game::draw(sf::RenderWindow& window) {
        if (end_game()) return;

        _snake.draw(window);
    }

    bool Game::end_game() {
        return !_snake.live();
    }

    long Game::get_score() const {
        return _snake.get_score();
    }
    void Game::set_color(sf::Color color) {
        _snake.set_color(color);
    }

    Move_snake& Game::get_snake() {
        return _snake;
    }
}
