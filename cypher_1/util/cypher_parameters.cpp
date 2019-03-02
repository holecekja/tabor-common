#pragma once

#include "cypher_parameters.h"

namespace cypher { namespace utility {
namespace parameter_pack_impl {

void parameter_pack::add (const std::string & name, const std::string & value)
{
	detail::parameter parameter;
	parameter.value = value;

	parameters_[boost::to_lower_copy(name)] = parameter;
}

bool parameter_pack::has (const std::string & name) const
{
	return parameters_.find(boost::to_lower_copy(name)) != parameters_.end();
}

detail::parameter parameter_pack::get (const std::string & name) const
{
	auto it = parameters_.find(boost::to_lower_copy(name));
	if (it != parameters_.end())
		return it->second;

	throw std::logic_error("Accessing an undefined parameter '" + name + "'");
}

detail::parameter parameter_pack::operator[] (const std::string & name) const
{
	return get(name);
}

void build_parameter_pack (parameter_pack & pack, const cypher_definition & definition, std::istream & in)
{
	// Remove the starting parenthesis if it's even there
	in >> std::ws;
	if (in.peek() == '(') {
		in.get();

		for (;;) {
			in >> std::ws;
			if (in.peek() == ')') {
				in.get();
				break;
			}

			// Read parameter name and value
			std::string name;
			parse_trim_until(in, name, "", [](char c){ return isalnum(c) || c == '_'; });

			in >> std::ws;
			if (in.peek() != '=')
				throw std::runtime_error("Expected '=' after a parameter name (" + name + ")");

			in.get();

			std::string value;
			parse_trim_until(in, value, "),");

			// Replace escape characters
			boost::replace_all(value, "\\s", " ");

			// Get the param definition, this will throw for unknown parameters
			definition.find_parameter_definition(name);

			pack.add(name, value);

			in >> std::ws;
			if (in.peek() == ',')
				in.get();
		}
	}

	// Make sure all the required parameters are present and add the default parameters
	auto all_parameters = definition.get_all_parameter_names();
	BOOST_FOREACH(auto& parameter_name, all_parameters) {
		if (!pack.has(parameter_name)) {
			auto parameter_definition = definition.find_parameter_definition(parameter_name);
			if (parameter_definition.optional) {
				pack.add(parameter_name, parameter_definition.default_value);
			} else {
				throw std::runtime_error("Required parameter '" + parameter_name + "' isn't specified");
			}
		}
	}
}

} // namespace cypher_parameter_impl //
}} // namespace cypher::utility //