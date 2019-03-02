#pragma once

#include "format_parser.h"
#include <cypher_manager.h>

namespace cypher { namespace utility {

	void format_parser::parse (std::istream & in)
	{
		// Parse the cypher name, and then it's parameter pack
		for (;;) {
			in >> std::ws;
			if (in.eof())
				break;

			std::string cypher_name;
			parse_trim_until(in, cypher_name, "(.\n");

			detail::parsed_cypher parsed;
			parsed.definition = ::cypher::cypher_manager::Instance().get(cypher_name);

			build_parameter_pack(parsed.parameters, parsed.definition, in);

			parsed_.push_back(parsed);

			in >> std::ws;
			if (in.peek() == '.')
				in.get();
			else
				break;
		}

        // If we have more than one cypher, make sure we don't use any binary cyphers
        if (parsed_.size() > 1)
        {
            for (auto& cypher: parsed_)
            {
                if (cypher.definition.is_binary() && &cypher != &parsed_.back())
                {
                    throw std::runtime_error("Cypher '" + cypher.definition.name() + "' has a binary output and can't be used as an input for other cyphers");
                }
            }
        }
	}

    bool format_parser::needs_binary_output () const
    {
        return !parsed_.empty() && parsed_.back().definition.is_binary();
    }

	format_parser::const_iterator format_parser::begin () const
	{
		return parsed_.begin();
	}

	format_parser::const_iterator format_parser::end () const
	{
		return parsed_.end();
	}

}} // namespace cypher::utility //