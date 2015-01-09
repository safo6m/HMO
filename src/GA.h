#pragma once

#include <vector>

#include "Data.h"
#include "Constants.h"

class GA{
    public: class Jedinka {
        private:
            std::vector<User> skladiste[NUM_WAREHOUSE];
            int fitness;

        public:
            int getFitness(void) const;
            std::vector<User> getWarehouseUsers(const int &warehouse) const;

            void setFitness(const int &_fitness);
            void setWarehouseUser(const int &warehouse, const User &user);

            void updateFitness(const std::vector<Warehouse> &w, int capacity, int cost);

            bool operator ==(const Jedinka &a);
    };

    private:
        std::vector<Jedinka> population;
        std::vector<User> users;
        std::vector<Warehouse> warehouses;

        char filename[256];
        int population_size;
        int mutation_prob;
        int elimination_size;
        int iterations;
        int vehicle_capacity;
        int vehicle_cost;

        // selekcija
        void selekcija(void);

        // krizanje
        Jedinka krizanje(const Jedinka &a, const Jedinka &b);

        // mutacija
        Jedinka mutacija(const Jedinka &a);

    public:
        GA();

        GA(char* _filename, int _population_size, int _mutation_prob,
           int _elimination_size, int _iterations);

        int run();
};
