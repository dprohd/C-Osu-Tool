#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <functional>
#include <algorithm>
#include <string>

bool settExist(const std::string& name);
std::string genRandom(size_t length);
void crtSett();