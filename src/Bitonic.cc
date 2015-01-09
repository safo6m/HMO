/*
 * Ulaz = vector<User> skladiste, skladiste
 *
 * Izlaz = cijena BITONIC ture
 */
#include "Bitonic.h"
#include "Constants.h"

#include <stack>
#include <algorithm>

using namespace std;

vector<User> bitonic_tour_brute(vector<User> users, Warehouse w) {
    users.push_back(User(-1, w.getX(), w.getY()));

    vector<User> res;
    int cost = -1;

    sort(users.begin(), users.end());

    do {
        int temp = bitonic_tour_cost(users);
        if (cost == -1 || cost > temp) {
            cost = temp;
            res = users;
        }

    } while (next_permutation(users.begin(), users.end()));

    return res;
}

vector<User> bitonic_tour(vector<User> users, Warehouse w) {
    // insert warehouse
    users.push_back(User(-1, w.getX(), w.getY()));

    vector<User> res;
    int n = users.size();

    int l[NUM_USER + 10][NUM_USER + 10];
    int N[NUM_USER + 10][NUM_USER + 10];

    memset(l, 63, sizeof l);
    memset(N, 0, sizeof N);

    sort(users.begin(), users.end());

    for (int j = 1; j < n; ++j) {
        for (int i = 0; i < j; ++i) {
            if (i == 0 && j == 1) {
                l[i][j] = users[i].getDistance(users[j]);
                N[i][j] = i;
            } else if (j > i + 1) {
                l[i][j] = l[i][j-1] + users[j].getDistance(users[j-1]);
                N[i][j] = j - 1;
            } else {
                for (int k = 0; k < i; ++k) {
                    int q = l[k][i] + users[j].getDistance(users[k]);
                    if (q < l[i][j]) {
                        l[i][j] = q;
                        N[i][j] = k;
                    }
                }
            }
        }
    }

    int k = 0;
    int i = n - 2;
    int j = n - 1;
    stack<int> S[2];

    while (j > 0) {
        S[k].push(j);
        j = N[i][j];
        if (j < i) {
            swap(i, j);
            k = 1 - k;
        }
    }

    S[0].push(0);
    while (!S[1].empty()) {
        S[0].push(S[1].top());
        S[1].pop();
    }

    for (int i = 0; i < n; ++i) {
        int id = S[0].top();
        S[0].pop();

        res.push_back(users[id]);
    }

    return res;
}

int bitonic_tour_cost(vector<User> users) {
    int res = 0;
    int n = users.size();

    for (int i = 1; i < n; ++i) {
        res += users[i].getDistance(users[i - 1]);
    }
    res += users[n - 1].getDistance(users[0]);

    return res;
}

