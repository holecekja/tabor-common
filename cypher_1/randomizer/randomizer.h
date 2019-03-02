#pragma once

namespace cypher { namespace randomizer {

	namespace detail
	{
		struct random_cypher
		{
			boost::function<std::string(int)> generator;
			int difficulty;
		};
	}

	std::vector<boost::function<std::string()> > get_random_cyphers (int difficulty, int num_cyphers, bool strict);

	bool handle_random_cyphers ();
}} // namespace cypher::randomizer //