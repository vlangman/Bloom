#pragma once

#include <string>
#include <map>
;//WTF IS GOING ON HERE WHY IS THIS SEMICOLON NEEDED?? 
namespace Utils
{
		 void createMap(std::map<std::string, char> *um);
		 const char* hex_char_to_bin(char c);
		 std::string BinToHex(std::string bin);
};

