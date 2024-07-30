#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <csignal>
#include <thread>
#include <unistd.h>
#include <stdlib.h>
#include <cmath>
#include <ctime>
#include "wares.hpp"
#include "hash.hpp"
#include "json.hpp"

// json function made by Niels Lohmann
// hash function made by Frank Thilo

/* NOTES:
some of the formatting is wonky because I'm using nano over ssh to make this :)
this isn't meant to be a GOOD game, it's mainly something to do when I'm bored at school
most of the items are stupid inside jokes from my friend group
*/

using json = nlohmann::json;

// init saved variables here
long long int score = 0;
int clickpower = 1;
int autopower = 0;
double autospeed = 5;
long long int levelup = 500;
/*
hashkey value does not matter, it is just used to make it much harder to change the save values
set useSaving to true to enable modified save detection
doesn't save unlocked items yet, might add that eventually but it's not that important to me right now
hashKey is NOT consistant between commits, and it can and will change randomly
same with useSaving, sometimes I turn it off for development reasons and forget to turn it back on
I'll try to turn it back on for releases though, along with a new hashKey
*/
int hashKey = 1;
bool useSaving = true;

// initlize items here, make sure to put them in the hashmap so the shop can access them
item gobump("gobump", "too deep in the files, +2 ppc", 69, 2, 0, 0);
item clown("Call The Clown", "15000 points, for a cost", -15000, -20, 0, 0);
item balls("Xander's Hairy Black Things", "ultimate power, +1,000,000 ppc", 10000000, 1000000, 0, 0);
item goons("Hire Goons", "Goonery co.'s finest, autoclicks for 500 points", 2000, 0, 0, 500);
item evan("Evan's Statues", "Weighs many kilograms, -0.05 autoclick time", 420, 0, 0.05, 0);
item fortnite("19 Dollar Fortnite Card", "Who wants it? +1 ppc", 19, 1, 0, 0);
item lunch("Lunch Concoction", "Tastes like jelly beans. Random stats", 0, 0, 0,0);
item toughman("Tough Man", "he can lift hotdogs and elephants, -0.5 autoclick time, autoclicks for 900 points", 8000, 0, 0.5, 900);
item gaykeeping("Gaykeeping", "tripod will not see this coming, -3 seconds autoclick time", 6969, 0, 3, -100);
item donate("Donating to the hobos", "spare change?", 1, 0, -0.01, 0);
item groupproject("Group Project", "better find the answer key", 0, -9999999, 999999999, -99999999);
item discord("New Monthly Discord Server", "why do we keep making these? +5 ppc", 7500, 5, 0, 0);
item subura("In Subura", "intrat pompeii, +650 autoclick power", 2479, 0, 0, 650);
item global("Current Event", "What's your opinion on tornadoes? -1 second autoclick time", 4275, 0, 1, 0);

// put dashes or smth between words or it'll segfault :)
std::map<std::string, item*> items {
	{"gobump", &gobump},
	{"call-the-clown", &clown},
	{"xanders-hairy-black-things", &balls},
	{"hire-goons", &goons},
	{"evans-statues", &evan},
	{"19-dollar-fortnite-card", &fortnite},
	{"lunch-concoction", &lunch},
	{"tough-man", &toughman},
	{"gaykeeping", &gaykeeping},
	{"donate", &donate},
	{"group-project", &groupproject},
	{"new-server", &discord},
	{"in-subura", &subura},
	{"current-event", &global}
};
std::vector<std::string> rebuyable({"donate", "new-server"});

void shop() {
lunch.setcost((rand() % 20000) - 10000);
lunch.setpower((rand() % 11500) - 1500);
std::cout << "welcome to the shop\nyou have " << score << " points.\nWhat would you like to buy? " << std::endl;
// display hashmap keys
for (const auto& pair : items) {
                        std::cout << "- " << pair.first << std::endl;
    }
std::string shopIn;
std::cin >> shopIn;

// check if item exists, if user wants to buy, if user has enough points to buy it
if (items.count(shopIn)) {
std::cout << std::endl << (*(items[shopIn])).getdesc() << std::endl;
std::string yn;
std::cout << (*(items[shopIn])).getname() << " costs " << (*(items[shopIn])).getcost() << " points. Do you want to buy it (y/n)? " << std::endl;
std::cin >> yn;
if (yn == "y") {
if (score >= (*(items[shopIn])).getcost()) {
score -= (*(items[shopIn])).getcost();
clickpower += (*(items[shopIn])).getpower();
if (autospeed > (*(items[shopIn])).getautospeed()) {
autospeed -= (*(items[shopIn])).getautospeed();
} else {
std::cout << "autoclick speed cap reached, adding to click power" << std::endl;
clickpower += int((*(items[shopIn])).getautospeed());
}
autopower += (*(items[shopIn])).getautopower();
std::cout << "Thank you for buying " << (*(items[shopIn])).getname() << std::endl;
if (count(rebuyable.begin(), rebuyable.end(), shopIn) == 0) {
items.erase(shopIn);
}
} else {
std::cout << "You don't have enough points. (poor)" << std::endl;
}
}
} else {
std::cout << "we don't have " << shopIn << std::endl;
}
}

// adds autopower to score every {autospeed} seconds
void autoclick() {
while (true) {
sleep(autospeed);
score += autopower;
}
}


// writes to nobsave.json
void jwrite() {
//autospeed = std::round(autospeed * 100) / 100;
std::cout << "\nsaving...\n";
json data;
std::vector<std::string> keys;
for (const auto& pair : items) {
keys.push_back(pair.first);
}
data["keys"] = keys;
data["score"] = std::to_string(score);
// save the hash only if it's enabled
if (useSaving) {
data["savehash"] = md5(std::to_string(score + clickpower + autospeed + autopower + levelup + hashKey));
}
data["power"] = std::to_string(clickpower);
data["autospeed"] = std::to_string(autospeed);
data["autopower"] = std::to_string(autopower);
data["levelup"] = std::to_string(levelup);
std::ofstream file("nobsave.json");
    file << std::setw(4) << data << std::endl;
    file.close();

}

// reads data.json
void jread() {
std::ifstream file("nobsave.json");

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
    score = std::stod(data["score"].get<std::string>());
    clickpower = std::stod(data["power"].get<std::string>());
    autospeed = std::stod(data["autospeed"].get<std::string>());
    autopower = std::stod(data["autopower"].get<std::string>());
    levelup = std::stoi(data["levelup"].get<std::string>());
    std::vector<std::string> keys = data["keys"];

// check if user has already bought item (no hashing for this yet)
for (auto it = items.begin(); it != items.end(); ) {
    if (std::find(keys.begin(), keys.end(), it->first) == keys.end()) {
        it = items.erase(it);
    } else {
        ++it;
    }
}
// check save if useSaving is true
if (useSaving) {
    std::string savehash = data["savehash"];
	if (md5(std::to_string(score + clickpower + autospeed + autopower + levelup + hashKey)) != savehash) {
		std::cout << "\nchanging nobsave.json is against the rules\n\nTRUMP THUMP (since joe biden died of ligma)!" << std::endl;
		score = 0;
		clickpower = 0;
		jwrite();
	}
}
}

// if user uses ctrl+c, save before quitting
// ctrl+c saving still doesn't work on windows but it doesn't do the weird thing anymore
void signalHandler(int signal) {
	jwrite();
	exit(0);
}

// earlier today there was some huge bug that i wanted to fix but i forgot what it was so that's probably bad
int main() {
// seed random number generator
srand ( time(NULL) );

// start autoclick thread
std::thread thread_obj(autoclick);
// initialize signal handler
signal(SIGINT, signalHandler);
// check if nobsave.json exists, make new one if not
try {
	jread();
} catch(...) {
	// runs on any error, may be bad under very specific circumstances
	std::cout << "Generating new nobsave.json..." << std::endl;
	jwrite();
}

std::string userIn;

std::cout << "press enter for more points , quit to save and quit, help for help, shop for shop, save to save, status for clicking status" << std::endl;

// while true to keep the thingy going until the user wants to stop playing
while (true) {
std::cout << score << " pts" << std::endl;
   std::getline(std::cin, userIn);

// check user input for keyword, add points if blank
    if (userIn.empty()) {
        score += clickpower;
    } else if (userIn == "quit") {
	jwrite();
	exit(0);
} else if (userIn == "help") {
	if (clickpower == 0) {
		std::cout << "delete nobsave.json.." << std::endl;
		break;
	}
	std::cout << "enter for points, quit to save and quit, shop for shop, save to save, status for clicking status" << std::endl;
} else if (userIn == "shop") {
	shop();
} else if (userIn == "save") {
    jwrite();
} else if (userIn == "status") {
    std::cout << std::endl << "Each click gives you " << clickpower << " points\nYou are automatically gaining " << autopower << " points every " << autospeed << " second(s)\n" << std::endl;
}

if (score >= levelup) {
clickpower += 2;
levelup *= 2;
std::cout << "You levelled up! Next level up at: " << levelup << std::endl;
}

}

return 0;
}
