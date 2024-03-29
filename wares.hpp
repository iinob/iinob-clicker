#include <string>

class item {
	std::string name;
	// leave as zero for no click power bonus
	int cost;
	int power;
	// leave as zero for no autoclick bonus
	// autospeed is in seconds, base value is 5
	double autospeed;
	int autopower;

	public:
		item(std::string iname, int icost, int ipower, double iautospeed, int iautopower);

		std::string getname();
		int getcost();
		int getpower();
		double getautospeed();
		int getautopower();
};
