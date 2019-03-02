#pragma once

namespace cypher { namespace utility {

namespace PARSING_RESULT
{
	enum E {
		DELIMITER_REACHED,
		STREAM_ERROR,
		INVALID_CHARACTER_REACHED,
	};
}

inline PARSING_RESULT::E parse_until (std::istream & in, std::string & buffer, const std::string & delims, const std::function<bool(char)> & predicate = std::function<bool(char)>())
{
	for (;;) {

		in.peek();
		if (!in.good())
			return PARSING_RESULT::STREAM_ERROR;

		const char c = static_cast<char>(in.peek());

		if (std::find(delims.begin(), delims.end(), c) != delims.end())
			return PARSING_RESULT::DELIMITER_REACHED;

		if (!predicate || predicate(c)) {
			buffer.push_back(c);
			in.get();
		} else {
			return PARSING_RESULT::INVALID_CHARACTER_REACHED;
		}
	}
}

inline PARSING_RESULT::E parse_trim_until (std::istream & in, std::string & buffer, const std::string & delims, const std::function<bool(char)> & predicate = std::function<bool(char)>())
{
	PARSING_RESULT::E result = parse_until(in, buffer, delims, predicate);
	boost::trim(buffer);

	return result;
}

std::vector<std::string> split_to_words (const std::string & text);
std::string reconstruct_words (const std::vector<std::string> & words);

std::string remove_native_characters (std::string text);
std::string read_file (const boost::filesystem::path & path);

void trim_interpunct (std::string & text);

}} // namespace cypher::utility //