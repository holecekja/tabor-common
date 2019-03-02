#pragma once

namespace cypher { namespace utility {

	namespace detail
	{
		struct char_info
		{
			std::string character;
			std::string morse_code;
			std::string mnemonic_word;
		};
	}


    std::vector<detail::char_info> generate_morse (bool inklude_CH_jako__CHUJ__);

	detail::char_info morse_info_for_first_character(const char* c, const std::vector<detail::char_info>& morse);

	std::string convert_string_to_morse(const std::string & input, const std::vector<detail::char_info>& morse);

}} // namespace cypher::utility //