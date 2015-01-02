#include "GA.h"
#include "Data.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

static int Rand(int lo, int hi) {
    return (rand() % (hi - lo + 1)) + lo;
}

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

GA::GA(char* _filename, int _population_size, int _mutation_prob,
        int _elimination_size, int _iterations) {
    strcpy(filename, _filename);
    population_size = _population_size;
    mutation_prob = _mutation_prob;
    elimination_size = _elimination_size;
    iterations = _iterations;
}

int GA::run() {
    int num_user, num_warehouse;
    int x, y;
    int warehouse_capacity, vehicle_capacity, user_capacity;
    int warehouse_cost, vehicle_cost;

    vector<Warehouse> w;
    vector<User> u;

    printf("Reading %s file...", filename);

    // procitaj ulaznu datoteku
    FILE *f = fopen(filename, "r");
    fscanf(f, "%d", &num_user);
    fscanf(f, "%d", &num_warehouse);

    w.resize(num_warehouse);
    u.resize(num_user);

    for (int i = 0; i < num_warehouse; ++i) {
        fscanf(f, "%d%d", &x, &y);
        w[i] = Warehouse(i, x, y);
    }

    for (int i = 0; i < num_user; ++i) {
        fscanf(f, "%d%d", &x, &y);
        u[i] = User(i, x, y);
    }

    fscanf(f, "%d", &vehicle_capacity);

    for (int i = 0; i < num_warehouse; ++i) {
        fscanf(f, "%d", &warehouse_capacity);
        w[i].setCapacity(warehouse_capacity);
    }

    for (int i = 0; i < num_user; ++i) {
        fscanf(f, "%d", &user_capacity);
        u[i].setCapacity(user_capacity);
    }

    for (int i = 0; i < num_warehouse; ++i) {
        fscanf(f, "%d", &warehouse_cost);
        w[i].setCost(warehouse_cost);
    }

    fscanf(f, "%d", &vehicle_cost);
    fclose(f);
    printf(" done\n");

    srand(time(NULL));

    printf("Generating starting populations...");
    // generiraj pocetnu populaciju
    printf(" done\n");

    printf("Starting GA.\n");
    for (int ITER = 0; ITER < this->iterations; ++ITER) {

        for (int ELIM = 0; ELIM < this->elimination_size; ++ELIM) {
            selekcija();
        }
    }

    printf("Exiting GA.\n");
    return 0;
}

void GA::selekcija(void) {
    // troturniska selekcija
    return;
}

GA::Jedinka GA::krizanje(const Jedinka &a, const Jedinka &b) {
    Jedinka child;
    /*
     * (random odabrani X, Y), za jedinke A i B, od A uzmi [0, X] i [Y, kraja]
     * a od B[X, Y] s time da svi korisnici kaj su vec u B[X, Y] ne uzimamo iz
     * A[0, X] U A[Y, kraja] (ili obratno)
     */

    if (Rand(0, 100) < this->mutation_prob) {
        return mutacija(child);
    } else {
        return child;
    }
}

GA::Jedinka GA::mutacija(const Jedinka &a) {
    Jedinka mut;
    /*
     * (random odabrani X, Y, Z) stavi korsnika Z iz skladista X u skladiste Y
     * (ako time ne narusavamo dostupan kapacitet)
     */
    return mut;
}

void GA::Jedinka::updateFitness(void) {
    /*
     * auti = bin packing racunamo First Fit Decreasing, more se napisati u O(n
     * log n) ali kak nam bude TSP O(n^2) moremo i to O(n^2)
     *
     * put = za tsp racunamo Bitonic Tour, O(n^2)
     *
     * fitness = sva skladista koja imaju korisnike * njihova cijena
     * + auti * cijena + put
     */
    return;
}
