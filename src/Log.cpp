#include "Log.h"

std::string NotRecognizedToken::toString() const
{
	std::string string = "Linia ";
	string += std::to_string(line);
	string += ", poz ";
	string += std::to_string(firstCharPos);
	string += ": Nie udalo sie rozponac tokenu.";

	return string;
}

std::string MissingSemicolon::toString() const
{
	std::string string = "Linia ";
	string += std::to_string(line);
	string += ": Brakujacy srednik.";

	return string;
}
