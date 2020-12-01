#pragma once
#include "NeuronalLayer.h"
#include "Genotype.h"

namespace base_network {
    class NN {
    public:
        NN(std::string path);

        NN(long number_inputs, std::vector<long> topology, long number_outputs
                                    , genetic::Genotype genotype = genetic::Genotype());

        std::vector<float> get_outputs(const std::vector<float>& inputs) const;

        genetic::Genotype get_genotype() const; 

        NN operator*(const NN& network) const;

        static long calculate_number_weigth(long number_inputs, std::vector<long> topology
                                                    , long number_outputs);

        void operator=(const NN& from);

        void set_activate_function(std::function<float(float a)> activate_function);

        bool operator==(const NN& network) const;

        void save(std::string path) const;
    private:
        long _number_inputs;
        long _number_outputs;
        std::vector<long> _topology;

        genetic::Genotype _genotype;

        std::vector<NeuronalLayer> _layers;

        void _set_genotype(const genetic::Genotype& genotype);
    };
}