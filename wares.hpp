#include <string>

class item {
	std::string name;
	std::string desc;
	// leave as zero for no click power bonus
	int cost;
	int power;
	// leave as zero for no autoclick bonus
	// autospeed is in seconds, base value is 5
	double autospeed;
	int autopower;

	public:
		item(std::string iname, std::string idesc, int icost, int ipower, double iautospeed, int iautopower);

		std::string getname();
		std::string getdesc();
		int getcost();
		void setcost(int icost);
		int getpower();
		void setpower(int ipower);
		double getautospeed();
		int getautopower();
};
