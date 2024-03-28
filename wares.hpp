#include <string>

class item {
	std::string name;
	int cost;
	int power;

	public:
		item(std::string iname, int icost, int ipower);

		std::string getname();
		int getcost();
		int getpower();
};
