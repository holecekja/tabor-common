#pragma once

namespace cypher { namespace utility {

/*
	Cypher is defined like this:
	name (string);
	comment (string);
	parameter1Type parameter1Name [= defaultValue] [#comment (string)];
	... (other parameters)
*/
namespace parameter_pack_impl {
	struct parameter_pack;
}

namespace PARAMETER_TYPE
{
	enum E
	{
		INT,
		STRING,
		BOOL
	};

	inline std::string name (E type)
	{
		switch (type)
		{
			case INT: return "int";
			case STRING: return "string";
			case BOOL: return "bool";
			default: return "unknown";
		}
	}
};

namespace detail
{
	struct parameter
	{
		parameter ();
		parameter (std::istream & stream);

		std::string name;
		PARAMETER_TYPE::E type;
		std::string default_value;
		std::string comment;
		bool optional;
	};

	typedef std::string (*cypher_function)(const std::string &, const parameter_pack_impl::parameter_pack &);
} // namespace detail //

struct cypher_definition
{
	cypher_definition ();
	cypher_definition (const std::string & definition, bool binary, detail::cypher_function function);

	const std::string & name () const;
	const std::string & comment () const;
	detail::cypher_function function () const;
	bool is_binary () const;

	// Throws if not found
	const detail::parameter & find_parameter_definition (const std::string & parameter_name) const;
	std::vector<std::string> get_all_parameter_names () const;
private:
	std::string name_;
	std::string comment_;
	std::map<std::string, detail::parameter> parameters_;
	detail::cypher_function function_;
    bool binary_;
};

}} // namespace cypher::utility //