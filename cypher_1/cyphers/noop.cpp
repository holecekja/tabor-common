DEFINE_TEXT_CYPHER(noop, "noop; This cypher doesn't do anything.;")
{
	(void)parameters;
	return source;
}