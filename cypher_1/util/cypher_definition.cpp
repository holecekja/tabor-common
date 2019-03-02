#include "cypher_definition.h"

namespace cypher { namespace utility {

namespace detail
{
	parameter::parameter()
	{}

	parameter::parameter(std::istream & stream)
	{
		stream >> std::ws;
		stream.peek();
		if (stream.eof())
			return;

		std::string type_string;
		stream >> type_string;
		boost::trim(type_string);
		boost::to_lower(type_string);

		if (type_string == "int")
			type = PARAMETER_TYPE::INT;
		else if (type_string == "string")
			type = PARAMETER_TYPE::STRING;
		else if (type_string == "bool")
			type = PARAMETER_TYPE::BOOL;
		else
			throw std::logic_error("Invalid parameter type '" + type_string + "'");

		utility::parse_trim_until(stream, name, "=;#");
        boost::to_lower(name);
			
		stream >> std::ws;
		if (stream.peek() == '=') {
			stream.get();
			stream >> std::ws;
			utility::parse_trim_until(stream, default_value, "#;");
			optional = true;
		} else {
			optional = false;
		}
		
		if (stream.peek() == '#') {
			stream.get();
			utility::parse_until(stream, comment, ";");
		}

		if (stream.peek() == ';') {
			stream.get();
		} else {
			throw std::logic_error("Invalid cypher format. ';' or '=' expected.");
		}
	}
}

cypher_definition::cypher_definition ()
{}

cypher_definition::cypher_definition (const std::string & definition, bool binary, detail::cypher_function function)
	: function_(function), binary_(binary)
{
	std::istringstream is (definition);

	// read the name and the comment
	getline(is, name_, ';');
	boost::trim(name_);

	getline(is, comment_, ';');
	boost::trim(comment_);

	for (;;)
	{
		detail::parameter param (is);
		if (is.eof())
			break;

		parameters_.insert(std::make_pair(param.name, param));
	}
}

const std::string & cypher_definition::name () const
{
	return name_;
}

const std::string & cypher_definition::comment () const
{
	return comment_;
}

detail::cypher_function cypher_definition::function () const
{
	return function_;
}

bool cypher_definition::is_binary () const
{
    return binary_;
}

const detail::parameter & cypher_definition::find_parameter_definition (const std::string & parameter_name) const
{
	auto it = parameters_.find(boost::to_lower_copy(parameter_name));
	if (it != parameters_.end())
		return it->second;

	throw std::runtime_error("Unknown parameter '" + parameter_name + "'");
}

std::vector<std::string> cypher_definition::get_all_parameter_names () const
{
	return ::cypher::utility::get_all_map_keys(parameters_);
}

}} // namespace cypher::utility::{} //