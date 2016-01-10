#include <stdio.h>
#include <tchar.h>
#include <map>
#include <vector>
#include <string>
#include <exception>
#include "RDService.h"

int main(int argc, char *args[])
{
	if (argc != 2)
	{
		wprintf(U("Service will be started at 5500 port\n"));
	}

	std::wstring address = U("http://localhost:5500");


	RDService listener(address);
	listener.open().wait();

	std::wcout << utility::string_t(U("Listening for requests at: ")) << address << std::endl;

	std::string line;
	while (true)
	{
		std::wcout << U("Hit 1 to insert some predefined dummy data") <<std::endl;
		std::wcout << U("Hit 2 to quit") << std::endl;
		std::getline(std::cin, line);
		if (line == "1")
		{
			listener.insertDummyData();
		}
		else if(line == "2")
		{
			break;
		}
	}
	

	listener.close().wait();

	return 0;
}