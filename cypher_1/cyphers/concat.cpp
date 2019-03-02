#include <util/parsing.h>

DEFINE_TEXT_CYPHER(concat,	"concat; Adds a text as a prefix/postfix;"
						    "string prefix = #Prefix text;"
						    "string postfix = #Postfix text;"
)
{
	std::string prefix = parameters.get<std::string>("prefix");
	std::string postfix = parameters.get<std::string>("postfix");

	return prefix + source + postfix;
}