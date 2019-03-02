#pragma once

#include "cypher_manager.h"
#include "util/cypher_definition.h"

namespace cypher {

namespace detail {

	void cypher_manager_impl::add (const utility::cypher_definition & definition)
	{
		cyphers_[definition.name()] = definition;
	}

	const utility::cypher_definition cypher_manager_impl::get (const std::string & name) const
	{
		auto it = cyphers_.find(name);
		if (it != cyphers_.end())
			return it->second;

		throw std::runtime_error("Unknown cypher '" + name + "'");
	}

	std::vector<std::string> cypher_manager_impl::get_all_cypher_names () const
	{
		return ::cypher::utility::get_all_map_keys(cyphers_);
	}

	int register_cypher (const char * definition, bool binary, ::cypher::utility::detail::cypher_function function)
	{
		::cypher::cypher_manager::Instance().add(::cypher::utility::cypher_definition(definition, binary, function));

		// dummy return value
		return 0;
	}
}

} // namespace cypher //