#pragma once

#include "Data.h"
#include <vector>

std::vector<User> bitonic_tour(std::vector<User> users, Warehouse w);

int bitonic_tour_cost(std::vector<User> users);
