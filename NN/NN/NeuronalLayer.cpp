#include "NeuronalLayer.h"
#include "Randomizer.h"
#include <stdexcept>

namespace base_network {
    NeuronalLayer::NeuronalLayer(long number_neuron, long number_inputs)
        : _number_neuron(number_neuron)
        , _number_inputs(number_inputs)
        , _weights(number_neuron, std::vector<float>(number_inputs, 0.0)) {}

    void NeuronalLayer::random_put_weights() {
        for (auto& neuron : _weights) {
            for (auto& weight : neuron) {
                weight = math::Randomizer::get_float_dist_negative_one_to_positive_one();
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
