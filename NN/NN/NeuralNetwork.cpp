#include "NeuralNetwork.h"
#include "Serialize/stream_writer.h"
#include <stdexcept>
#include <fstream>
#include "Serialize/stream_read.h"

namespace base_network {
    NN::NN(std::string path) {
        std::ifstream in(path, std::ios::binary | std::ios::in);

        binary::read(in, _number_inputs);
        binary::read(in, _topology);
        binary::read(in, _number_outputs);
        _genotype.load(in);

        if (_topology.size() == 0) {
            _layers.push_back(NeuronalLayer(_number_outputs, _number_inputs));
            _set_genotype(_genotype);
            return;
        }

        _layers.push_back(NeuronalLayer(_topology[0], _number_inputs));
        for (int i = 1; i < _topology.size(); ++i) {
            _layers.push_back(NeuronalLayer(_topology[i], _topology[i - 1]));
        }
        _layers.push_back(NeuronalLayer(_number_outputs, _topology.back()));
        _set_genotype(_genotype);
    }

    NN::NN(long number_inputs, std::vector<long> topology
                                            , long number_outputs, genetic::Genotype genotype)
            : _number_inputs(number_inputs)
            , _topology(topology)
            , _number_outputs(number_outputs) {
        long number_weight = calculate_number_weigth(number_inputs, topology, number_outputs);

        if (genotype.is_zero()) {
            genotype.create_random_genotype(number_weight);
        }

        if (genotype.get_genom().size() != number_weight) {
            throw std::invalid_argument("Number of gens im genotype match the number of weight in nerual network");
        }

        if (_topology.size() == 0) {
            _layers.push_back(NeuronalLayer(_number_outputs, _number_inputs));
            _set_genotype(genotype);
            return;
        }

        _layers.push_back(NeuronalLayer(_topology[0], _number_inputs));
        for (int i = 1; i < _topology.size(); ++i) {
            _layers.push_back(NeuronalLayer(_topology[i], _topology[i - 1]));
        }
        _layers.push_back(NeuronalLayer(_number_outputs, _topology.back()));

        _set_genotype(genotype);
    }

    std::vector<float> NN::get_outputs(const std::vector<float>& inputs) const {
        std::vector<float> outputs = inputs;

        for (const auto& layer : _layers) {
            outputs = layer.get_outputs(outputs);
        }

        return outputs;
    }

    genetic::Genotype NN::get_genotype() const {
        return _genotype;
    }

    NN NN::operator*(const NN& network) const {
        NN res = NN(*this);
        res._set_genotype(_genotype * network._genotype);

        return res;
    }

    long NN::calculate_number_weigth(long number_inputs, std::vector<long> topology, long number_outputs) {
        if (topology.size() == 0) {
            return number_inputs * number_outputs;
        }

        long sum = number_inputs * topology[0] + number_outputs * topology.back();
        for (int i = 1; i < topology.size(); ++i) {
            sum += topology[i] * topology[i - 1];
        }
        return sum;
    }

    void NN::operator=(const NN& from) {
        _layers.clear();
        
        _layers = from._layers;
        _genotype = from._genotype;

        _number_inputs      = from._number_inputs;
        _topology           = from._topology;
        _number_outputs     = from._number_outputs;
    }

    void NN::set_activate_function(std::function<float(float a)> activate_function) {
        for (auto layer : _layers) {
            layer.set_activate_function(activate_function);
        }
    }

    bool NN::operator==(const NN& network) const {
        return  _number_inputs      == network._number_inputs &&
                _topology           == network._topology &&
                _number_outputs     == network._number_outputs &&
                _genotype           == network._genotype;
    }

    void NN::save(std::string path) const {
        std::ofstream out(path, std::ios::binary | std::ios::out);

        binary::write(out, _number_inputs);
        binary::write(out, _topology);
        binary::write(out, _number_outputs);
        _genotype.save(out);
        out.close();
    }

    void NN::_set_genotype(const genetic::Genotype& genotype) {
        _genotype = genotype;

        if (_genotype.get_genom().size() != calculate_number_weigth(_number_inputs, _topology
                                                                        , _number_outputs)) {
            throw std::invalid_argument("Number of gens im genotype match the number of weight in nerual network");
        }
        if (_topology.size() == 0) {
            _layers[0].set_weight_from_gen(_genotype.get_genom().begin(),
                (_genotype.get_genom().begin() + _number_inputs * _number_outputs));
            return;
        }

        auto iter_begin = _genotype.get_genom().begin();
        _layers[0].set_weight_from_gen(_genotype.get_genom().begin(),
            (_genotype.get_genom().begin() + _number_inputs * _topology[0]));

        iter_begin += _number_inputs * _topology[0];

        for (int i = 1; i < _topology.size(); ++i) {
            _layers[i].set_weight_from_gen(iter_begin,
                (iter_begin + _topology[i] * _topology[i - 1]));
            iter_begin += _topology[i] * _topology[i - 1];
        }

        _layers[_topology.size()].set_weight_from_gen(iter_begin,
            (iter_begin + _number_outputs * _topology.back()));
    }
}