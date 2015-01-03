/*
 * Ulaz = vector<User> skladiste, kapacitet vozila, trosak vozila
 *
 * Izlaz = cijena otvaranja potrebnih ruta
 */
#include "FirstFit.h"

using namespace std;

inline bool cmp(const User &a, const User &b) {
    return (a.getCapacity() > b.getCapacity());
}

int first_fit(vector<User> users, int capacity, int cost) {
    int res = 0;
    int n = users.size();

    vector<int> bins;

    sort(users.begin(), users.end(), cmp);

    for (int i = 0; i < n; ++i) {
        vector<int>::iterator it;
        for (it = bins.begin(); it != bins.end(); ++it) {
            if (capacity - *it >= users[i].getCapacity()) break;
        }

        if (it == bins.end()) {
            bins.push_back(users[i].getCapacity());
            res += 1;
        } else {
            *it += users[i].getCapacity();
        }
    }

    return res * cost;
}
