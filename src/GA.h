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
        std::vector<Jedinka> populacija;

    public:
        // selekcija

        // krizanje

        // mutacija

};
