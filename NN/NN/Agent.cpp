#include "Agent.h"

namespace genetic {
    Agent::Agent(std::string path) 
        : _neural_network(path)
        , _score(0) {}

    Agent::Agent(long number_inputs, std::vector<long> topology, long number_outputs)
        : _neural_network(number_inputs, topology, number_outputs)
        , _score(0) {}

    Agent Agent::operator*(const Agent& agent) const {
        Agent res = Agent(*this);

        res._neural_network = _neural_network * agent._neural_network;
        return res;
    }
    bool Agent::operator<(const Agent& agent) const
    {
        return _score > agent._score;
    }

    bool Agent::operator==(const Agent& agent) const
    {
        return _neural_network == agent._neural_network && _score == agent._score;
    }

    void Agent::add_score(long score) {
        _score += score;
    }

    void Agent::set_score(long score) {
        _score = score;
    }
    std::vector<float> Agent::get_result(std::vector<float> inputs)
    {
        return _neural_network.get_outputs(inputs);
    }

    long Agent::get_score() const
    {
        return _score;
    }

    void Agent::save(std::string path) const {
        _neural_network.save(path);
    }
}
