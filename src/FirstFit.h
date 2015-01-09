#pragma once

#include "Data.h"
#include <vector>

std::vector<std::vector<User> > first_fit(std::vector<User> users, int capacity);

std::vector<std::vector<User> > dist_fit(std::vector<User> users, int capacity);
