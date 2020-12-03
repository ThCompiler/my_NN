#include "Game.h"

namespace game {
    Game::Game(Snake shake, const std::vector<sf::Vector2f>& feeds)
        : _snake(shake)
        , _feeds(feeds)
        , _last_point_snake(0) {}

    void game::Game::move() {
        if (end_game()) return;
        _snake.move(_feeds.back(), window_size, window_size);


        if (_snake.get_coords() == _feeds.back()) {
            _feeds.pop_back();
            _snake.add_score();
        }
    }

    void Game::draw(sf::RenderWindow& window, bool with_feed) {
        if (end_game()) return;

        _snake.draw(window);

        if (!with_feed) return;

        sf::CircleShape circle(size / 2.0f);

        circle.setFillColor(sf::Color::Green);
        circle.setOrigin(sf::Vector2f(size / 2.0f, size / 2.0f));
        circle.setPosition(_feeds.back());
        window.draw(circle);
    }

    bool Game::end_game() {
        if (_snake.get_score() != _last_point_snake) {
            _last_point_snake = _snake.get_score();
            _last_chek = std::chrono::steady_clock::now();
        }
        return !_snake.live() || _feeds.size() == 0;
    }

    long Game::get_score() const {
        return _snake.get_score();
    }
    void Game::set_color(sf::Color color) {
        _snake.set_color(color);
    }
}
