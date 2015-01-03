#include "GA.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "FirstFit.h"
#include "Bitonic.h"

using namespace std;

static int Rand(int lo, int hi) {
    return (rand() % (hi - lo + 1)) + lo;
}

int GA::Jedinka::getFitness(void) const {
    return fitness;
}

vector<User> GA::Jedinka::getWarehouseUsers(const int &warehouse) const {
    return skladiste[warehouse];
}

void GA::Jedinka::setFitness(const int &_fitness) {
    fitness = _fitness;
}

void GA::Jedinka::setWarehouseUser(const int &warehouse, const User &user) {
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
    int flag[NUM_USER];

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
    this->vehicle_capacity = vehicle_capacity;

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

    this->users = u;
    this->warehouses = w;

    fscanf(f, "%d", &vehicle_cost);
    this->vehicle_cost = vehicle_cost;

    fclose(f);
    printf(" done\n");

    srand(time(NULL));

    printf("Generating starting populations...");

    for (int i = 0; i < this->population_size; ++i) {
        Jedinka curr;

        // TODO

        curr.updateFitness(this->warehouses, this->vehicle_capacity, this->vehicle_cost);
        this->population.push_back(curr);
    }

    printf(" done\n");

    printf("Starting GA.\n");
    for (int ITER = 0; ITER < this->iterations; ++ITER) {

        for (int ELIM = 0; ELIM < this->elimination_size; ++ELIM) {
            selekcija();
        }
    }

    printf("Extracting best unit from population.\n");
    Jedinka best = this->population[0];
    for (int i = 1; i < population_size; ++i) {
        if (this->population[i].getFitness() < best.getFitness()) {
            best = this->population[i];
        }
    }

    printf("Solution found: %d\n", best.getFitness());

    printf("Exiting GA.\n");
    return 0;
}

void GA::selekcija(void) {
    // troturnirska selekcija
    return;
}

GA::Jedinka GA::krizanje(const Jedinka &a, const Jedinka &b) {
    Jedinka child;
    /*
     * (random odabrani X, Y), za jedinke A i B, od A uzmi [0, X] i [Y, kraja]
     * a od B[X, Y] s time da svi korisnici kaj su vec u B[X, Y] ne uzimamo iz
     * A[0, X] U A[Y, kraja] (ili obratno)
     */
    int x = Rand(1, NUM_WAREHOUSE) - 1;
    int y = Rand(1, NUM_WAREHOUSE) - 1;
    if (y < x) swap(x, y);

    for (int i = 0; i < NUM_WAREHOUSE; ++i) {
        vector<User> skladiste;

        if (i <= x || i >= y) {
            skladiste = a.getWarehouseUsers(i);
        } else {
            skladiste = b.getWarehouseUsers(i);
        }

        for (int j = 0; j < skladiste.size(); ++j) {
            child.setWarehouseUser(i, skladiste[j]);
        }
    }

    // ako nije dobra -> vrati bolju ili pokusaj opet
    bool fits = true;
    for (int i = 0; i < NUM_WAREHOUSE; ++i) {
        vector<User> skladiste = child.getWarehouseUsers(i);
        int need = 0;
        for (int j = 0; j < skladiste.size(); ++j) {
            need += skladiste[j].getCapacity();
        }

        if (need > this->warehouses[i].getCapacity()) {
            fits = false;
            break;
        }
    }

    if (fits == false) {
        if (Rand(0, 1) == 0) {
            if (a.getFitness() < b.getFitness()) {
                child = a;
            } else {
                child = b;
            }
        } else {
            child = krizanje(a, b);
        }
    }

    // mutacija
    if (Rand(0, 100) < this->mutation_prob) {
        return mutacija(child);
    } else {
        child.updateFitness(this->warehouses, this->vehicle_capacity, this->vehicle_cost);
        return child;
    }
}

GA::Jedinka GA::mutacija(const Jedinka &a) {
    Jedinka mut;
    /*
     * (random odabrani X, Y, Z) stavi korsnika Z iz skladista X u skladiste Y
     * (ako time ne narusavamo dostupan kapacitet)
     */

    int x = Rand(1, NUM_WAREHOUSE) - 1;
    vector<User> skladiste_x = a.getWarehouseUsers(x);

    int z = Rand(0, skladiste_x.size() - 1);

    int y;
    while (1) {
        y = Rand(1, NUM_WAREHOUSE) - 1;
        vector<User> skladiste_y = a.getWarehouseUsers(y);

        int need = 0;
        for (int j = 0; j < skladiste_y.size(); ++j) {
            need += skladiste_y[j].getCapacity();
        }

        need += skladiste_x[z].getCapacity();

        if (need <= this->warehouses[y].getCapacity()) {
            break;
        }
    }

    for (int i = 0; i < NUM_WAREHOUSE; ++i) {
        vector<User> skladiste = a.getWarehouseUsers(i);

        for (int j = 0; j < skladiste.size(); ++j) {
            if (i != x || j != z) {
                mut.setWarehouseUser(i, skladiste[j]);
            }
        }
    }

    mut.setWarehouseUser(y, skladiste_x[z]);

    mut.updateFitness(this->warehouses, this->vehicle_capacity, this->vehicle_cost);
    return mut;
}

void GA::Jedinka::updateFitness(const vector<Warehouse> &w, int capacity, int cost) {
    /*
     * auti = bin packing racunamo First Fit Decreasing, more se napisati u O(n
     * log n) ali kak nam bude TSP O(n^2) moremo i to O(n^2)
     *
     * put = za tsp racunamo Bitonic Tour, O(n^2)
     *
     * fitness = sva skladista koja imaju korisnike * njihova cijena
     * + auti * cijena + put
     */
    int res = 0;
    for (int i = 0; i < NUM_WAREHOUSE; ++i) {
        if (skladiste[i].size() > 0) {
            vector<vector<User> > tours = first_fit(skladiste[i], capacity);

            for (int j = 0; j < tours.size(); ++j) {
                res += cost;
                res += bitonic_tour(tours[j], w[i]);
            }
            res += w[i].getCost();
        }
    }

    this->fitness = res;
    return;
}
