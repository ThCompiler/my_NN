#include "GeneticAlgorithm.h"
#include "Genotype.h"
#include "Serialize/stream_writer.h"
#include "Serialize/stream_read.h"
#include "Randomizer.h"

namespace genetic {
    Genotype::Genotype()
        : _genome() {}

    Genotype::Genotype(std::vector<float> genome) {
        _genome = genome;
    }

    void Genotype::create_random_genotype(long number_gen)
    {
        _genome = std::vector<float>(number_gen, 0.0);

        for (auto& gen : _genome) {
            gen = math::Randomizer::get_float_dist_negative_one_to_positive_one();
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
            if (math::Randomizer::get_float_dist_zero_to_one() < GeneticAlgorithm::chance_got_new_gen()) {
                child._genome[i] = parent._genome[i];
            }
        }

        if (math::Randomizer::get_float_dist_zero_to_one() < GeneticAlgorithm::chance_agent_mutation()) {
            for (auto& gen : child._genome) {
                if (math::Randomizer::get_float_dist_zero_to_one() < GeneticAlgorithm::chance_gen_mutation()) {
                    gen = math::Randomizer::get_float_dist_zero_to_one()
                            * (GeneticAlgorithm::mutation_amount() * 2) - GeneticAlgorithm::mutation_amount();
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