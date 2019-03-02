#pragma once

#include "cypher_definition.h"

namespace cypher { namespace utility {
namespace parameter_pack_impl {

namespace detail
{
	struct parameter
	{
		std::string value;

		template <class T>
		T as () const
		{
			try {
				return boost::lexical_cast<T>(value);
			}
			catch (const std::exception &) {
				throw std::runtime_error("Invalid parameter value '" + value + "'");
			}
		}
	};
} // namespace detail //

struct parameter_pack
{
	void add (const std::string & name, const std::string & value);

	bool has (const std::string & name) const;

	template <class T>
	T get (const std::string & name) const
	{
		return get(name).as<T>();
	}

	detail::parameter get (const std::string & name) const;
	detail::parameter operator[] (const std::string & name) const;

private:
	std::map<std::string, detail::parameter> parameters_;
};

void build_parameter_pack (parameter_pack & pack, const cypher_definition & definition, std::istream & in);

} // namespace cypher_parameter_impl //

using parameter_pack_impl::parameter_pack;

}} // namespace cypher::utility //