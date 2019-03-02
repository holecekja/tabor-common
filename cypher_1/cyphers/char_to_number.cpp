DEFINE_TEXT_CYPHER(char2number, "char2number; Converts characters to numbers;"
	                            "int start=0 #the starting offset;"
	                            "int interpunct_as=0 #what to translate the interpunction characters as;"
	                            "int space_as=0 #what to translate whitespace characters as;"
)
{
	int start = parameters.get<int>("start");
	int interpunct = parameters.get<int>("interpunct_as");
	int space = parameters.get<int>("space_as");
	
	std::ostringstream os;

	BOOST_FOREACH(char c, boost::to_lower_copy(source)) {
		if (isalpha(c)) {
			os << (int)(c - 'a' + 1) + start;
		} else if (ispunct(c)) {
			os << interpunct;
		} else if (isspace(c)) {
			os << space;
		} else {
			throw std::runtime_error("Can't apply the cypher, unsupported character found '" + std::string(1, c) + "'");
		}

		os << " ";
	}

	return os.str();
}