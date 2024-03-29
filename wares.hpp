#include <string>

class item {
	std::string name;
	int cost;
	int power;
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
