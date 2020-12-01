#pragma once
#include <vector>
#include <random>
#include <fstream>

namespace genetic {
    class Genotype {
    public:
        Genotype();

        Genotype(std::vector<float> genome);

        void create_random_genotype(long number_gen);

        const std::vector<float>& get_genom() const;

        bool is_zero() const;

        Genotype operator*(const Genotype& parent) const;
        
        bool operator==(const Genotype& genotype) const;

        void save(std::ofstream& file) const;

        void load(std::ifstream& file);
    private:
        std::vector <float> _genome;
    };
}