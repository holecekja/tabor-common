#pragma once

namespace cypher { namespace utility {

	void do_print_file (const boost::filesystem::path & file)
	{
		ShellExecuteA(
			0,
			"print",
			file.string().c_str(),
			nullptr,
			nullptr,
			SW_SHOWNA
		);
	}

	void print_file (const boost::filesystem::path & file)
	{
		boost::filesystem::path txt_file = file.parent_path() / "to_print.txt";

		boost::filesystem::copy_file(file, txt_file, boost::filesystem::copy_option::overwrite_if_exists);

		do_print_file(txt_file);
	}

}} // namespace cypher::utility //