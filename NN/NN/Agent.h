#pragma once
#include "NeuralNetwork.h" 

namespace genetic {
    class Agent {
    public:
        Agent(std::string path);

        Agent(long number_inputs, std::vector<long> topology, long number_outputs);

        Agent operator*(const Agent& agent) const;

        virtual bool operator<(const Agent& agent) const;

        bool operator==(const Agent& agent) const;

        void add_score(long score);

        void set_score(long score);

        std::vector<float> get_result(std::vector<float> inputs);

        long get_score() const;

        void save(std::string path) const;
    protected:
        long _score;

    private:
        base_network::NN _neural_network;
    };
}