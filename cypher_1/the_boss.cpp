#pragma once

#include "the_boss.h"

namespace cypher {

	void boss::make_it_work (const std::string & source, std::ostream & out, const utility::format_parser& format)
	{
		bool verbose = utility::program_options().get('v', "verbose");

		std::string buffer = utility::remove_native_characters(source);
		for(auto it = format.begin(); it != format.end(); ++it) {

			if (verbose) {
				std::clog << "\nApplying cypher '" << it->definition.name() << "' to the text:\n" << buffer << "\n";
			}

			buffer = it->definition.function()(buffer, it->parameters);

			if (verbose) {
				std::clog << "Result:\n" << buffer << "\n";
			}
		}

		if (verbose) {
			std::clog << "\n\n";

			if (utility::program_options().get('v', "verbose").value == "boss") {
				std::clog << "I'm the boss...\n\n";
			}
		}

		out << buffer;
	}

} // namespace cypher //