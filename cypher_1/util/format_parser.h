#pragma once

#include "cypher_parameters.h"
#include "cypher_definition.h"

namespace cypher { namespace utility {

/*
	cypher_name_1(param1=12).cypher2.cypher3().cypher2(param4=Hello)
*/

namespace detail
{
	struct parsed_cypher
	{
		cypher_definition definition;
		parameter_pack parameters;
	};
} // namespace detail //

struct format_parser
{
	typedef std::vector<detail::parsed_cypher> parsed_cyphers_container;
	typedef parsed_cyphers_container::const_iterator const_iterator;

	void parse (std::istream & in);

    bool needs_binary_output () const;

	const_iterator begin () const;
	const_iterator end () const;

private:
	parsed_cyphers_container parsed_;
};

}} // namespace cypher::utility //