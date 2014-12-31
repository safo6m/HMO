#pragma once

#include <vector>

#include "constants.h"

class GA{
    class Jedinka {
        private:
            std::vector<int> skladiste[NUM_WAREHOUSE];
            int fitness;

        public:
            int getFitness(void) const;
            std::vector<int> getWarehouseUsers(const int &warehouse) const;

            void setFitness(const int &_fitness);
            void setWarehouseUser(const int &warehouse, const int &user);

            bool operator ==(const Jedinka &a);
    };

    private:
        std::vector<Jedinka> population;
        char filename[256];
        int population_size;
        double mutation_prob;
        int elimination_size;
        int iterations;

        // selekcija
        // mutacija
        // krizanje

    public:
        GA();

        GA(char* _filename, int _population_size, double _mutation_prob,
           int _elimination_size, int _iterations);

        int run();
};
