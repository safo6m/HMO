#include "GA.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <set>

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

	vector<Warehouse> w;
	vector<User> u;

	fprintf(stderr, "Reading %s file...", filename);

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
	fprintf(stderr, " done\n");

	srand(time(NULL));

	fprintf(stderr, "Generating starting populations...");

	for (int i = 0; i < this->population_size; ++i) {
		Jedinka curr;

		for (int j = 0; j < num_user; ++j) {
			int warehouse, capacity;
			vector<User> skladiste;

			//pomocna varijabla
			User curr_user = this->users[j];

			int warehouse_dist[NUM_WAREHOUSE]; // udaljenost korisnika od svakog skladista
			int total_dist = 0; // ukupna udaljlenost
			for (int k = 0; k < NUM_WAREHOUSE; k++){
				warehouse_dist[k] = curr_user.getDistance(this->warehouses[k]);
				total_dist += warehouse_dist[k];
			}

			int warehouse_upper_bounds[NUM_WAREHOUSE]; //gornje granice intervala za rand broj

			while (1) {
				// izracunaj gornje granice intervala, donja granica[i] = gg[i-1]
				for (int k = 0; k < NUM_WAREHOUSE; k++){
					warehouse_upper_bounds[k] = ((total_dist - warehouse_dist[k]) * 1000) / total_dist;
					if (k > 0){
						warehouse_upper_bounds[k] += warehouse_upper_bounds[k-1];
					}
				}

				int rand_x = Rand(1, warehouse_upper_bounds[NUM_WAREHOUSE-1]) - 1;

				warehouse = 0;
				while(rand_x > warehouse_upper_bounds[warehouse]){
					warehouse++;
				}

				// warehouse = Rand(1, this->warehouses.size()) - 1;
				skladiste = curr.getWarehouseUsers(warehouse);
				capacity = this->warehouses[warehouse].getCapacity();

				for (int k = 0; k < skladiste.size(); ++k) {
					capacity -= skladiste[k].getCapacity();
				}

				if (capacity >= curr_user.getCapacity()) {
					curr.setWarehouseUser(warehouse, curr_user);
					break;
				} else {
					//eliminiraj to skladiste ako trenutni korisnik ne stane v skladiste
					warehouse_dist[warehouse] = total_dist;
				}
			}
		}

		curr.updateFitness(this->warehouses, this->vehicle_capacity, this->vehicle_cost);
		this->population.push_back(curr);
	}

	fprintf(stderr, " done\n");

	fprintf(stderr, "Starting GA.\n");
	for (int ITER = 0; ITER < this->iterations; ++ITER) {
		if (ITER % 1000 == 0) fprintf(stderr, "%d\n", ITER);
		selekcija();
	}

	fprintf(stderr, "Extracting best unit from population.\n");
	Jedinka best = this->population[0];
	for (int i = 1; i < population_size; ++i) {
		if (this->population[i].getFitness() < best.getFitness()) {
			best = this->population[i];
		}
	}

	fprintf(stderr, "Solution found: %d\n", best.getFitness());

	vector< pair<int, vector<vector<User> > > > solution;
	int cnt = 0;

	for (int i = 0; i < this->warehouses.size(); ++i) {
		vector<User> skladiste = best.getWarehouseUsers(i);
		vector<vector<User> > curr = dist_fit(skladiste, vehicle_capacity);
		solution.push_back(make_pair(i, curr));
		cnt += curr.size();
	}

	printf("%d\n\n", cnt);

	for (int i = 0; i < solution.size(); ++i) {
		int warehouse_id = solution[i].first;
		vector<vector<User> > tours = solution[i].second;

		for (int j = 0; j < tours.size(); ++j) {
			printf("%d: ", warehouse_id);
			vector<User> tour = bitonic_tour(tours[j], this->warehouses[warehouse_id]);

			int start = 0;
			while (tour[start].getID() != -1) {
				start += 1;
			}

			for (int k = 1; k < tour.size(); ++k) {
				printf(" %d", tour[(start + k) % (tour.size())].getID());
			}

			printf("\n\n");
		}
	}

	printf("\n%d\n", best.getFitness());

	fprintf(stderr, "Exiting GA.\n");
	return 0;
}

inline bool cmp(const GA::Jedinka &a, const GA::Jedinka &b) {
	return (a.getFitness() < b.getFitness());
}

void GA::selekcija(void) {
	// troturnirska selekcija
	vector<Jedinka> next_population;

    for (int i = 0; i < this->elimination_size; ++i) {
        int a = Rand(1, population_size) - 1;
        int b = Rand(2, population_size) - 1;

        if (b <= a){
            b--;
        }

        Jedinka child;

        if (this->population[a].getFitness() < this->population[b].getFitness()) {
            child = krizanje(this->population[a], this->population[b]);
        } else {
            child = krizanje(this->population[b], this->population[a]);
        }

        next_population.push_back(child);
    }

	sort(this->population.begin(), this->population.end(), cmp);
	for (int i = 0; i < this->population_size - this->elimination_size; ++i) {
		next_population.push_back(this->population[i]);
	}

	this->population = next_population;
	return;
}

GA::Jedinka GA::krizanje(const Jedinka &a, const Jedinka &b) {
	Jedinka child;
	/*
	 * (random odabrani X, Y), za jedinke A i B, od A uzmi [0, X] i [Y, kraja]
	 * a od B[X, Y] s time da svi korisnici kaj su vec u B[X, Y] ne uzimamo iz
	 * A[0, X] U A[Y, kraja] (ili obratno)
	 */
	int x = Rand(1, this->warehouses.size()) - 1;
	int y = Rand(1, this->warehouses.size()) - 1;
	if (y < x) swap(x, y);

	set<int> flag;

	for (int i = 0; i < this->warehouses.size(); ++i) {
		vector<User> skladiste;

		if (i <= x || i >= y) {
			skladiste = a.getWarehouseUsers(i);
		} else {
			skladiste = b.getWarehouseUsers(i);
		}

		for (int j = 0; j < skladiste.size(); ++j) {
			if (flag.find(skladiste[j].getID()) == flag.end()) {
				child.setWarehouseUser(i, skladiste[j]);
				flag.insert(skladiste[j].getID());
			}
		}
	}

	// rasporedi preostale
	for (int j = 0; j < this->users.size(); ++j) {
		if (flag.find(j) != flag.end()) {
			continue;
		}

		int warehouse, capacity;
		vector<User> skladiste;

		while (1) {
			warehouse = Rand(1, this->warehouses.size()) - 1;
			skladiste = child.getWarehouseUsers(warehouse);
			capacity = this->warehouses[warehouse].getCapacity();

			for (int k = 0; k < skladiste.size(); ++k) {
				capacity -= skladiste[k].getCapacity();
			}

			if (capacity >= this->users[j].getCapacity()) {
				child.setWarehouseUser(warehouse, this->users[j]);
				break;
			}
		}
	}

	// ako nije dobra -> vrati bolju ili pokusaj opet
	bool fits = true;
	for (int i = 0; i < this->warehouses.size(); ++i) {
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
		child = krizanje(a, b);
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

	int x = Rand(1, this->warehouses.size()) - 1;
	vector<User> skladiste_x = a.getWarehouseUsers(x);

	if (skladiste_x.size() == 0) {
		return a;
	}

	int z = Rand(0, skladiste_x.size() - 1);

	int y;
	while (1) {
		y = Rand(1, this->warehouses.size()) - 1;
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

	for (int i = 0; i < this->warehouses.size(); ++i) {
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
				vector<User> tour = bitonic_tour(tours[j], w[i]);
				res += cost;
				res += bitonic_tour_cost(tour);
			}
			res += w[i].getCost();
		}
	}

	this->fitness = res;
	return;
}
