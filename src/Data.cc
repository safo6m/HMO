#include "Data.h"

#include <cmath>

using namespace std;

#define SQR(x) ((x)*(x))

Point::Point() {
}

Point::Point(int _x, int _y) :
    x(_x),
    y(_y) {
}

void Point::setCapacity(int _capacity) {
    capacity = _capacity;
}

int Point::getCapacity(void) const {
    return capacity;
}

int Point::getX(void) const {
    return x;
}

int Point::getY(void) const {
    return y;
}

int Point::getDistance(const Point &a) {
    return (int) 100 * sqrt(
            SQR(a.getX() - getX()) +
            SQR(a.getY() - getY()));
}

User::User() {
}

User::User(int _id, int _x, int _y) :
    Point(_x, _y),
    id(_id) {
}

int User::getID(void) const {
    return id;
}

bool User::operator <(const User &a) const {
    if (getX() == a.getX()) {
        return getY() < a.getY();
    } else {
        return getX() < a.getX();
    }
}

bool User::operator ==(const User &a) const {
    return !((*this < a) || (a < *this));
}

Warehouse::Warehouse() {
}

Warehouse::Warehouse(int _id, int _x, int _y) :
    Point(_x, _y),
    id(_id) {
}

void Warehouse::setCost(int _cost) {
    cost = _cost;
}

int Warehouse::getCost(void) const {
    return cost;
}

int Warehouse::getID(void) const {
    return id;
}


