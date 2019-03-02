#include "the_boss.h"
#include "randomizer/randomizer.h"

#include <juce_graphics/juce_graphics.h>

#include <sstream>

std::string get_format ()
{
	// format can be passed in as a command line option, as a first line of input, from a file or interactively
	
	auto & options = cypher::utility::program_options();
	if (auto fmt = options.get('f', "format")) {
		// as a command line option
		return fmt.value;
	} else if (options.get('i', "interactive")) {
		// interactive
		std::cout << "Format: ";

		std::string format;
		getline(std::cin, format);
		return format;
	} else if (auto file = options.get("format-file")) {
		// in an external file
		std::ifstream fmtFile (file.value);
        fmtFile >> std::noskipws;

		std::ostringstream os;
		os << fmtFile.rdbuf();
		return os.str();
	} else {
		// first line of the std input
		std::string format;
		getline(std::cin, format);
		return format;
	}
}

std::string get_source ()
{
	// source text can be passed in as a command line option, as standard input, from a file or interactively

	auto & options = cypher::utility::program_options();
	if (auto src = options.get('s', "source")) {
		// as a command line option
		return src.value;
	} else if (options.get('i', "interactive")) {
		// interactive
		std::cout << "Source (empty line to end): ";

		std::string source;
		for (;;) {
			std::string line;
			getline(std::cin, line);

			if (line.empty())
				break;

			if (!source.empty())
				source.append("\n");

			source.append(line);
		}
		return source;
	} else if (auto file = options.get("source-file")) {
		// in an external file
		std::ifstream source (file.value);
		source >> std::noskipws;

		std::ostringstream os;
		os << source.rdbuf();
		return os.str();
	} else {
		// from std input
		std::ostringstream os;
		os << std::cin.rdbuf();
		return os.str();
	}
}

void show_help ()
{
	auto all_cyphers = cypher::cypher_manager::Instance().get_all_cypher_names();

	std::clog << "Cypher generator v." << APP_VERSION << "\n\n"
	<< "Command line switches:\n" <<
	"  -i, --interactive\n    Starts the program in the interactive mode.\n\n"
	"  -f, --format\n    Specifies the cypher format.\n\n"
	"  --format-file\n    Specifies the name of the file to read the format from.\n\n"
	"  -s, --source\n    Specifies the input text.\n\n"
	"  --source-file\n    Specifies the name of the file to read the input text from.\n\n"
	"  -o, --output-file\n    Specifies the name of the file to put the result to.\n\n"
	"  -S, --silent\n    Disables most of the program output and leaves only the resulting cypher.\n\n"
	"  -v, --verbose\n    Writes out additional information about the conversion process.\n"
	"    This option is useful to debug your format expressions in order to\n"
	"    see what is going on.\n\n"
	"  --random-level\n    Enables the random cypher generation. Takes an optional difficulty\n"
	"    level 1-4, where 1 is the easiest and 4 is the most difficult.\n"
	"    If this option is used, one of the input options below must be specified.\n\n"
	"  --random-input-file\n    Specifies the input file for the random mode.\n\n"
	"  --random-input-dir\n    Specifies a directory with the files to apply the random cyphers to.\n\n"
	"  --random-output-dir\n    Specifies a directory where to put the generated files.\n\n"
	"  --random-merge-into\n    Specifies a file where all the resulting cyphers will be copied to.\n\n"
	"  --random-strict\n    If specified, the program refuses to generate cyphers if there isn't enough\n"
	"    distinct cypher types.\n\n"
	"  --random-print\n    If specified and the --random-merge-into option is also specified,\n"
	"    the result will be automatically printed to the default printer.\n\n"
	"\n\n"
	"If no input option is specified (including --interactive), the format is read as the first "
	"line of the standard input and the rest of the standard input is interpreted as the input text\n\n"
	"In order to chain more than one cypher together, use a . syntax.\n"
	"for example to first shift letters in a word by one and then translate\n"
	"the result to a morse code, use the following as the format:\n"
	"   shift(SHIFT=1).morse\n";



	std::clog << "\nAvailable cyphers:\n\n";

	BOOST_FOREACH(const auto & cypher_name, all_cyphers) {
		auto & cypher_definition = cypher::cypher_manager::Instance().get(cypher_name);

		std::clog << "  " << boost::to_upper_copy(cypher_definition.name()) << "\n";
		std::clog << cypher::utility::format_with_indent(cypher_definition.comment(), 4);

		auto parameter_names = cypher_definition.get_all_parameter_names();
		if (!parameter_names.empty()) {
			std::clog << "\n\n    Parameters:\n\n";

			BOOST_FOREACH(const auto & parameter_name, parameter_names) {
				auto & parameter_definition = cypher_definition.find_parameter_definition(parameter_name);

				std::clog	<< "      " << cypher::utility::PARAMETER_TYPE::name(parameter_definition.type) 
							<< " " << boost::to_upper_copy(parameter_definition.name);
				if (parameter_definition.optional) {
					std::clog << " (default value = " << parameter_definition.default_value << ")";
				} else {
					std::clog << " (required)";
				}

				std::clog << "\n";
				
				std::clog << cypher::utility::format_with_indent(parameter_definition.comment, 8);
				std::clog << "\n\n";
			}
		} else {
			std::clog << "\n\n";
		}

		std::clog << "\n  " << std::setw(78) << std::setfill('=') << "" << std::setfill(' ') << "\n";
	}
}

int main(int argc, char ** argv)
{
	srand((unsigned)time(0));

    juce::initialiseJuce_GUI();

	try {
		cypher::utility::options options (argc, argv);
		cypher::utility::set_program_options(options);

		if (cypher::utility::program_options().get("debug")) {
			std::clog << "\nDEBUG - All command line parameters:\n";
			cypher::utility::program_options().DEBUG_print_all();
			std::clog << "\n";
		}

		if (cypher::utility::program_options().get('S', "silent")) {
			std::clog.rdbuf(new cypher::utility::nil_streambuf());
		}

		if (cypher::utility::program_options().get('h', "help") || cypher::utility::program_options().get('?')) {
			show_help();
		} else {

			if (!cypher::randomizer::handle_random_cyphers()) {
				std::string format = get_format();
				std::string source = get_source();

                std::istringstream format_stream(format);
                
                cypher::utility::format_parser parsed_format;
                parsed_format.parse(format_stream);

				std::clog << "Converting text \"" << source << "\"\nWith format: " << format << std::endl;

                std::ios::openmode outputMode = std::ios::out;

                if (parsed_format.needs_binary_output())
                    outputMode |= std::ios::binary;

                std::ostringstream os (outputMode);

				cypher::boss my_boss;
				my_boss.make_it_work(source, os, parsed_format);

                // Echo
                if (!parsed_format.needs_binary_output())
                {
                    std::cout << os.str();
                }

                if (auto param = cypher::utility::program_options().get('o', "output-file")) {
                    std::ofstream of (param.value, outputMode | std::ios::trunc);
                    if (!of.is_open()) {
                        std::cerr << "\nError opening the output file!\n";
                    } else {
                        of << os.str();
                    }
                }
			}
		}
	}
	catch (const std::exception & ex)
	{
		std::cerr << "Error: " << ex.what() << std::endl;
	}

	if (cypher::utility::program_options().get('i', "interactive"))
		std::cin.get();

    juce::shutdownJuce_GUI();

	return 0;
}