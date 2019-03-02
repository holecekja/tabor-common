#pragma once

#include "util/cypher_definition.h"
#include "util/cypher_parameters.h"

namespace cypher {

namespace detail {

	struct cypher_manager_impl
	{
		void add (const utility::cypher_definition & definition);

		const utility::cypher_definition get (const std::string & name) const;

		std::vector<std::string> get_all_cypher_names () const;
	private:
		std::map<std::string, utility::cypher_definition> cyphers_;
	};

	int register_cypher (const char * definition, bool binary, ::cypher::utility::detail::cypher_function function);
}

typedef Loki::SingletonHolder<detail::cypher_manager_impl> cypher_manager;

} // namespace cypher //

#define DEFINE_CYPHER(name, definition, binary) \
	std::string cypher_function_definition_##name (const std::string &, const ::cypher::utility::parameter_pack & parameters); \
	int cypher_function_definition_registrator_##name = ::cypher::detail::register_cypher(definition, binary, cypher_function_definition_##name); \
	std::string cypher_function_definition_##name (const std::string & source, const ::cypher::utility::parameter_pack & parameters)

#define DEFINE_TEXT_CYPHER(name, definition) DEFINE_CYPHER(name, definition, false)
#define DEFINE_BINARY_CYPHER(name, definition) DEFINE_CYPHER(name, definition, true)