#pragma once

namespace cypher { namespace utility {

namespace detail
{
	struct option
	{
		std::string name;
		std::string value;

		inline option ()
		{}

		inline option (const std::string & _name, const std::string & _value)
			: name(_name), value(_value)
		{}

		inline operator bool () const
		{
			return !name.empty();
		}

		template <typename T>
		T as () const
		{
			if (*this) {
				try {
					return boost::lexical_cast<T>(value);
				} catch (const std::exception & ex) {
					throw std::runtime_error("Option '" + name + "' has an invalid value (" + ex.what() + ")");
				}
			} else {
				throw std::runtime_error("Option '" + name + "' must be specified.");
			}
		}

		template <typename T>
		T as (const T & default_value) const
		{
			if (*this)
				return as<T>();
			else
				return default_value;
		}
	};
} // namespace detail //

struct options
{
	inline options ()
	{}

	inline options (int argc, char ** argv)
	{
		for (int i = 1; i != argc; ++i) {
			const char * arg = argv[i];
			if (*arg == '-') {
				++arg;

				if (*arg == '-') {
					// long option name
					++arg;

					if (*arg != 0) {
						std::string name = arg;

						if ((i + 1) != argc && *(argv[i + 1]) != 0 && *(argv[i + 1]) != '-') {
							++i;
							add(name, argv[i]);
						} else {
							add(name);
						}
					}
				} else if (*arg != 0) {
					// short option name
					char name = *arg;
				
					++arg;
					if (*arg != 0) {
						add(std::string(1, name), arg);
					} else {
						add(std::string(1, name));
					}
				}
			}
		}
	}

	inline detail::option get (char name) const
	{
		return get (std::string(1, name));
	}

	inline detail::option get (const std::string & name) const
	{
		auto it = options_.find(name);
		if (it != options_.end())
			return detail::option(it->first, it->second);
		else
			return detail::option();
	}

	inline detail::option get (char short_name, const std::string & long_name) const
	{
		if (auto s = get(short_name))
			return s;
		else
			return get(long_name);
	}

	inline void DEBUG_print_all () const
	{
		for (auto it = options_.begin(); it != options_.end(); ++it) {
			std::clog << it->first << " : " << it->second << "\n";
		}
	}

private:

	inline void add (const std::string & name, const std::string & value = "")
	{
		options_.insert(std::make_pair(name, value));
	}

	std::map<std::string, std::string> options_;
};

const options & program_options ();
void set_program_options (const options & value);

bool verbose_output ();

}} // namespace cypher::utility::{} //