char				special(char input)
{
	if ((input >= 0x5b && input <= 0x60) ||
		(input >= 0x7b && input <= 0x7d))
		return (input);
	return (0);
}
