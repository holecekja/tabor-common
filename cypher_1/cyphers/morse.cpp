#include <util/parsing.h>
#include <util/morse_code.h>

DEFINE_TEXT_CYPHER(morse,	"morse; Translates the text to the morse code;"
						    "bool reversed = 0 #If enabled, uses '-' instead of '.' and vice versa;"
						    "int generate = 0 #If set to 1, writes out all the morse characters (including numbers if set to 2);"
                            "bool useCH = 1 #If enabled, the letter 'ch' is included in the alphabet. Otherwise it's encoded as 'c' + 'h';"
)
{
	bool reversed = parameters.get<bool>("reversed");
	int generate = parameters.get<int>("generate");
    bool useCH = parameters.get<bool>("useCH");

    auto morseData = cypher::utility::generate_morse(useCH);

	if (generate) {
		std::ostringstream os;
		os << std::left;

        auto put = [&os, &morseData](const std::string& s)
        {
            auto info = cypher::utility::morse_info_for_first_character(s.c_str(), morseData);
            os << std::setw(3) << s << std::setw(4) << info.morse_code << "  " << info.mnemonic_word << "\n";
        };

		for (char c = 'a'; c <= 'h'; ++c) {
            put(std::string(1, c));
		}

        if (useCH)
        {
            put("ch");
        }

        for (char c = 'i'; c <= 'z'; ++c) {
            put(std::string(1, c));
        }

		if (generate == 2) {
			os << "\n";
			for (char c = '0'; c <= '9'; ++c) {
                put(std::string(1, c));
			}
		}

		return os.str();
	} else {
		std::string morse = cypher::utility::convert_string_to_morse(source, morseData);

		while (!boost::ends_with(morse, "|||")) {
			morse += '|';
		}

		if (reversed) {
			boost::replace_all(morse, "-", "x");
			boost::replace_all(morse, ".", "-");
			boost::replace_all(morse, "x", ".");
		}

		return morse;
	}
}