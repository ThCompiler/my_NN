#include "Evolution.h"
#include "GeneticAlgorithm.h"
#include "Randomizer.h"
#include <iostream>

namespace genetic {
    Evolution::Evolution()
        : _window(sf::VideoMode(game::window_size, game::window_size), "SFML works!")
        , _number_of_generation(0) {

        long lenght_snake = math::Randomizer::get_long_rand(10, 40);
        _agents.resize(size_of_population, Agent(game::Move_snake::number_of_input_neuron, { 3 }, 3));

        
        
        for (int i = 0; i < size_of_population; ++i) {
            _agents[i] = Agent(game::Move_snake::number_of_input_neuron, { 3 }, 3);

            game::Move_snake tmp { _agents[i] };
            _games.push_back(game::Game(tmp));
        }
    }

    Evolution::Evolution(std::string path1, std::string path2) 
    : _window(sf::VideoMode(game::window_size, game::window_size), "SFML works!")  {
        _agents.push_back(Agent(path1));
        _agents.push_back(Agent(path2));
        _agents = GeneticAlgorithm::recombinate(_agents, size_of_population);

        for (int i = 0; i < size_of_population; ++i) {
            _games.push_back(game::Game(game::Move_snake(_agents[i])));
        }
    }

    void Evolution::evolution() {
        _number_of_generation = 0;
        long dec = 1;

        

        while (do_evolution) {

            std::vector<std::vector<float>>inputs;
            std::vector<std::vector<long>> answers;
            std::vector<long> targets;

            for (int i = 0; i < 1000; ++i) {
                inputs.push_back(_generate_inputs());
                targets.push_back(math::Randomizer::get_long_rand(0, 2));
                answers.push_back(_generate_answers(targets.back(), inputs.back(), targets.back()));
            }
            _simulate_game(inputs, answers, targets);
            //_restart(true);

            GeneticAlgorithm::update(_agents);
            _number_of_generation++;

            if (_number_of_generation > 100L * dec) {
                _agents[0].save("nn1-3.bin");
                _agents[0].save("nn2-3.bin");
                dec++;
            }

            std::cout << _agents[0].get_score() << " " << _agents[1].get_score() << " in " 
                                                << _number_of_generation << " generation\n";
            _agents = GeneticAlgorithm::recombinate(GeneticAlgorithm::select(_agents), size_of_population);
            _restart();
        }
    }

    void Evolution::save() const {
        _agents[0].save("nn9.bin");
    }

    void Evolution::load(std::string path) {
        Agent load = Agent(path);
        
        game::Game loaded = game::Game(game::Move_snake(load, sf::Color::Red));
        _window.setFramerateLimit(30);
        while (!loaded.end_game()) {
            _window.clear();
            //loaded.move();
            _check_save();
            loaded.draw(_window);
            _window.display();
        }
        while (true) {
            _check_save();
        }   
    }

    Evolution::~Evolution() {
        _window.close();
    }

    std::vector<float> Evolution::_generate_inputs() const {
        std::vector<float> inputs { 0, 0, 0 };
        long number = math::Randomizer::get_long_rand(1, 2);
        long last = -1;
        for (long i = 1; i <= number; ++i) {
            long next = 0;
            do {
                next = math::Randomizer::get_long_rand(0, 2);
            } while (next == last);

            inputs[next] = 1.0f;
        }
        return inputs;
    }

    std::vector<long> Evolution::_generate_answers(long target, const std::vector<float>& inputs, int targets) const {
        std::vector<long> answer;

       if (inputs[targets] < 0.5f) {
           switch (targets) {
           case 0:
               return { target == (long)game::move_target::UP ? (long)game::move_target::LEFT :
                   (target == (long)game::move_target::LEFT ? (long)game::move_target::DOWN :
                       (target == (long)game::move_target::DOWN ? (long)game::move_target::RIGHT :
                           (long)game::move_target::UP)) };
               break;
           case 1:
               return{ target };
               break;
           case 2:
               return { target == (long)game::move_target::UP ? (long)game::move_target::RIGHT :
                   (target == (long)game::move_target::LEFT ? (long)game::move_target::UP :
                       (target == (long)game::move_target::DOWN ? (long)game::move_target::LEFT :
                           (long)game::move_target::DOWN)) };
               break;
           default:
               return { target };
               break;
           }
        }

        for (int i = 0; i < inputs.size(); ++i) {
            if (inputs[i] > 0.5f) continue;
            
            switch (i) {
            case 0:
                answer.push_back(target == (long)game::move_target::UP ? (long)game::move_target::LEFT :
                    (target == (long)game::move_target::LEFT ? (long)game::move_target::DOWN :
                        (target == (long)game::move_target::DOWN ? (long)game::move_target::RIGHT :
                                                                (long)game::move_target::UP)));
                break;
            case 1:
                answer.push_back(target);
                break;
            case 2:
                answer.push_back(target == (long)game::move_target::UP ? (long)game::move_target::RIGHT :
                    (target == (long)game::move_target::LEFT ? (long)game::move_target::UP :
                        (target == (long)game::move_target::DOWN ? (long)game::move_target::LEFT :
                                                            (long)game::move_target::DOWN)));
                break;
            default:
                answer.push_back(target);
                break;
            }
        }
        return answer;
    }

    void Evolution::_simulate_game(std::vector<std::vector<float>>inputs, std::vector<std::vector<long>> answers, std::vector<long> targets) {
        _window.setFramerateLimit(30);
        _games[0].set_color(sf::Color::Red);

        long max_id = 0;
        long continu = 0;
        long tmp = math::Randomizer::get_long_rand(0, 3);
        for (auto& game : _games) {
            game.get_snake().set_target((game::move_target)tmp);
        }

        while (continu < 1000 && _window.isOpen()) {
            
            //continu = false;
            for (auto& game : _games) {
                answers[continu] = _generate_answers((long)game.get_snake().get_target(), inputs[continu], targets[continu]);
                game.move(inputs[continu], answers[continu], targets[continu]);
            }

            if (!_check_save()) break;

            /*for (int i = 0; i < _games.size(); ++i) {
                if (_games[i].get_score() > _games[max_id].get_score() 
                    || _games[max_id].end_game()) {
                    _games[max_id].set_color();
                    max_id = i;
                    _games[i].set_color(sf::Color::Red);
                }
            }

            _window.clear();
            for (int i = 0; i < _games.size(); ++i) {
                if (i != max_id) {
                    _games[i].draw(_window);
                }
            }
            _games[max_id].draw(_window);

            _window.display();*/
            ++continu;
        }
    }

    void Evolution::_restart(bool save_score) {
        
        _games.clear();
        if (!save_score) {
            for (auto& agent : _agents) {
                agent.set_score(0);
            }
        }

        for (int i = 0; i < size_of_population; ++i) {
            game::Move_snake tmp { _agents[i] };
            _games.push_back(game::Game(tmp));
        }
    }

    bool Evolution::_check_save() {
        sf::Event event;

        while (_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                save();
                do_evolution = false;
                return false;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::S) {
                    save();
                    do_evolution = false;
                    return false;
                }
            }
        }

        return true;
    }

    bool Evolution::do_evolution = true;
}
