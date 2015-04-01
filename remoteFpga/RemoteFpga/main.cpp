#include "Fpga.h"
#include <iostream>
int main()
{
	Fpga fpga;
	fpga.initializeFpga();
	char text;
	cout << "Appuyer sur une touche pour terminer le programme." << endl;
	cin >> text;
	fpga.closeFpga();
	return 0;
}