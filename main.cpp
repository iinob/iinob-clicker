#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <csignal>
#include <thread>
#include <unistd.h>
#include "wares.hpp"
#include "hash.hpp"
#include "json.hpp"

// json function made by Niels Lohmann
// hash function made by Frank Thilo

using json = nlohmann::json;

// init saved variables here
long long int score = 0;
int clickpower = 1;
int autopower = 0;
double autospeed = 5;
// hashkey value does not matter, it is just used to make it much harder to change the save values
// set useSaving to true to enable modified save detection
int hashKey = 0;
bool useSaving = false;

item gobump("gobump", 69, 2, 0, 0);
item clown("call-the-clown", -1000, -10, 0, 0);
item balls("xanders-hairy-balls", 10000000, 1000000, 0, 0);
item goons("hire-goons", 150, 0, 0, 150);
std::map<std::string, item*> items {
	{"gobump", &gobump},
	{"call-the-clown", &clown},
	{"xanders-hairy-balls", &balls},
	{"hire-goons", &goons}
};

void shop() {
std::cout << "welcome to the shop\nyou have " << score << " points.\nWhat would you like to buy? " << std::endl;
for (const auto& pair : items) {
                        std::cout << "- " << pair.first << std::endl;
    }
std::string shopIn;
std::cin >> shopIn;
if (items.count(shopIn)) {
std::string yn;
std::cout << (*(items[shopIn])).getname() << " costs " << (*(items[shopIn])).getcost() << " points. Do you want to buy it (y/n)? " << std::endl;
std::cin >> yn;
if (yn == "y") {
if (score >= (*(items[shopIn])).getcost()) {
score -= (*(items[shopIn])).getcost();
clickpower += (*(items[shopIn])).getpower();
autospeed -= (*(items[shopIn])).getautospeed();
autopower += (*(items[shopIn])).getautopower();
std::cout << "Thank you for buying " << (*(items[shopIn])).getname() << std::endl;
} else {
std::cout << "You don't have enough points." << std::endl;
}
}
} else {
std::cout << "we don't have " << shopIn << std::endl;
}
}

void autoclick() {
while (true) {
sleep(autospeed);
score += autopower;
std::cout << "autotest" << std::endl;
}
}



// writes to data.json
void jwrite() {
std::cout << "\nsaving...\n";
json data;
data["score"] = std::to_string(score);
if (useSaving) {
data["savehash"] = md5(std::to_string(score + clickpower + hashKey));
}
data["power"] = std::to_string(clickpower);
std::ofstream file("data.json");
    file << std::setw(4) << data << std::endl;
    file.close();

}

// reads data.json
void jread() {
std::ifstream file("data.json");

    // check if file opens
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
    }

    // grab json
    json data;
    file >> data;

    // close file
    file.close();

    // get json
    score = std::stoi(data["score"].get<std::string>());
    clickpower = std::stoi(data["power"].get<std::string>());
if (useSaving) {
    std::string savehash = data["savehash"];
	if (md5(std::to_string(score + clickpower + hashKey)) != savehash) {
		std::cout << "\nchanging data.json is against the rules\n\nBIDEN BLAST!!!" << std::endl;
		score = 0;
		clickpower = 0;
		jwrite();
	}
}
    // display json
    std::cout << "jread: " << score << std::endl;
}

// ctrl+c saving is broken on windows
void signalHandler(int signal) {
	jwrite();
	exit(0);
}

int main() {
std::thread thread_obj(autoclick);
signal(SIGINT, signalHandler);
// check if data.json exists, make new one if not
try {
	jread();
} catch(...) {
	// runs on any error, may be bad under very specific circumstances
	std::cout << "Generating new data.json..." << std::endl;
	jwrite();
}

std::string userIn;

std::cout << "press enter for more points , q to quit, help for help, s for shop" << std::endl;

while (true) {
std::cout << score << " pts" << std::endl;
   std::getline(std::cin, userIn);

    if (userIn.empty()) {
        score += clickpower;
    } else if (userIn == "q") {
	jwrite();
	exit(0);
} else if (userIn == "help") {
	if (clickpower == 0) {
		std::cout << "delete data.json.." << std::endl;
		break;
	}
	std::cout << "enter for points, q to quit, s for shop" << std::endl;
} else if (userIn == "s") {
	shop();
}


}

return 0;
}
