#include "morse_code.h"

#define ADD_CHAR(_c, _mn, _morse) \
	info.character = std::string(#_c); \
	info.mnemonic_word = #_mn; \
	info.morse_code = #_morse; \
	morse_code.push_back(info)

namespace cypher { namespace utility {

    std::vector<detail::char_info> generate_morse (bool inklude_CH_jako__CHUJ__)
	{
        std::vector<detail::char_info> morse_code;

		detail::char_info info;

		ADD_CHAR(a, ak�t, .-);
		ADD_CHAR(b, bl�skavice, -...);
		ADD_CHAR(c, c�lovn�ci, -.-.);
		ADD_CHAR(d, d�lava, -..);
		ADD_CHAR(e, erb, .);
		ADD_CHAR(f, filip�ny, ..-.);
		ADD_CHAR(g, gr�nsk� zem, --.);
		ADD_CHAR(h, hrachovina, ....);

        if (inklude_CH_jako__CHUJ__)
        {
            ADD_CHAR(ch, chl�b n�m d�v�, ----);
        }
		
        ADD_CHAR(i, ibis, ..);
		ADD_CHAR(j, jasm�n b�l�, .---);
		ADD_CHAR(k, kr�kor�, -.-);
		ADD_CHAR(l, lup�ne�ek, .-..);
		ADD_CHAR(m, m�v�, --);
		ADD_CHAR(n, n�stup, -.);
		ADD_CHAR(o, � n� p�n, ---);
		ADD_CHAR(p, pap�rn�ci, .--.);
		ADD_CHAR(q, kv�l� ork�n, --.-);
		ADD_CHAR(r, rar�ek, .-.);
		ADD_CHAR(s, sekera, ...);
		ADD_CHAR(t, tr�n, -);
		ADD_CHAR(u, u�en�, ..-);
		ADD_CHAR(v, vyu�en�, ...-);
		ADD_CHAR(w, wag�n kl�d, .--);
		ADD_CHAR(x, x�nie m�, -..-);
		ADD_CHAR(y, � se kr�t�, -.--);
		ADD_CHAR(z, zn�m� �ena, --..);

		ADD_CHAR(1, , .----);
		ADD_CHAR(2, , ..---);
		ADD_CHAR(3, , ...--);
		ADD_CHAR(4, , ....-);
		ADD_CHAR(5, , .....);
		ADD_CHAR(6, , -....);
		ADD_CHAR(7, , --...);
		ADD_CHAR(8, , ---..);
		ADD_CHAR(9, , ----.);
		ADD_CHAR(0, , -----);

        // Sort the characters first by their size (larger chars first)
        std::sort(morse_code.begin(), morse_code.end(), [](auto& lhs, auto& rhs)
        {
            return lhs.character.size() > rhs.character.size();
        });

		return morse_code;
	}

	detail::char_info morse_info_for_first_character(const char* c, const std::vector<detail::char_info>& morse)
	{
        for (auto& info: morse)
        {
            if (boost::istarts_with(c, info.character))
            {
                return info;
            }
        }

	    throw std::runtime_error("Can't translate text '" + std::string(c) + "' to the morse code");
	}

	std::string convert_string_to_morse(const std::string & input, const std::vector<detail::char_info>& morse)
	{
		std::ostringstream os;

		std::string text_to_translate = squish_whitespaces(input);

	    for (const char* next = text_to_translate.c_str(); next < text_to_translate.c_str() + text_to_translate.size();) {
            unsigned advance = 1;
			if (isspace(*next)) {
				os << "|";
			} else if (*next == '.') {
				os << "|";
			} else if (ispunct(*next)) {
				// ignore
			} else {
				auto char_info = morse_info_for_first_character(next, morse);
				os << char_info.morse_code << "|"; 
                advance = char_info.character.size();
			}

            next += advance;
		}

		return os.str();
	}

}} // namespace cypher::utility //