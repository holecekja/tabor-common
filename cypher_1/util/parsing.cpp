#pragma once

namespace cypher { namespace utility {

std::vector<std::string> split_to_words (const std::string & text)
{
	std::istringstream is (text);
	std::vector<std::string> buffer;

	std::copy(std::istream_iterator<std::string>(is), std::istream_iterator<std::string>(), std::back_inserter(buffer));
	return buffer;
}

std::string reconstruct_words (const std::vector<std::string> & words)
{
	std::ostringstream os;
	std::copy(words.begin(), words.end(), std::ostream_iterator<std::string>(os, " "));

	return os.str();
}

std::string remove_native_characters (std::string text)
{
	std::string fmt = "μθψύανισωϊος";
	std::string rep = "escrzyaieouudtn";

	BOOST_FOREACH(char & c, text) {
		std::size_t i = fmt.find(c);
		if (i != fmt.npos) {
			c = rep[i];
		} else if (c < 0) {
			c = ' ';
		}
	}

	return text;
}

std::string read_file (const boost::filesystem::path & path)
{
	std::ifstream in (path.string());
	if (!in.is_open())
		throw std::runtime_error("Can't open file '" + path.relative_path().string() + "'");

	std::ostringstream os;
	os << in.rdbuf();

	return os.str();
}

void trim_interpunct (std::string & text)
{
	boost::algorithm::trim_if(text, [](char c) 
	{
		return isspace(c) || ispunct(c);
	});
}

}} // namespace cypher::utility //