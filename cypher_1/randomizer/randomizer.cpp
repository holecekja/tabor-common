#include "randomizer.h"
#include <the_boss.h>
#include <util/printer.h>

namespace cypher { namespace randomizer {

	typedef Loki::SingletonHolder<std::multimap<int, detail::random_cypher>> Cyphers;

	void add_cypher(int min_difficulty, boost::function<std::string(int)> function)
	{
		(void)min_difficulty, (void)function;
		detail::random_cypher cypher;
		cypher.difficulty = min_difficulty;
		cypher.generator = function;

		Cyphers::Instance().insert(std::make_pair(min_difficulty, cypher));
	}

	bool chance (int ch)
	{
		return (rand() % 100 + 1) <= ch;
	}

	int generate_cyphers ()
	{
		// Very Easy
		add_cypher(1, [](int diff) -> std::string
		{
			if (diff > 2 && chance(50))
				return "morse(reversed=1)";
			else
				return "morse";
		});

		add_cypher(1, [](int diff) -> std::string
		{
			std::ostringstream os;

			if (diff == 4 && chance(40)) {
				int start = rand() % 100 + 100;
				os << "char2number(start=" << start << ").concat(prefix=Start +" << start << "\\s)";
			} else if (diff >= 3 && chance(30)) {
				int space = rand() % 100 + 100;
				os << "char2number(space_as=" << space << ")";
			} else {
				os << "char2number";
			}

			return os.str();
		});

		add_cypher(1, [](int diff) -> std::string
		{
			if (diff == 1) {
				return "wshuffle(strict=0)";
			} else {
				return "wshuffle";
			}
		});

		add_cypher(1, [](int diff) -> std::string
		{
			std::ostringstream os;
			int shift = 1;

			if (diff == 4 && chance(50)) {
				shift = rand() % 100 - 50;
			} else if (diff >= 3 && chance(50)) {
				shift = rand() % 26 - 13;
			} else if (diff >= 2) {
				shift = rand() % 26;
			}

			if (shift == 0)
				shift = 1;

			os << "shift(shift=" << -shift << ").concat(prefix=" << (shift > 0 ? "Plus" : "Minus") << " " << abs(shift) << ":\\s)";
			return os.str();
		});

		// Easy

		// Medium
		add_cypher(3, [](int) -> std::string
		{
			std::ostringstream os;

			int shift = rand() % 26 + 1;
			
			os << "shift(shift=" << -shift << ").concat(prefix=Plus " << shift << ":\\s).morse";

			return os.str();
		});

		// Difficult

			//ahoooooooooooooooooooooj pavoooooooooooooooooooooouuuuuuuukkuuuuuuuuuuuuuuuuuuuuuuuuuuuuu
			//-
			//nazdaaaaaaaaaaaaaarrrrrr jiiiiiiiiiiiiiiiiiiiiiiiiiiirkoooooooooooooooooooooooooooooooooo

		// HUA!

		return 0;
	}

	int _dummy_init_cyphers = generate_cyphers();

	std::vector<boost::function<std::string()> > get_random_cyphers (int difficulty, int num_cyphers, bool strict)
	{
		if (difficulty < 1 || difficulty > 4)
			throw std::invalid_argument("Invalid difficulty level!");

		std::vector<boost::function<std::string()> > matching_cyphers;
		for (auto it = Cyphers::Instance().begin(), e = Cyphers::Instance().upper_bound(difficulty); it != e; ++it) {
			matching_cyphers.push_back(std::bind(it->second.generator, difficulty));
		}

		if (((int)matching_cyphers.size() < num_cyphers && strict) || matching_cyphers.empty()) {
			throw std::runtime_error("Not enough distinct cyphers available.");
		}

		std::vector<boost::function<std::string()> > result;
		std::random_shuffle(matching_cyphers.begin(), matching_cyphers.end());

		unsigned i = 0;
		while (num_cyphers-- > 0) {
			result.push_back(matching_cyphers[i++ % matching_cyphers.size()]);
		}

		return result;
	}

	std::vector<boost::filesystem::path> get_input_files ()
	{
		std::vector<boost::filesystem::path> files;

		if (auto file = utility::program_options().get("random-input-file")) {
			files.push_back(file.value);
		}

		if (auto dir = utility::program_options().get("random-input-dir")) {
			auto dir_path = boost::filesystem::path(dir.value);
			auto dir_iterator = boost::filesystem::directory_iterator(dir_path);
			for(auto file = boost::filesystem::directory_iterator(dir_path); file != boost::filesystem::directory_iterator(); ++file) {
				files.push_back(file->path());
			}
		}

		if (utility::program_options().get("random-lines-file")) {
			auto dir_path = boost::filesystem::path("temp_lines_dir");
			auto dir_iterator = boost::filesystem::directory_iterator(dir_path);
			for(auto file = boost::filesystem::directory_iterator(dir_path); file != boost::filesystem::directory_iterator(); ++file) {
				files.push_back(file->path());
			}
		}
			//ahoooooooooooooooooooooj pavoooooooooooooooooooooouuuuuuuukkuuuuuuuuuuuuuuuuuuuuuuuuuuuuu

		return files;
	}

	std::vector<boost::filesystem::path> get_output_files (const std::vector<boost::filesystem::path> & input_files)
	{
		std::vector<boost::filesystem::path> files;

		if (auto dir = utility::program_options().get("random-output-dir")) {
			boost::filesystem::path dir_path (dir.value);
			boost::filesystem::create_directories(dir_path);

			BOOST_FOREACH(auto file, input_files) {
				files.push_back(dir_path / file.leaf());
			}
		} else if (auto file = utility::program_options().get('o', "output-file")) {
			files.push_back(file.value);
		}

		if (files.size() != 0 && files.size() != input_files.size()) {
			throw std::runtime_error("--output-file can't be used when --random-input-dir is used");
		}

		return files;
	}
			//ahoooooooooooooooooooooj pavoooooooooooooooooooooouuuuuuuukkuuuuuuuuuuuuuuuuuuuuuuuuuuuuu

	void generate_random_input_files ()
	{
		if (auto lines = utility::program_options().get("random-lines-file")) {
			int num_sentences = utility::program_options().get("random-lines-count").as<int>(1);
			int min_length = utility::program_options().get("random-lines-min").as<int>(20);
			int max_length = utility::program_options().get("random-lines-max").as<int>(50);
			char delimiter = utility::program_options().get("random-lines-delim").as<char>('.');

			if (num_sentences <= 0 || min_length <= 0 || max_length <= 0)
				throw std::runtime_error("Invalid random lines parameters");

			std::string all_lines = utility::read_file(lines.value);

			all_lines = utility::remove_native_characters(all_lines);

			boost::replace_all(all_lines, "\n", " ");
			boost::replace_all(all_lines, "\t", " ");

			std::vector<std::string> sentences;
			boost::split(sentences, all_lines, [delimiter](char c) { return c == delimiter; });

			if (utility::verbose_output()) {
				std::clog << "Found sentences:\n";
				BOOST_FOREACH(auto & sentence, sentences) {
					std::clog << sentence << "\n";
				}
			//ahoooooooooooooooooooooj pavoooooooooooooooooooooouuuuuuuukkuuuuuuuuuuuuuuuuuuuuuuuuuuuuu

				std::clog << "\n\n";
			}

			// Compress whitespaces
			BOOST_FOREACH(auto & sentence, sentences) {
				sentence.erase(
					std::unique(
						sentence.begin(), 
						sentence.end(), 
						[](char first, char second)
						{
							return isspace(first) && isspace(second);
						}
					),
					sentence.end()
				);

				utility::trim_interpunct(sentence);
			}
			//ahoooooooooooooooooooooj pavoooooooooooooooooooooouuuuuuuukkuuuuuuuuuuuuuuuuuuuuuuuuuuuuu

			// Remove the lines that do not meet the criteria
			sentences.erase(
				std::remove_if(
					sentences.begin(), 
					sentences.end(), 
					[min_length, max_length](const std::string & s)
					{
						return static_cast<int>(s.size()) < min_length || static_cast<int>(s.size()) > max_length;
					}
				),
				sentences.end()
			);

			if (utility::verbose_output()) {
				std::clog << "Reduced sentences:\n";
				BOOST_FOREACH(auto & sentence, sentences) {
					std::clog << sentence << "\n";
				}

				std::clog << "\n\n";
			}

			if (static_cast<int>(sentences.size()) < num_sentences) {
				throw std::runtime_error("Not enought sentences meeting the criteria to fulfill the request");
			}

			std::random_shuffle(sentences.begin(), sentences.end());

			boost::filesystem::path temp_dir ("temp_lines_dir");
			boost::filesystem::remove_all(temp_dir);
			boost::filesystem::create_directories(temp_dir);
			for (int i = 0; i != num_sentences; ++i) {
				boost::filesystem::path file_path = temp_dir / std::to_string((long long)i);
				std::ofstream of (file_path.string());
				
				if (!of.is_open())
					throw std::runtime_error("Can't generate temporary file");

				of << sentences[i];
			}
			//ahoooooooooooooooooooooj pavoooooooooooooooooooooouuuuuuuukkuuuuuuuuuuuuuuuuuuuuuuuuuuuuu

			if (utility::verbose_output()) {
				std::clog << "Generated " << num_sentences << " random sentences.\n";
			}
		}
	}

	bool handle_random_cyphers ()
	{
		generate_random_input_files();
		
		auto input_files = get_input_files();
		if (input_files.empty())
			// No request for random cyphers
			return false;

		auto output_files = get_output_files(input_files);

		int level = utility::program_options().get("random-level").as<int>(4);
		if (level < 1 || level > 4)
			throw std::runtime_error("Invalid random difficulty level. Must be 1-4.");

		bool strict = utility::program_options().get("random-strict");

		auto cyphers = get_random_cyphers(level, input_files.size(), strict);

		if (auto merge_to = utility::program_options().get("random-merge-into")) {
			std::ofstream of (merge_to.value);
			of << "\n\n";
			//ahoooooooooooooooooooooj pavoooooooooooooooooooooouuuuuuuukkuuuuuuuuuuuuuuuuuuuuuuuuuuuuu
		}

		for (unsigned i = 0; i != input_files.size(); ++i) {
			std::string cypher_definition = cyphers[i]();

			std::ifstream in (input_files[i].string());
			if (!in.is_open())
				throw std::runtime_error("Can't open input file '" + input_files[i].string() + "'");

			std::ostringstream os;
			os << in.rdbuf();
			std::string source = os.str();

            std::istringstream format_stream(cypher_definition);
            cypher::utility::format_parser parsed_format;
            parsed_format.parse(format_stream);

			std::ostringstream result_ostream;
			boss the_boss;
			the_boss.make_it_work(source, result_ostream, parsed_format);

			std::clog << "Cypher " << i+1 << ":\n" << result_ostream.str() << "\n\n";

			std::ofstream out;
			if (!output_files.empty()) {
				out.open(output_files[i].string());

				if (!out.is_open())
					throw std::runtime_error("Can't open output file '" + output_files[i].string() + "'");

				out << result_ostream.str();
			}

			if (auto merge_to = utility::program_options().get("random-merge-into")) {
				std::ofstream merge (merge_to.value, std::ios::out | std::ios::app | std::ios::ate);

				if (!merge.is_open())
					throw std::runtime_error("Can't open merge-into file '" + merge_to.value + "'");

				merge << result_ostream.str() << "\n\n";
			}
		}

		if (auto merge_to = utility::program_options().get("random-merge-into")) {
			if (utility::program_options().get("random-print"))
				utility::print_file(merge_to.value);	
		}

		return true;
	}
}} // namespace cypher::randomizer //