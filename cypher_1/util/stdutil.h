#pragma once

namespace cypher { namespace utility { namespace {

	template <class K, class V, class C, class A>
	std::vector<K> get_all_map_keys(const std::map<K, V, C, A> & map)
	{
		std::vector<K> keys;

		BOOST_FOREACH(auto& value, map) {
			keys.push_back(value.first);
		}

		return keys;
	}

	std::string format_with_indent(std::string text, unsigned indent)
	{
		const unsigned CONSOLE_WIDTH = 80;

		boost::replace_all(text, "\t", "  ");

		std::string indent_string (indent, ' ');

		unsigned pos = indent;
		std::ostringstream os;
		os << indent_string;
		
		bool skipLeadingWhiteSpaces = true;
		for (auto it = text.begin(); it != text.end(); ++it) {
			if (*it == '\n') {
				os << "\n";
				pos = indent;
				skipLeadingWhiteSpaces = true;
			} else {
				if (skipLeadingWhiteSpaces && isspace(*it))
					continue;

				skipLeadingWhiteSpaces = false;

				os << *it;
				++pos;
				if (pos == CONSOLE_WIDTH) {
					os << indent_string;
					pos = indent;
					skipLeadingWhiteSpaces = true;
				}
			}
		}

		return os.str();
	}

	std::string squish_whitespaces (const std::string & input)
	{
		std::string result;
		bool last_space = false;
		BOOST_FOREACH(char c, input) {
			if (isspace(c)) {
				if (last_space) {
					continue;
				} else {
					last_space = true;
					result += ' ';
				}
			} else {
				last_space = false;
				result += c;
			}
		}

		return result;
	}

	struct nil_streambuf : public std::streambuf
	{
	private:
		int sputc ( char c ) { 
			return c;
		}
		std::streamsize sputn ( const char *, std::streamsize n ) {
			return n;
		}
	};

}}} // namespace cypher::utility::{} //