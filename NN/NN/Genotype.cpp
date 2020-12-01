#include <random>
#include <chrono>
#include "GeneticAlgorithm.h"
#include "Genotype.h"
#include "Serialize/stream_writer.h"
#include "Serialize/stream_read.h"

namespace genetic {
    std::mt19937 Genotype::_generate = std::mt19937((std::mt19937::result_type)
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
            ).count());

    std::uniform_real_distribution<float> Genotype::_dist = std::uniform_real_distribution<float>(0.0, 1.0);

    Genotype::Genotype()
        : _genome() {}

    Genotype::Genotype(std::vector<float> genome) {
        _genome = genome;
    }

    void Genotype::create_random_genotype(long number_gen, float min_val, float max_val)
    {
        _genome = std::vector<float>(number_gen, 0.0);

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

        std::uniform_real_distribution<float> dist(min_val, max_val);

        for (auto& gen : _genome) {
            gen = dist(generate);
        }
    }

    const std::vector<float>& Genotype::get_genom() const
    {
        return _genome;
    }

    bool Genotype::is_zero() const
    {
        return _genome.size() == 0;
    }

    Genotype Genotype::operator*(const Genotype& parent) const {
        Genotype child(_genome);

        for (int i = 0; i < child._genome.size(); ++i) {
            if (_dist(_generate) < GeneticAlgorithm::chance_got_new_gen()) {
                child._genome[i] = parent._genome[i];
            }
        }

        if (_dist(_generate) < GeneticAlgorithm::chance_agent_mutation()) {
            for (auto& gen : child._genome) {
                if (_dist(_generate) < GeneticAlgorithm::chance_gen_mutation()) {
                    gen = _dist(_generate) * (GeneticAlgorithm::mutation_amount() * 2) - GeneticAlgorithm::mutation_amount();
                }
            }
        }
        return child;
    }

    bool Genotype::operator==(const Genotype& genotype) const {
        return _genome == genotype._genome;
    }

    void Genotype::save(std::ofstream& file) const {
        binary::write(file, _genome);
    }

    void Genotype::load(std::ifstream& file) {
        binary::read(file, _genome);
    }
}