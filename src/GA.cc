#include "GA.h"

using namespace std;

int GA::Jedinka::getFitness(void) const {
    return fitness;
}

vector<int> GA::Jedinka::getWarehouseUsers(const int &warehouse) const {
    return skladiste[warehouse];
}

void GA::Jedinka::setFitness(const int &_fitness) {
    fitness = _fitness;
}

void GA::Jedinka::setWarehouseUser(const int &warehouse, const int &user) {
    skladiste[warehouse].push_back(user);
    // keep them sorted for comparison
    sort(skladiste[warehouse].begin(), skladiste[warehouse].end());
}

bool GA::Jedinka::operator ==(const Jedinka &a) {
    if (fitness != a.getFitness()) {
        return false;
    } else {
        for (int i = 0; i < NUM_WAREHOUSE; ++i) {
            if (skladiste[i] != a.getWarehouseUsers(i)) {
                return false;
            }
        }
    }
    return true;
}

GA::GA() {
}

GA::GA(char* _filename, int _population_size, double _mutation_prob,
        int _elimination_size, int _iterations) {
    strcpy(filename, _filename);
    population_size = _population_size;
    mutation_prob = _mutation_prob;
    elimination_size = _elimination_size;
    iterations = _iterations;
}

int GA::run() {
    return 1;
}

