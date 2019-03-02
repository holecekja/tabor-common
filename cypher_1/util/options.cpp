#include "options.h"

namespace cypher { namespace utility {

static options _program_options;

const options & program_options ()
{
	return _program_options;
}

void set_program_options (const options & value)
{
	_program_options = value;
}

bool verbose_output ()
{
	return program_options().get('v', "verbose");
}

}} // namespace cypher::utility //