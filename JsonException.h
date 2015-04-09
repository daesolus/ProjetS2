#include <iostream>
#include <exception>
class JsonException : public std::exception
{
public:
	JsonException() {}
	~JsonException() throw() {}
	const char* what() const throw() { return "Le fichier de configuration JSON est invalide"; }
};	