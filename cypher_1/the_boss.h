#pragma once

#include <util/format_parser.h>

namespace cypher {

	struct boss
	{
		void make_it_work (const std::string & source, std::ostream & out, const utility::format_parser& format);
	};

} // namespace cypher //