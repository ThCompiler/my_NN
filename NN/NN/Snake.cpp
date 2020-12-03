#include "Snake.h"
#include "Randomizer.h"
#include <exception>

namespace game {
    const long Move_snake::number_of_input_neuron = 3;

    Move_snake::Move_snake(genetic::Agent& agent, sf::Color color)
        : _agent(agent)
        , _my_color(color)
        , _counter_of_step(0) {
        _cubes.push_back(cube(sf::Vector2f(205, 205), move_target::UP));
    }

    move_target Move_snake::move(long height, long width, std::vector<float> inputs) {
        if (!_alive) return move_target::INCORRECT;
        
        _counter_of_step++;

        //move_target next = _think(height, width);
        
        std::vector<float> outputs = _agent.get_result(inputs);

        if (outputs[3] > 0) {
            _cubes[0].move(_cubes[0].get_target());
            return _cubes[0].get_target();
        }

        long max_id = 0;

        for (long i = 1; i < 3; ++i) {
            if (outputs[i] > outputs[max_id]) {
                max_id = i;
            }
        }

        move_target last = _cubes[0].get_target();

        switch (max_id) {
        case 0:
            last = last == move_target::UP ? move_target::LEFT :
                (last == move_target::LEFT ? move_target::DOWN :
                    (last == move_target::DOWN ? move_target::RIGHT : move_target::UP));
            break;
        case 1:
            break;
        case 2:
            last = last == move_target::UP ? move_target::RIGHT :
                (last == move_target::LEFT ? move_target::UP :
                    (last == move_target::DOWN ? move_target::LEFT : move_target::DOWN));
            break;
        default:
            break;
        }
        _cubes[0].move(last);
        return last;
        /*
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

        if (_alive) {
            _agent.add_score(1);
        }*/
    }

    void game::Move_snake::kill() {
        _alive = false;
        _agent.add_score(-1);
    }

    void Move_snake::add_score() {
        _agent.add_score(1);
        
        _add_cubes = true;
    }

    sf::Vector2f Move_snake::get_coords() const
    {
        return _cubes[0].get_coords();
    }

    void Move_snake::draw(sf::RenderWindow& window) const {
        if (!_alive) return;

        for (const auto& cube : _cubes) {
            cube.draw(window, _my_color);
        }
    }

    bool Move_snake::live() const {
        return _alive;
    }

    long Move_snake::get_score() const
    {
        return _agent.get_score();
    }

    bool Move_snake::check_introsekt(sf::Vector2f coords) const {
        sf::RectangleShape snake(sf::Vector2f(size, size));
        sf::RectangleShape object(sf::Vector2f(size, size));

        snake.setPosition(_cubes[0].get_coords());
        object.setPosition(coords);

        return snake.getGlobalBounds().intersects(object.getGlobalBounds());
    }

    void Move_snake::set_color(sf::Color color) {
        _my_color = color;
    }

    long Move_snake::get_count_of_step() const {
        return _counter_of_step;
    }

    bool Move_snake::generate_snake(long lenght) {
        move_target last = _cubes[0].get_target();

        while (lenght != 0) {
            last = _check_target(_target_for_generate_next_cube(last));
            if(last == move_target::INCORRECT) {
                cube tmp = _cubes[0];
                _cubes.clear();
                _cubes.push_back(tmp);
                break;
            }
            
            switch (last)
            {
            case move_target::UP:
                _cubes.push_back(cube(sf::Vector2f(_cubes.back().get_coords().x, _cubes.back().get_coords().y + size), last));
                break;

            case move_target::LEFT:
                _cubes.push_back(cube(sf::Vector2f(_cubes.back().get_coords().x + size, _cubes.back().get_coords().y), last));
                break;

            case move_target::RIGHT:
                _cubes.push_back(cube(sf::Vector2f(_cubes.back().get_coords().x - size, _cubes.back().get_coords().y), last));
                break;

            case move_target::DOWN:
                _cubes.push_back(cube(sf::Vector2f(_cubes.back().get_coords().x, _cubes.back().get_coords().y - size), last));
                break;

            default:
                throw std::exception("Something strange");
            }
            lenght--;
        }
        return lenght != 0;
    }

    void Move_snake::set_coords(sf::Vector2f coords) {
        _cubes.clear();
        _cubes.push_back(cube(coords, move_target::UP));
    }

    std::vector<cube>& Move_snake::get_snake()
    {
        return _cubes;
    }

    void Move_snake::set_snake(std::vector<cube>& snake) {
        _cubes = snake;
    }

    void Move_snake::set_target(move_target target) {
        _cubes[0] = cube(_cubes[0].get_coords(), target);
    }

    move_target Move_snake::get_target()
    {
        return _cubes[0].get_target();
    }


    move_target Move_snake::_think(long height, long width) const {
        std::vector<float> outputs = _agent.get_result(_create_inputs(height, width));

        if (outputs[3] > 0) {
            return _cubes[0].get_target();
        }

        long max_id = 0;

        for (long i = 1; i < 3; ++i) {
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

    std::vector<float> Move_snake::_create_inputs(long height, long width) const
    {
        std::vector<float> test = _create_distance_to_obstacle(height, width);
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
        }

        return inputs;
    }

    std::vector<float> Move_snake::_create_distance_to_obstacle(long height, long width) const {
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
        
        test[0] = test[0]/size > 1.0f ? 0.0f : 1.0f;
        test[1] = test[1]/size > 1.0f ? 0.0f : 1.0f;
        test[2] = test[2]/size > 1.0f ? 0.0f : 1.0f;
        test[3] = test[3]/size > 1.0f ? 0.0f : 1.0f;
        return test;
    }

    move_target Move_snake::_target_for_generate_next_cube(move_target last) const {
        std::vector<float> rand_target = {};
        long max_id = 0;

        rand_target = { math::Randomizer::get_float_dist_zero_to_one()
                          , math::Randomizer::get_float_dist_zero_to_one()
                          , math::Randomizer::get_float_dist_zero_to_one() };

        max_id = 0;
        for (long i = 1; i < 3; ++i) {
            if (rand_target[i] > rand_target[max_id]) {
                max_id = i;
            }
        }

        switch (max_id) {
        case 0:
            return (last == move_target::UP ? move_target::LEFT :
                (last == move_target::LEFT ? move_target::DOWN :
                    (last == move_target::DOWN ? move_target::RIGHT : move_target::UP)));
        case 1:
            return last;
        case 2:
            return (last == move_target::UP ? move_target::RIGHT :
                (last == move_target::LEFT ? move_target::UP :
                    (last == move_target::DOWN ? move_target::LEFT : move_target::DOWN)));
            break;
        default:
            throw std::exception("Something strange");
        }
    }

   

    move_target Move_snake::_check_target(move_target target) const {
        std::vector<sf::Vector2f> pos =
            { sf::Vector2f(_cubes.back().get_coords().x + size, _cubes.back().get_coords().y)
            , sf::Vector2f(_cubes.back().get_coords().x - size, _cubes.back().get_coords().y)
            , sf::Vector2f(_cubes.back().get_coords().x, _cubes.back().get_coords().y + size)
            , sf::Vector2f(_cubes.back().get_coords().x, _cubes.back().get_coords().y - size) };

        std::vector<bool> empty { true, true, true, true };

        for (const auto& cube : _cubes) {
            if (cube.get_coords() == pos[(long)move_target::UP]) {
                empty[(long)move_target::UP] = false;
            }
            if (cube.get_coords() == pos[(long)move_target::LEFT]) {
                empty[(long)move_target::LEFT] = false;
            }
            if (cube.get_coords() == pos[(long)move_target::RIGHT]) {
                empty[(long)move_target::RIGHT] = false;
            }
            if (cube.get_coords() == pos[(long)move_target::DOWN]) {
                empty[(long)move_target::DOWN] = false;
            }
        }

        if (empty[(long)target]) {
            return target;
        }
        if (empty[(long)move_target::UP]) {
            return move_target::UP;
        }
        if (empty[(long)move_target::LEFT]) {
            return move_target::LEFT;
        }
        if (empty[(long)move_target::RIGHT]) {
            return move_target::RIGHT;
        }
        if (empty[(long)move_target::DOWN]) {
            return move_target::DOWN;
        }

        return move_target::INCORRECT;
    }

    cube::cube(sf::Vector2f coords, move_target target)
        : _coords(coords)
        , _move_target(target) {}

    move_target cube::move(move_target target_next) {
        /*switch (_move_target) {
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
        }*/

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