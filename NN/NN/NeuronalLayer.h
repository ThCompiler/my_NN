#pragma once
#include <vector>
#include <functional>

namespace base_network {
    class NeuronalLayer {
    public:
        NeuronalLayer() = delete;

        NeuronalLayer(long number_neuron, long number_inputs);

        void random_put_weights(float minVal, float maxVal);

        std::vector<float> get_outputs(const std::vector<float>& inputs) const;

        void set_activate_function(std::function<float(float a)> activate_function);

        void set_weight_from_gen(const std::vector<float>::const_iterator& input_begin
                                            , const std::vector<float>::const_iterator& input_end);
    private:
        long _number_neuron;
        long _number_inputs;

        std::vector<std::vector<float>> _weights;

        std::function<float(float a)> _activate_function = [](float a) { return (exp(2.0f * a) - 1.0f) / (1.0f + exp(2.0f * a)); };
    };
}