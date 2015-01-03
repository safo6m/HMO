/*
 * Ulaz = vector<User> skladiste, kapacitet vozila, trosak vozila
 *
 * Izlaz = potrebne rute
 */
#include "FirstFit.h"

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
