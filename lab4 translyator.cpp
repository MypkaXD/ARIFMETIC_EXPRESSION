#include "Header.h"
#include <iostream>

int main() {

	std::cout << "ENTER THE EXPRESSION: " << std::endl;

	while (true) {
		try {
			char s[1000];
			std::cin.getline(s, 1000);
			Lecsems l(s);
			ArithmeticСalculation ac(l);
			std::cout << ac.calculating() << std::endl;
		}
		catch (const char* e) {
			std::cout << e << std::endl;
		}
	}

	return 0;
}