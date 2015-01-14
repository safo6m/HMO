#include <cstdio>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <algorithm>

using namespace std;

const int POPULATION = 10;
const int ITERATION  = 1000;

inline int Rand(int lo, int hi) {
	return (rand() % (hi - lo + 1)) + lo;
}

class user_t {
    public:
        int id;
        int x, y;
        int capacity;

        int get_distance(const user_t &u) const {
            return (int) 100 * sqrt(pow(u.x - x, 2.) + pow(u.y - y, 2.));
        }

        bool operator ==(const user_t &u) const {
            return (id == u.id);
        }

        bool operator <(const user_t &u) const {
            return (id < u.id);
        }

        user_t() {}

        user_t(int _id, int _x, int _y) {
            id = _id;
            x = _x;
            y = _y;
        }
};

class warehouse_t : public user_t {
    public:
        int cost;

        warehouse_t() {}
} global_warehouse;

inline bool cmp_distance(const user_t &a, const user_t &b) {
    return (a.get_distance(global_warehouse) < b.get_distance(global_warehouse));
}

inline bool cmp_capacity(const user_t &a, const user_t &b) {
    return (a.capacity > b.capacity);
}

int tour_cost(vector<user_t> users) {
    int res = 0;
    int n = users.size();

    for (int i = 1; i < n; ++i) {
        res += users[i].get_distance(users[i - 1]);
    }
    res += users[n - 1].get_distance(users[0]);

    return res;
}

int minimal_cost(vector<user_t> users) {
    int cost = -1;

    sort(users.begin(), users.end());

    do {
        int temp = tour_cost(users);
        if (cost == -1 || temp < cost) {
            cost = temp;
        }

    } while (next_permutation(users.begin(), users.end()));

    return cost;
}

vector<user_t> tour(vector<user_t> users, warehouse_t w) {
    user_t ware = user_t(-1, w.x, w.y);

    users.push_back(ware);

    vector<user_t> res;
    int cost = -1;

    sort(users.begin(), users.end());

    do {
        int temp = tour_cost(users);
        if (cost == -1 || temp < cost) {
            cost = temp;
            res = users;
        }

    } while (next_permutation(users.begin(), users.end()));

    return res;
}

vector< vector<user_t> > first_fit(vector<user_t> users, int capacity, warehouse_t warehouse) {
    int n = users.size();

    vector<int> bins;
    vector< vector<user_t> > ret;

    sort(users.begin(), users.end(), cmp_capacity);

    for (int i = 0; i < n; ++i) {
        vector<int>::iterator it;
        int bin = 0;

        for (it = bins.begin(); it != bins.end(); ++it, ++bin) {
            if (capacity - *it >= users[i].capacity) break;
        }

        if (it == bins.end()) {
            bins.push_back(users[i].capacity);
            ret.push_back(vector<user_t>());
        } else {
            *it += users[i].capacity;
        }
        ret[bin].push_back(users[i]);
    }

    return ret;
}

vector< vector<user_t> > dist_fit(vector<user_t> users, int capacity, warehouse_t warehouse) {
    vector< vector<user_t> > ret;

    bool FLAG[1024];
    memset(FLAG, 0, sizeof FLAG);

    while (1) {
        vector< user_t > bin;
        int curr;
        int cap;

        for (curr = 0; curr < (int)users.size() && FLAG[curr]; ++curr);
        if (curr >= (int)users.size()) break;

        FLAG[curr] = 1;
        cap = capacity - users[curr].capacity;

        bin.push_back(users[curr]);

        while (cap > 0) {
            int best_dist, best_id = -1;
            int dist;

            for (auto it = bin.begin(); it != bin.end(); ++it) {
                for (int i = 0; i < (int)users.size(); ++i) {
                    if (FLAG[i]) continue;
                    if (cap < users[i].capacity) continue;

                    dist = users[i].get_distance(*it);
                    if (best_id == -1 || dist < best_dist) {
                        best_id = i;
                        best_dist = dist;
                    }
                }
            }

            if (best_id != -1) {
                cap -= users[best_id].capacity;
                bin.push_back(users[best_id]);
                FLAG[best_id] = 1;
            } else {
                break;
            }
        }

        ret.push_back(bin);
    }

    return ret;
}

vector< vector<user_t> > closest_fit(vector<user_t> users, int capacity, warehouse_t w) {
    vector< vector<user_t> > ret;

    bool FLAG[1024];
    memset(FLAG, 0, sizeof FLAG);

    while (1) {
        vector< user_t > bin;
        int best_dist, best_id = -1, dist;
        int cap = capacity;

        for (int i = 0; i < (int)users.size(); ++i) {
            if (FLAG[i]) continue;

            dist = w.get_distance(users[i]);
            if (best_id == -1 || dist < best_dist) {
                best_dist = dist;
                best_id = i;
            }
        }

        if (best_id == -1) break;

        FLAG[best_id] = 1;
        cap -= users[best_id].capacity;
        bin.push_back(users[best_id]);

        while (cap > 0) {
            best_id = -1;

            for (auto it = bin.begin(); it != bin.end(); ++it) {
                for (int i = 0; i < (int)users.size(); ++i) {
                    if (FLAG[i]) continue;
                    if (cap < users[i].capacity) continue;

                    dist = users[i].get_distance(*it) + users[i].get_distance(w);
                    if (best_id == -1 || dist < best_dist) {
                        best_dist = dist;
                        best_id = i;
                    }
                }
            }

            if (best_id != -1) {
                FLAG[best_id] = 1;
                cap -= users[best_id].capacity;
                bin.push_back(users[best_id]);
            } else {
                break;
            }
        }

        ret.push_back(bin);

    }

    return ret;
}

vector< vector<user_t> > arrange_users_dist(
        const vector<warehouse_t> &warehouses, int num_warehouse,
        const vector<user_t> &users, int num_user,
        int vehicle_capacity, int vehicle_cost) {

    vector< vector<user_t> > custom_users;
    custom_users.resize(num_warehouse);

    for (int i = 0; i < num_warehouse; ++i) {
        custom_users[i] = users;
        global_warehouse = warehouses[i];
        sort(custom_users[i].begin(), custom_users[i].end(), cmp_distance);
    }

    vector<int> w(num_warehouse, 0);
    vector<int> last_id(num_warehouse, 0);
    vector< vector<user_t> > greedy;
    greedy.resize(num_warehouse);

    bool FLAG[1024];
    memset(FLAG, 0, sizeof FLAG);

    for (int left = num_user; left > 0; -- left) {
        int best_dist, best_id = -1, best_user_id = -1;
        for (int i = 0; i < num_warehouse; ++i) {
            while (last_id[i] < num_user && FLAG[custom_users[i][last_id[i]].id]) {
                last_id[i] += 1;
            }

            if (last_id[i] < num_user) {

                int need = w[i] + custom_users[i][last_id[i]].capacity;

                int dist = custom_users[i][last_id[i]]
                    .get_distance(warehouses[i]);

                if (need > warehouses[i].capacity) {
                    continue;
                }

                if (best_id == -1 || dist < best_dist) {
                    best_id = i;
                    best_user_id = custom_users[i][last_id[i]].id;
                    best_dist = dist;
                }
            }

        }

        if (best_id == -1) {
            printf("Something went wrong!");
            exit(1);
        }

        FLAG[best_user_id] = 1;
        w[best_id] += users[best_user_id].capacity;
        greedy[best_id].push_back(users[best_user_id]);
    }

    return greedy;
}

vector< vector<user_t> > arrange_users(
        const vector<warehouse_t> &warehouses, int num_warehouse,
        const vector<user_t> &users, int num_user,
        int vehicle_capacity, int vehicle_cost) {

    vector<int> w(num_warehouse, 0);

    vector< vector<user_t> > greedy;
    greedy.resize(num_warehouse);

    vector< vector<user_t> > ret;
    ret.resize(num_warehouse);

    bool FLAG[1024];
    memset(FLAG, 0, sizeof FLAG);

    for (int i = 0; i < num_warehouse; ++i) {
        greedy[i].push_back(user_t(-1, warehouses[i].x, warehouses[i].y));
    }

    while (1) {
        int best_id = -1, best_dist, best_user_id;
        int dist;

        for (int i = 0; i < num_warehouse; ++i) {
            for (auto it = greedy[i].begin(); it != greedy[i].end(); ++it) {
                for (int j = 0; j < num_user; ++j) {
                    if (FLAG[j]) continue;
                    if (w[i] + users[j].capacity > warehouses[i].capacity) continue;

                    dist = users[j].get_distance(warehouses[i]) +
                        users[j].get_distance(*it);

                    if (w[i] == 0)
                        dist += warehouses[i].cost;

                    if (best_id == -1 || dist < best_dist) {
                        best_id = i;
                        best_user_id = j;
                        best_dist = dist;
                    }
                }
            }
        }

        if (best_id == -1) break;
        FLAG[best_user_id] = 1;
        greedy[best_id].push_back(users[best_user_id]);
        ret[best_id].push_back(users[best_user_id]);
        w[best_id] += users[best_user_id].capacity;
    }

    return ret;
}

vector< vector<user_t> > arrange_users_fitness(
        const vector<warehouse_t> &warehouses, int num_warehouse,
        const vector<user_t> &users, int num_user,
        int vehicle_capacity, int vehicle_cost) {

    vector<int> w(num_warehouse, 0);

    vector< vector<user_t> > ret;
    ret.resize(num_warehouse);

    bool FLAG[1024];
    memset(FLAG, 0, sizeof FLAG);

    while (1) {
        int best_id = -1, best_dist, best_user_id;
        int dist;

        for (int i = 0; i < num_warehouse; ++i) {
            for (int j = 0; j < num_user; ++j) {
                if (FLAG[j]) continue;
                if (w[i] + users[j].capacity > warehouses[i].capacity) continue;

                dist = warehouses[i].cost;

                {
                    vector<user_t> temp;
                    vector< vector<user_t> > tours;

                    temp = ret[i];
                    temp.push_back(users[j]);

                    tours = closest_fit(temp, vehicle_capacity, warehouses[i]);

                    for (int k = 0; k < (int)tours.size(); ++k) {
                        dist += tour_cost(tour(tours[k], warehouses[i]));
                        dist += vehicle_cost;
                    }
                }

                if (best_id == -1 || dist < best_dist) {
                    best_id = i;
                    best_user_id = j;
                    best_dist = dist;
                }
            }
        }

        if (best_id == -1) break;
        FLAG[best_user_id] = 1;
        ret[best_id].push_back(users[best_user_id]);
        w[best_id] += users[best_user_id].capacity;
    }

    return ret;
}

int fitness(vector< vector<user_t> > sol,
        const vector<warehouse_t> warehouses,
        int num_warehouse,
        int vehicle_capacity,
        int vehicle_cost) {

    int fitness = 0;

    vector< user_t > curr;
    vector< vector<user_t> > tours;

    for (int i = 0; i < num_warehouse; ++i) {
        if (sol[i].size() > 0) {
            fitness += warehouses[i].cost;

            tours = closest_fit(sol[i], vehicle_capacity, warehouses[i]);

            for (int j = 0; j < (int)tours.size(); ++j) {
                fitness += tour_cost(tour(tours[j], warehouses[i]));
                fitness += vehicle_cost;
            }
        }
    }

    return fitness;
}

vector< vector<user_t> > mutation(const vector< vector<user_t> > &x,
        const vector<warehouse_t> &warehouses,
        int num_warehouse,
        const vector<user_t> &users,
        int num_user) {

    srand(time(NULL));
    vector< vector<user_t> > mut(num_warehouse);

    int src = Rand(1, num_warehouse) - 1;

    if (x[src].size() == 0) return x;

    int id = x[src][Rand(1, x[src].size()) - 1].id;

    while (1) {
        int dst = Rand(1, num_warehouse) - 1;
        if (src == dst) continue;

        int left = warehouses[dst].capacity;
        for (auto it = x[dst].begin(); it != x[dst].end(); ++it) {
            left -= it->capacity;
        }

        if (users[id].capacity <= left) {
            for (int i = 0; i < num_warehouse; ++i) {
                if (i == src) continue;
                mut[i] = x[i];
            }
            for (auto it = x[src].begin(); it != x[src].end(); ++it) {
                if (it->id == id) continue;
                mut[src].push_back(*it);
            }
            mut[dst].push_back(users[id]);
            break;
        }
    }

    return mut;
}


vector< vector<user_t> > local_search(const vector< vector<user_t> > &x,
        const vector<warehouse_t> &warehouses,
        int num_warehouse,
        const vector<user_t> &users,
        int num_user,
        int vehicle_capacity,
        int vehicle_cost,
        int limit = -1) {

    int fit = fitness(x,
            warehouses,
            num_warehouse,
            vehicle_capacity,
            vehicle_cost);

    int best_fit = fit;
    int curr_fit;

    vector< vector<user_t> > sol = x, curr;
    queue< pair<int, vector< vector<user_t> > > > q;

    for (q.push(make_pair(fit, sol)); !q.empty(); q.pop()) {
        curr = q.front().second;
        curr_fit = q.front().first;

        fprintf(stderr, ".");

        for (int i = 0; i < num_user; ++i) {
            for (int j = i + 1; j < num_user; ++j) {
                vector< vector<user_t> > temp(num_warehouse);

                /* swap */
                for (int k = 0; k < num_warehouse; ++k) {
                    for (auto it = curr[k].begin(); it != curr[k].end(); ++it) {
                        if (it->id == i) {
                            temp[k].push_back(users[j]);
                        } else if (it->id == j) {
                            temp[k].push_back(users[i]);
                        } else {
                            temp[k].push_back(*it);
                        }
                    }
                }

                bool valid = true;
                /* check if valid */
                for (int k = 0; k < num_warehouse; ++k) {
                    int capacity = warehouses[k].capacity;
                    for (auto it = temp[k].begin(); it != temp[k].end(); ++it) {
                        capacity -= users[it->id].capacity;
                    }
                    if (capacity < 0) {
                        valid = false;
                        break;
                    }
                }

                if (!valid) continue;

                /* check new fitness */
                int temp_fit = fitness(temp,
                        warehouses,
                        num_warehouse,
                        vehicle_capacity,
                        vehicle_cost);

                /* update solution */
                if (temp_fit < best_fit) {
                    best_fit = temp_fit;
                    sol = temp;
                }

                /* check if better than starting */
                if (temp_fit < curr_fit) {
                    q.push(make_pair(temp_fit, temp));
                }
            }
        }

        if (limit != -1) {
            if (limit == 0) break;
            limit -= 1;
        }
    }

    return sol;
}

int main(int argc, char** argv) {
    int x, y;
    int num_user, num_warehouse;
    int vehicle_capacity, vehicle_cost;
    int warehouse_capacity, user_capacity, warehouse_cost;

    bool FLAG[1024];
    memset(FLAG, 0, sizeof FLAG);

    vector<user_t> users;
    vector<warehouse_t> warehouses;

    fprintf(stderr, "Reading input file ...");

    FILE *f = fopen("input.in", "r");
    fscanf(f, "%d", &num_user);
    fscanf(f, "%d", &num_warehouse);

    warehouses.resize(num_warehouse);
    users.resize(num_user);

    for (int i = 0; i < num_warehouse; ++i) {
        fscanf(f, "%d%d", &x, &y);
        warehouses[i].id = i;
        warehouses[i].x = x;
        warehouses[i].y = y;
    }

    for (int i = 0; i < num_user; ++i) {
        fscanf(f, "%d%d", &x, &y);
        users[i].id = i;
        users[i].x = x;
        users[i].y = y;
    }

    fscanf(f, "%d", &vehicle_capacity);

    for (int i = 0; i < num_warehouse; ++i) {
        fscanf(f, "%d", &warehouse_capacity);
        warehouses[i].capacity = warehouse_capacity;
    }

    for (int i = 0; i < num_user; ++i) {
        fscanf(f, "%d", &user_capacity);
        users[i].capacity = user_capacity;
    }

    for (int i = 0; i < num_warehouse; ++i) {
        fscanf(f, "%d", &warehouse_cost);
        warehouses[i].cost = warehouse_cost;
    }

    fscanf(f, "%d", &vehicle_cost);

    fprintf(stderr, "done\n");

    fprintf(stderr, "Computing first solution ...");
    // arrange_users, arrange_users_dist, arrange_users_fitness
    vector< vector<user_t> > greedy = arrange_users(
            warehouses, num_warehouse,
            users, num_user,
            vehicle_capacity, vehicle_cost);

    int f_gre = fitness(greedy,
            warehouses,
            num_warehouse,
            vehicle_capacity,
            vehicle_cost);

    fprintf(stderr, "done\n");

    vector< pair< vector< vector<user_t> >, int > > population;
    for (int i = 0; i < POPULATION; ++i) {
        population.push_back(make_pair(greedy, f_gre));
    }

    fprintf(stderr, "[||||||||||]\n ");
    for (int i = 0; i < ITERATION; ++i) {
		if (i % (ITERATION / 10) == 0) fprintf(stderr, ".");

        for (int j = 0; j < POPULATION; ++j) {
            vector< vector< user_t > > mut =
                mutation(population[j].first,
                         warehouses,
                         num_warehouse,
                         users,
                         num_user);

            int f_mut = fitness(mut,
                    warehouses,
                    num_warehouse,
                    vehicle_capacity,
                    vehicle_cost);

            int f_pop = population[j].second;

            if (f_mut < f_pop) {
                population[j].first = mut;
                population[j].second = f_mut;
            }
        }
    }

	fprintf(stderr, "\n");

    fprintf(stderr, "Starting local search: ");

    for (int i = 0; i < POPULATION; ++i) {
        fprintf(stderr, "\tsolution %d: ", i + 1);

        vector< vector<user_t> > local = local_search(population[i].first,
                warehouses,
                num_warehouse,
                users,
                num_user,
                vehicle_capacity,
                vehicle_cost,
                100);

        int f_local = fitness(local,
                warehouses,
                num_warehouse,
                vehicle_capacity,
                vehicle_cost);

        if (f_local < population[i].second) {
            population[i] = make_pair(local, f_local);
        }

        fprintf(stderr, "done\n");
    }

	fprintf(stderr, "Finding best solution ...");

    for (int i = 0; i < POPULATION; ++i) {
        int f_pop = population[i].second;

        if (f_pop < f_gre) {
            greedy = population[i].first;
            f_gre = f_pop;
        }
    }

	fprintf(stderr, "done\n");

    vector< user_t > curr;
    vector< vector<user_t> > tours;
    vector< pair< int, vector<user_t> > > output;
    int sol = 0, tours_cnt = 0;

    for (int i = 0; i < num_warehouse; ++i) {
        if (greedy[i].size() > 0) {
            sol += warehouses[i].cost;

            // dist_fit, first_fit, closest_fit
            tours = closest_fit(greedy[i], vehicle_capacity, warehouses[i]);

            for (int j = 0; j < (int)tours.size(); ++j) {
                curr = tour(tours[j], warehouses[i]);

                sol += tour_cost(curr);
                sol += vehicle_cost;

                output.push_back(make_pair(i, curr));
            }

            tours_cnt += tours.size();
        }
    }

    printf("%d\n\n", tours_cnt);

    for (int i = 0; i < (int)output.size(); ++i) {
        int &id = output[i].first;
        vector< user_t > &path = output[i].second;

        printf("%d: ", id);

        int offset, n = path.size();

        for (offset = 0; path[offset].id != -1; ++offset);
        for (int j = 1; j < n; ++j) {
            printf(" %d", path[(j + offset) % n].id);
        }

        printf("\n\n");
    }

    printf("\n%d\n", sol);
    fprintf(stderr, "Minimal computed cost: %d\n", sol);

    return 0;
}
