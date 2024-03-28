#include <iostream>
#include <string>
#include "wares.hpp"


item::item(std::string iname, int icost, int ipower) {
name = iname;
cost = icost;
power = ipower;
}

std::string item::getname() {
	return name;
}

int item::getcost() {
	return cost;
}

int item::getpower() {
	return power;
}
