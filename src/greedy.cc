#include <cstdio>
#include <cmath>
#include <vector>

using namespace std;

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

bool FLAG[1024];

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

vector< vector<user_t> > first_fit(vector<user_t> users, int capacity) {
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

vector< vector<user_t> > dist_fit(vector<user_t> users, int capacity) {
    vector< vector<user_t> > ret;

    memset(FLAG, 0, sizeof FLAG);

    while (1) {
        vector< user_t > bin;
        int curr;
        int cap;

        for (curr = 0; curr < users.size() && FLAG[curr]; ++curr);

        if (curr >= users.size()) break;

        FLAG[curr] = 1;
        cap = capacity - users[curr].capacity;

        bin.push_back(users[curr]);

        while (cap > 0) {
            int best_dist, best_id = -1;
            int dist;

            for (auto it = bin.begin(); it != bin.end(); ++it) {
                for (int i = 0; i < users.size(); ++i) {
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

int main(int argc, char** argv) {
    int x, y;
    int num_user, num_warehouse;
    int vehicle_capacity, vehicle_cost;
    int warehouse_capacity, user_capacity, warehouse_cost;

    memset(FLAG, 0, sizeof FLAG);

    vector<user_t> users;
    vector<warehouse_t> warehouses;

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
            return 1;
        }

        FLAG[best_user_id] = 1;
        w[best_id] += users[best_user_id].capacity;
        greedy[best_id].push_back(users[best_user_id]);
    }

    vector< user_t > curr;
    vector< vector<user_t> > tours;
    vector< pair< int, vector<user_t> > > output;
    int sol = 0, tours_cnt = 0;

    for (int i = 0; i < num_warehouse; ++i) {
        if (greedy[i].size() > 0) {
            sol += warehouses[i].cost;

            tours = dist_fit(greedy[i], vehicle_capacity);

            for (int j = 0; j < tours.size(); ++j) {
                curr = tour(tours[j], warehouses[i]);

                sol += tour_cost(curr);
                sol += vehicle_cost;

                output.push_back(make_pair(i, curr));
            }

            tours_cnt += tours.size();
        }
    }

    printf("%d\n\n", tours_cnt);

    for (int i = 0; i < output.size(); ++i) {
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
    fprintf(stderr, "%d\n", sol);

    return 0;
}
