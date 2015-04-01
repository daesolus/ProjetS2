#include "Fpga.h"
#include <iostream>
Fpga::Fpga(): tRegistre(), btnValeur(0), ancienneValeur(-1)
{
	if (_fpga.estOk())
	{
		std::cout << _fpga.messageErreur() << endl;
#ifdef _WIN32
		INT rc;
		WSADATA wsaData;

		rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (rc) {
			cout << "WSAStartup Failed." << endl;
		}
#endif
		ws = std::unique_ptr<WebSocket>(WebSocket::from_url("ws://107.170.171.251:56453"));
		assert(ws);
	}
	else
	{
		cout << _fpga.messageErreur() << endl;
	}
}
Fpga::Fpga(const std::string server) : tRegistre(), btnValeur(0), ancienneValeur(-1)
{
	if (_fpga.estOk())
	{
		std::cout << _fpga.messageErreur() << endl;
#ifdef _WIN32
		INT rc;
		WSADATA wsaData;

		rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (rc) {
			cout << "WSAStartup Failed." << endl;
		}
#endif
		ws = std::unique_ptr<WebSocket>(WebSocket::from_url(server));
		assert(ws);
	}
	else
	{
		cout << _fpga.messageErreur() << endl;
	}
}

Fpga::~Fpga()
{	
	if (tRegistre.joinable()) tRegistre.join(); 
#ifdef _WIN32
	WSACleanup();
#endif
	tRegistre.detach();
}

void Fpga::threadFpga()
{
	bool continu = true;
	while (continu)
	{
		_fpga.sleep(100);
		if (_fpga.lireRegistre(BTN, btnValeur))
		{
			if (btnValeur != ancienneValeur)
			{
				std::string ordre = "";
				switch (btnValeur)
				{
				case PhonemeA: ordre = "!r"; break;
				case PhonemeO: ordre = "!m"; break;
				case PhonemeU: ordre = "!l"; break;
				default:
					break;
				}
				if (ordre != "")
				{
					ws->send(ordre);
					ws->poll();
				}
				ancienneValeur = btnValeur;
			}
		}
		else
		{
			cout << _fpga.messageErreur() << endl;
			continu = false;
		}
	}
}
