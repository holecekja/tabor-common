#include <util/parsing.h>

DEFINE_TEXT_CYPHER(shift,	"shift; Shifts the alphabet;"
						    "int shift #Specifies the shift. The text is shifted to the right, meaning the shift to decode the cypher will be -shift;"
)
{
	int shift = parameters.get<int>("shift");

	std::ostringstream os;

	int count = 'z' - 'a' + 1;
	shift = shift % count;
	while (shift < 0)
		shift += count;

	BOOST_FOREACH(char c, boost::to_lower_copy(source)) {
		if (isalpha(c)) {
			//char shifted = (((c - 'a') + shift) % ('z' - 'a' + 1)) + 'a';
			int offset = static_cast<int>(c) - 'a';
			int new_offset = offset + shift;
			new_offset = new_offset % count;
			char new_char = 'a' + static_cast<char>(new_offset);

			os << new_char;
		} else {
			os << c;
		}
	}

	return os.str();
}