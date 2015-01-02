#pragma once

class Point{
    private:
        int x, y;
        int capacity;

    public:
        Point();
        Point(int _x, int _y);

        void setCapacity(int _capacity);

        int getCapacity(void) const;
        int getX(void) const;
        int getY(void) const;

        int getDistance(const Point &a);
};

class User : public Point {
    private:
        int id;
    public:
        User();
        User(int _id, int _x, int _y);

        int getID(void) const;
};

class Warehouse : public Point {
    private:
        int id;
        int cost;
    public:
        Warehouse();
        Warehouse(int _id, int _x, int _y);

        void setCost(int _cost);

        int getCost(void) const;
        int getID(void) const;
};
