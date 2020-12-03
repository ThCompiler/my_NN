#include "Snake.h"

namespace game {
    const long Snake::number_of_input_neuron = 5;

    Snake::Snake(genetic::Agent& agent_moving, genetic::Agent& agent_eating, sf::Color color)
        : _agent_moving(agent_moving)
        , _agent_eating(agent_eating)
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
        _agent_eating.add_score(-1);
    }

    void Snake::add_score() {
        _agent_eating.add_score(64);
        
        _add_cubes = true;
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
        return _agent_eating.get_score();
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
        std::vector<float> outputs = _think_eating(feed, height, width);

        move_target wait_moving = _convert_otputs(outputs);

        outputs = _think_moving(feed, height, width, wait_moving);

        if (outputs[3] > 0.5f) {
            return wait_moving;
        }
        outputs.pop_back();

        return _convert_otputs(outputs);        
    }

    std::vector<float> Snake::_think_moving(const sf::Vector2f& feed, long height, long width, move_target wait_moving) const {
        cube tmp = _cubes[0];
        tmp.move(wait_moving);
        std::vector<float> test = _create_distance_to_obstacle(height, width, tmp);
        std::vector<float> inputs;

        switch (_cubes[0].get_target())
        {
        case move_target::UP:
            inputs.push_back(test[(long)move_target::LEFT]);
            inputs.push_back(test[(long)move_target::UP]);
            inputs.push_back(test[(long)move_target::RIGHT]);
            break;

        case move_target::LEFT:
            inputs.push_back(test[(long)move_target::DOWN]);
            inputs.push_back(test[(long)move_target::LEFT]);
            inputs.push_back(test[(long)move_target::UP]);
            break;

        case move_target::RIGHT:
            inputs.push_back(test[(long)move_target::UP]);
            inputs.push_back(test[(long)move_target::RIGHT]);
            inputs.push_back(test[(long)move_target::DOWN]);
            break;

        case move_target::DOWN:
            inputs.push_back(test[(long)move_target::RIGHT]);
            inputs.push_back(test[(long)move_target::DOWN]);
            inputs.push_back(test[(long)move_target::LEFT]);
            break;

        default:
            throw std::exception("Something strange");
            break;
        }

        return _agent_moving.get_result(inputs);
    }

    move_target Snake::_convert_otputs(std::vector<float> outputs) const {
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

    std::vector<float> Snake::_think_eating(const sf::Vector2f& feed, long height, long width) const {
        sf::Vector2f dist_to_target = { (feed.x - _cubes[0].get_coords().x) / size, (feed.y - _cubes[0].get_coords().y) / size };

        float normal_width = (float)width / size;
        float normal_height = (float)height / size;

        std::vector<float>inputs;
        switch (_cubes[0].get_target())
        {
        case move_target::UP:
            inputs.push_back(dist_to_target.x / normal_width);
            inputs.push_back(-dist_to_target.y / normal_height);
            break;

        case move_target::LEFT:
            inputs.push_back(-dist_to_target.y / normal_height);
            inputs.push_back(-dist_to_target.x / normal_width);
            break;

        case move_target::RIGHT:
            inputs.push_back(dist_to_target.y / normal_height);
            inputs.push_back(dist_to_target.x / normal_width);
            break;

        case move_target::DOWN:
            inputs.push_back(-dist_to_target.x / normal_width);
            inputs.push_back(dist_to_target.y / normal_height);
            break;

        default:
            throw std::exception("Something strange");
            break;
        }
        return _agent_eating.get_result(inputs);
    }

    std::vector<float> Snake::_create_distance_to_obstacle(long height, long width, cube tmp) const {
        std::vector<float> test { _cubes[0].get_coords().x, (height - _cubes[0].get_coords().x)
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
        
        test[0] = test[0]/size > 1.0f ? 0.0f : 1.0f;
        test[1] = test[1]/size > 1.0f ? 0.0f : 1.0f;
        test[2] = test[2]/size > 1.0f ? 0.0f : 1.0f;
        test[3] = test[3]/size > 1.0f ? 0.0f : 1.0f;
        return test;
    }

    cube::cube(sf::Vector2f coords, move_target target)
        : _coords(coords) {}

    move_target cube::move(move_target target_next) {
        switch (target_next) {
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