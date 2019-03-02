#include <util/parsing.h>

DEFINE_TEXT_CYPHER(wshuffle,	"wshuffle; Randomly shuffles the characters in each word;"
						        "bool keep_ends= 0 #If enabled, keeps the starting and the ending character in place;"
						        "int keep_limit= 0 #The minimum word size where the keep_ends option is enabled;"
						        "bool strict= 1 #If enabled, all the characters are guaranteed to be shuffled;"
)
{
	bool keep_ends = parameters.get<bool>("keep_ends");
	int keep_limit = parameters.get<int>("keep_limit");
	bool strict = parameters.get<bool>("strict");

	auto words = cypher::utility::split_to_words(source);

	BOOST_FOREACH(std::string & word, words) {
		// Find the left and right ends
		auto first = word.begin();
		auto last = word.end() - 1;

		while (first != last && !isalnum(*first)) {
			++first;
		}

		while (first != last && !isalnum(*last)) {
			--last;
		}

		int size = last - first;
		if (keep_ends && keep_limit <= size) {

			if (size > 2) {
				++first;
				--last;
			} else {
				// Do not shuffle words of 1 or 2 in length
				continue;
			}
		}

		unsigned first_index = first - word.begin();
		std::string original = word.substr(first_index, last-first + 1);

        // Create indexes into the string (just 0..originalSize)
        std::vector<std::size_t> indexes;
        std::generate_n(std::back_inserter(indexes), original.size(), [i = 0U]() mutable { return i++; });

		for (;;) {
            // Shuffle the indexes
            std::random_shuffle(indexes.begin(), indexes.end());

            // If we use the strict shuffling, make sure no index is in it's original position
            bool needsAnotherPass = false;
            if (strict && indexes.size() > 1)
            {
                for (auto i = 0U; i != indexes.size(); ++i) {
                    // If this index is in it's original position, try again
                    if (indexes[i] == i) {
                        needsAnotherPass = true;
                        break;
                    }
                }
            }

            if (!needsAnotherPass)
                break;
		}

        // Update the word
        for (auto i = 0U; i != indexes.size(); ++i) {
            word[first_index + i] = original[indexes[i]];
        }
	}

	return cypher::utility::reconstruct_words(words);
}