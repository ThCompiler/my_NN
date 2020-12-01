#include "Evolution.h"
#include "GeneticAlgorithm.h"
#include <random>
#include <chrono>
#include <iostream>

namespace genetic {
    Evolution::Evolution()
        : _window(sf::VideoMode(game::window_size, game::window_size), "SFML works!")
        , _feeds(_generate_feed(game::window_size)) {

        _agents.resize(size_of_population, Agent(game::Snake::number_of_input_neuron, { 2 }, 3));
        for (int i = 0; i < size_of_population; ++i) {
            _agents[i] = Agent(game::Snake::number_of_input_neuron, { 2 }, 3);
            _games.push_back(game::Game(game::Snake(_agents[i]), _feeds));
        }
    }

    Evolution::Evolution(std::string path1, std::string path2) 
    : _window(sf::VideoMode(game::window_size, game::window_size), "SFML works!")
        , _feeds(_generate_feed(game::window_size))
    {
        _agents.push_back(Agent(path1));
        _agents.push_back(Agent(path2));
        _agents = GeneticAlgorithm::recombinate(_agents, size_of_population);

        for (int i = 0; i < size_of_population; ++i) {
            _games.push_back(game::Game(game::Snake(_agents[i]), _feeds));
        }
    }

    void Evolution::evolution() {
        _number_of_generation = 0;
        int dec = 1;

        while (do_evolution) {
            _simulate_game();
            GeneticAlgorithm::update(_agents);
            _number_of_generation++;

            if (_number_of_generation > 100 * dec) {
                _agents[0].save("nn1-3e.bin");
                _agents[0].save("nn2-3e.bin");
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
        
        game::Game loaded = game::Game(game::Snake(load, sf::Color::Red), _generate_feed(game::window_size));
        _window.setFramerateLimit(30);
        while (!loaded.end_game()) {
            _window.clear();
            loaded.move();
            _check_save();
            loaded.draw(_window, true);
            _window.display();
        }
        while (true) {
            _check_save();
        }   
    }

    Evolution::~Evolution() {
        _window.close();
    }

    void Evolution::_simulate_game() {
        _window.setFramerateLimit(30);
        _games[0].set_color(sf::Color::Red);

        long max_id = 0;
        bool continu = true;

        while (continu && _window.isOpen()) {

            continu = false;
            for (auto& game : _games) {
                game.move();

                continu = game.end_game() ? continu : 1;
            }

            if (!_check_save()) break;

            for (int i = 0; i < _games.size(); ++i) {
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
            _games[max_id].draw(_window, true);

            _window.display();
        }
    }

    void Evolution::_restart() {
        
        _games.clear();
        for (auto& agent : _agents) {
            agent.set_score(0);
        }

        _feeds = _generate_feed(game::window_size);

        for (int i = 0; i < size_of_population; ++i) {
            _games.push_back(game::Game(game::Snake(_agents[i]), _feeds));
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

    std::vector<sf::Vector2f> Evolution::_generate_feed(long size) const {
        std::vector<sf::Vector2f> feeds;

        std::random_device rd;

        std::mt19937::result_type seed = rd() ^ (
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
                ).count() +
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()
                ).count());

        std::mt19937 generate(seed);

        std::uniform_int_distribution<long> dist(0, (float)size / game::size - 1.0f);
        sf::Vector2f new_coords;

        for (int i = 0; i < game::number_feed; ++i) {
            bool contin = true;
            do {
                contin = false;
                new_coords = { dist(generate) * game::size + game::size / 2.0f
                    , dist(generate) * game::size + game::size / 2.0f };

                for (const auto& feed : feeds) {
                    if (new_coords == feed) {
                        contin = true; 
                        break;
                    }
                }

                if (new_coords == sf::Vector2f(95, 95)) {
                    contin = true;
                }
            } while (contin);

            feeds.push_back(new_coords);
        }

        return feeds;
    }

    bool Evolution::do_evolution = true;
}
