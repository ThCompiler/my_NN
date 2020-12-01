#include "GeneticAlgorithm.h"
#include <stdexcept>
#include <random>
#include <chrono>

namespace genetic {
    float GeneticAlgorithm::_chance_gen_mutation = 0.3f;
    float GeneticAlgorithm::_mutation_amount = 1.0f;
    float GeneticAlgorithm::_chance_got_new_gen = 0.6f;
    float GeneticAlgorithm::_chance_agent_mutation = 0.9f;

    std::function<std::vector<Agent>(const std::vector<Agent>&, long)> 
                    GeneticAlgorithm::recombinate = GeneticAlgorithm::_defualt_recombinate;
    std::function<std::vector<Agent>(const std::vector<Agent>&)> 
                    GeneticAlgorithm::select = GeneticAlgorithm::_defualt_select;

    float GeneticAlgorithm::chance_gen_mutation() {
        return _chance_gen_mutation;
    }

    float GeneticAlgorithm::mutation_amount() {
        return _mutation_amount;
    }

    float GeneticAlgorithm::chance_got_new_gen() {
        return _chance_got_new_gen;
    }

    float GeneticAlgorithm::chance_agent_mutation() {
        return _chance_agent_mutation;
    }

    void GeneticAlgorithm::update(std::vector<Agent>& agents) {
        std::sort(agents.begin(), agents.end());
    }

    std::vector<Agent> GeneticAlgorithm::_defualt_recombinate(const std::vector<Agent>& agents, long count) {
        if (agents.size() < 2) {
            throw std::invalid_argument("For recombination needed two or more agents");
        }

        std::random_device rd;

        std::mt19937::result_type seed = rd() ^ (
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
                ).count());
        std::mt19937 generate(seed);

        std::vector<Agent> new_population = agents;
        while (new_population.size() < count) {
            std::shuffle(new_population.begin(), new_population.end(), generate);
            new_population.push_back(new_population[0] * new_population[1]);
        }

        return new_population;
    }

    std::vector<Agent> GeneticAlgorithm::_defualt_select(const std::vector<Agent>& agents) {
        return { agents[0], agents[1] };
    }

    std::vector<Agent> GeneticAlgorithm::_initialize(long count) {
        return std::vector<Agent>();
    }
}