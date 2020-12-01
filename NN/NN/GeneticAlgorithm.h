#pragma once
#include "Agent.h"
#include <functional>

namespace genetic {
    class GeneticAlgorithm {
    public:
        static float chance_gen_mutation();

        static float mutation_amount();

        static float chance_got_new_gen();

        static float chance_agent_mutation();

        static std::function<std::vector<Agent>(const std::vector<Agent>&, long)> recombinate;

        static std::function<std::vector<Agent>(const std::vector<Agent>&)> select;

        static std::function<std::vector<Agent>(long)> initialize;

        static void update(std::vector<Agent>& agents);

    private:
        static float _chance_gen_mutation;
        static float _mutation_amount;
        static float _chance_got_new_gen;
        static float _chance_agent_mutation;

        static std::vector<Agent> _defualt_recombinate(const std::vector<Agent>& agents, long count);

        static std::vector<Agent> _defualt_select(const std::vector<Agent>& agents);

        static std::vector<Agent> _initialize(long count);
    };
}