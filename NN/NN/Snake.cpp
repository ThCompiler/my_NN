#include "Snake.h"

namespace game {
    const long Snake::number_of_input_neuron = 2;

    Snake::Snake(genetic::Agent& agent, sf::Color color)
        : _agent(agent)
        , _my_color(color) {
        _cubes.push_back(cube(sf::Vector2f(95, 95), move_target::UP));
    }

    void Snake::move(const sf::Vector2f& feed, long height, long width) {
        if (!_alive) return;
        
        move_target next = _think(feed, height, width);
    
        cube new_cube = _cubes[_cubes.size() - 1];
        
        for (auto& cube : _cubes) {
            next = cube.move(next);
        }

        if (_add_cubes) {
            _cubes.push_back(new_cube);
            _add_cubes = false;
        }

        for (int i = 1; i < _cubes.size(); ++i) {
            if (_cubes[0].introsects(_cubes[i])) {
                kill();
            }
        }

        sf::Vector2f snake_coords = _cubes[0].get_coords();
        if (snake_coords.x < 0 || snake_coords.x > width
            || snake_coords.y < 0 || snake_coords.y > height) {
            kill();
        }
    }

    void game::Snake::kill() {
        _alive = false;
        //_agent.add_score(-1);
    }

    void Snake::add_score() {
        _agent.add_score(64);
        
        //_add_cubes = true;
    }

    sf::Vector2f Snake::get_coords() const
    {
        return _cubes[0].get_coords();
    }

    void Snake::draw(sf::RenderWindow& window) const {
        if (!_alive) return;

        for (const auto& cube : _cubes) {
            cube.draw(window, _my_color);
        }
    }

    bool Snake::live() const {
        return _alive;
    }

    long Snake::get_score() const
    {
        return _agent.get_score();
    }

    bool Snake::check_introsekt(sf::Vector2f coords) const {
        sf::RectangleShape snake(sf::Vector2f(size, size));
        sf::RectangleShape object(sf::Vector2f(size, size));

        snake.setPosition(_cubes[0].get_coords());
        object.setPosition(coords);

        return snake.getGlobalBounds().intersects(object.getGlobalBounds());
    }

    void Snake::set_color(sf::Color color) {
        _my_color = color;
    }

    move_target Snake::_think(const sf::Vector2f& feed, long height, long width) const {
        std::vector<float> outputs = _agent.get_result(_create_inputs(feed, height, width));

        long max_id = 0;

        for (long i = 1; i < outputs.size(); ++i) {
            if (outputs[i] > outputs[max_id]) {
                max_id = i;
            }
        }

        move_target last = _cubes[0].get_target();

        switch (max_id) {
        case 0:
            return last == move_target::UP ? move_target::LEFT :
                (last == move_target::LEFT ? move_target::DOWN :
                    (last == move_target::DOWN ? move_target::RIGHT : move_target::UP));
        case 1:
            return last;
        case 2:
            return last == move_target::UP ? move_target::RIGHT :
                (last == move_target::LEFT ? move_target::UP :
                    (last == move_target::DOWN ? move_target::LEFT : move_target::DOWN));
        default:
            return last;
        }
    }

    std::vector<float> Snake::_create_inputs(const sf::Vector2f& feed, long height, long width) const
    {
        std::vector<float> test = _create_distance_to_obstacle(height, width);
        std::vector<float> inputs;
        sf::Vector2f dist_to_target = { (feed.x - _cubes[0].get_coords().x)/size, (feed.y - _cubes[0].get_coords().y)/size };

        switch (_cubes[0].get_target())
        {
        case move_target::UP:
           /* inputs.push_back(test[(long)move_target::LEFT]);
            inputs.push_back(test[(long)move_target::UP]);
            inputs.push_back(test[(long)move_target::RIGHT]);*/
            inputs.push_back(dist_to_target.x);
            inputs.push_back(-dist_to_target.y);
            break;

        case move_target::LEFT:
            /*inputs.push_back(test[(long)move_target::DOWN]);
            inputs.push_back(test[(long)move_target::LEFT]);
            inputs.push_back(test[(long)move_target::UP]);*/
            inputs.push_back(-dist_to_target.y);
            inputs.push_back(-dist_to_target.x);
            break;

        case move_target::RIGHT:
           /* inputs.push_back(test[(long)move_target::UP]);
            inputs.push_back(test[(long)move_target::RIGHT]);
            inputs.push_back(test[(long)move_target::DOWN]);*/
            inputs.push_back(dist_to_target.y);
            inputs.push_back(dist_to_target.x);
            break;

        case move_target::DOWN:
            /*inputs.push_back(test[(long)move_target::RIGHT]);
            inputs.push_back(test[(long)move_target::DOWN]);
            inputs.push_back(test[(long)move_target::LEFT]);*/
            inputs.push_back(-dist_to_target.x);
            inputs.push_back(dist_to_target.y);
            break;

        default:
            throw std::exception("Something strange");
            break;
        }

        for (auto& in : inputs) {
            //if (in >= 0) {
                in /= ((float)width/size);
            //}
        }

        return inputs;
    }

    std::vector<float> Snake::_create_distance_to_obstacle(long height, long width) const {
        std::vector<float> test { _cubes[0].get_coords().x, (width - _cubes[0].get_coords().x)
                                  , _cubes[0].get_coords().y, (height - _cubes[0].get_coords().y) };

        for (const auto& cube : _cubes) {
            if (cube.get_coords().x == _cubes[0].get_coords().x) {
                if (cube.get_coords().y < _cubes[0].get_coords().y) {
                    test[(long)move_target::UP] = _cubes[0].get_coords().y - cube.get_coords().y;
                }
                if (cube.get_coords().y > _cubes[0].get_coords().y) {
                    test[(long)move_target::DOWN] = cube.get_coords().y - _cubes[0].get_coords().y;
                }
            }

            if (cube.get_coords().y == _cubes[0].get_coords().y) {
                if (cube.get_coords().x < _cubes[0].get_coords().x) {
                    test[(long)move_target::LEFT] = _cubes[0].get_coords().x - cube.get_coords().x;
                }
                if (cube.get_coords().x > _cubes[0].get_coords().x) {
                    test[(long)move_target::RIGHT] = cube.get_coords().x - _cubes[0].get_coords().x;
                }
            }
        }
        
        test[0] = test[0]/size > 1.0f ? 0.0f : float(width);
        test[1] = test[1]/size > 1.0f ? 0.0f : float(width);
        test[2] = test[2]/size > 1.0f ? 0.0f : float(width);
        test[3] = test[3]/size > 1.0f ? 0.0f : float(width);
        return test;
    }

    cube::cube(sf::Vector2f coords, move_target target)
        : _coords(coords) {}

    move_target cube::move(move_target target_next) {
        switch (_move_target) {
        case move_target::UP:
            _coords.y -= (double)size;
            break;
        case move_target::DOWN:
            _coords.y += (double)size;
            break;
        case move_target::RIGHT:
            _coords.x += (double)size;
            break;
        case move_target::LEFT:
            _coords.x -= (double)size;
            break;
        default:
            break;
        }

        move_target last_target = _move_target;

        if (_move_target != target_next) {
            _move_target = target_next;
        }

        return last_target;
    }

    move_target cube::get_target() const {
        return _move_target;
    }

    void cube::draw(sf::RenderWindow& window, sf::Color color) const {
        sf::RectangleShape restangle(sf::Vector2f(size, size));
        restangle.setOrigin(sf::Vector2f(size / 2.0f, size / 2.0f));
        restangle.setPosition(_coords);
        restangle.setFillColor(color);
        window.draw(restangle);
    }
    bool cube::introsects(const cube& cube) const {
        return cube._coords == _coords;
    }

    sf::Vector2f cube::get_coords() const {
        return _coords;
    }
}