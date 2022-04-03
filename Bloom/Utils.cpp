#include "Utils.hpp"

void Utils::createMap(std::map<std::string, char>* um)
{
	(*um)["0000"] = '0';
	(*um)["0001"] = '1';
	(*um)["0010"] = '2';
	(*um)["0011"] = '3';
	(*um)["0100"] = '4';
	(*um)["0101"] = '5';
	(*um)["0110"] = '6';
	(*um)["0111"] = '7';
	(*um)["1000"] = '8';
	(*um)["1001"] = '9';
	(*um)["1010"] = 'A';
	(*um)["1011"] = 'B';
	(*um)["1100"] = 'C';
	(*um)["1101"] = 'D';
	(*um)["1110"] = 'E';
	(*um)["1111"] = 'F';
}

const char* Utils::hex_char_to_bin(char c)
{
	// TODO handle default / error
	switch (toupper(c))
	{
	case '0': return "0000";
	case '1': return "0001";
	case '2': return "0010";
	case '3': return "0011";
	case '4': return "0100";
	case '5': return "0101";
	case '6': return "0110";
	case '7': return "0111";
	case '8': return "1000";
	case '9': return "1001";
	case 'A': return "1010";
	case 'B': return "1011";
	case 'C': return "1100";
	case 'D': return "1101";
	case 'E': return "1110";
	case 'F': return "1111";
	}
}

// function to find hexadecimal
// equivalent of binary
std::string Utils::BinToHex(std::string bin)
{
	int l = bin.size();
	for (int i = 1; i <= (4 - l % 4) % 4; i++)
		bin = '0' + bin;

	std::map<std::string, char> bin_hex_map;
	createMap(&bin_hex_map);

	int i = 0;
	std::string hex = "";

	while (1)
	{
		hex += bin_hex_map[bin.substr(i, 4)];
		i += 4;
		if (i == bin.size())
			break;
	}

	//std::cout << "BINARY TO HEX [" + hex + "]" << std::endl;
	// required hexadecimal number
	return hex;
}