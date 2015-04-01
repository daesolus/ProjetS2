#ifndef FPGA_H
#define FPGA_H

#include "CommunicationFPGA.h"
#include "easywsclient.hpp"
#include <thread>
#include <assert.h>
#include <stdio.h>
#include <string>
#include <memory>

using easywsclient::WebSocket;
class Fpga
{
private:
	CommunicationFPGA _fpga;
	enum { PhonemeA = 1, PhonemeO = 2, PhonemeU = 4 };
	void threadFpga();
	int btnValeur;
	short int ancienneValeur;
	std::thread tRegistre;
	std::unique_ptr<WebSocket> ws;
public:
	Fpga();
	Fpga(const std::string);
	~Fpga();
	void initializeFpga(){
		tRegistre = std::thread(&Fpga::threadFpga, this);
	}
	void closeFpga(){ tRegistre.detach(); };
};

#endif // FPGA_H