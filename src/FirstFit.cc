/*
 * Ulaz = vector<User> skladiste, kapacitet vozila, trosak vozila
 *
 * Izlaz = potrebne rute
 */
#include "FirstFit.h"

#include <set>

using namespace std;

inline bool cmp(const User &a, const User &b) {
    return (a.getCapacity() > b.getCapacity());
}

vector<vector<User> > first_fit(vector<User> users, int capacity) {
    int n = users.size();

    vector<int> bins;
    vector<vector<User> > ret;

    sort(users.begin(), users.end(), cmp);

    for (int i = 0; i < n; ++i) {
        vector<int>::iterator it;
        int bin = 0;

        for (it = bins.begin(); it != bins.end(); ++it, ++bin) {
            if (capacity - *it >= users[i].getCapacity()) break;
        }

        if (it == bins.end()) {
            bins.push_back(users[i].getCapacity());
            ret.push_back(vector<User>());
        } else {
            *it += users[i].getCapacity();
        }
        ret[bin].push_back(users[i]);
    }

    return ret;
}

vector< vector<User> > dist_fit(vector<User> users, int capacity) {
    vector< vector<User> > ret;
    set<int> FLAG;

    while (1) {
        vector< User > bin;
        int curr;
        for (curr = 0; curr < users.size(); ++curr) {
		if (FLAG.find(curr) == FLAG.end()) {
			break;
		}
	}

        if (curr >= users.size()) break;

        int cap = capacity - users[curr].getCapacity();

        FLAG.insert(curr);

        bin.push_back(users[curr]);

        while (1) {
            int best_dist, best_id = -1;
            int dist;

            for (auto it = bin.begin(); it != bin.end(); ++it) {
                for (int i = 0; i < users.size(); ++i) {
                    if (FLAG.find(i) != FLAG.end()) continue;
                    if (cap < users[i].getCapacity()) continue;

                    dist = users[i].getDistance(*it);
                    if (best_id == -1 || dist < best_dist) {
                        best_id = i;
                        best_dist = dist;
                    }
                }
            }

            if (best_id != -1) {
                cap -= users[best_id].getCapacity();
                bin.push_back(users[best_id]);
                FLAG.insert(best_id);
            } else {
                break;
            }
        }

        ret.push_back(bin);
    }

    return ret;
}
