#include <iostream>
#include <string>
#include "wares.hpp"


item::item(std::string iname, int icost, int ipower, double iautospeed, int iautopower) {
name = iname;
cost = icost;
power = ipower;
autospeed = iautospeed;
autopower = iautopower;
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

double item::getautospeed() {
	return autospeed;
}

int item::getautopower() {
	return autopower;
}
