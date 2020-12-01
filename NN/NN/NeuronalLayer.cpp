#include "NeuronalLayer.h"
#include <random>
#include <stdexcept>
#include <chrono>

namespace base_network {
    NeuronalLayer::NeuronalLayer(long number_neuron, long number_inputs)
        : _number_neuron(number_neuron)
        , _number_inputs(number_inputs)
        , _weights(number_neuron, std::vector<float>(number_inputs, 0.0)) {}

    void NeuronalLayer::random_put_weights(float min_val, float max_val) {
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

        std::mt19937 gen(seed);

        std::uniform_real_distribution<float> dist(min_val, max_val);

        for (auto& neuron : _weights) {
            for (auto& weight : neuron) {
                weight = dist(gen);
            }
        }
    }

    std::vector<float> NeuronalLayer::get_outputs(const std::vector<float>& inputs) const {
        if (_number_inputs != inputs.size()) {
            throw std::invalid_argument("Number of wait inputs not equal number of got inputs");
        }

        std::vector<float> outputs(_number_neuron, 0.0);

        int k = 0;
        for (const auto& neuron : _weights) {
            for (int i = 0; i < _number_inputs; ++i) {
                outputs[k] += inputs[i] * neuron[i];
            }
            outputs[k] += 0.2f;
            outputs[k] = _activate_function(outputs[k]);
            ++k;
        }

        return outputs;
    }

    void NeuronalLayer::set_activate_function(std::function<float(float a)> activate_function) {
        _activate_function = activate_function;
    }

    void NeuronalLayer::set_weight_from_gen(const std::vector<float>::const_iterator& input_begin
                                        , const std::vector<float>::const_iterator& input_end) {
        std::vector<float>::const_iterator iter = input_begin;

        if (input_end - iter != _number_inputs * _number_neuron) {
            throw std::invalid_argument("Gotted genotype can't be use in this neuronal layer");
        }

        for (auto& neuron : _weights) {
            for (auto& weight : neuron) {
                weight = *(iter++);
            }
        }
    }
}
