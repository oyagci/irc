char				letter(char input)
{
	if ((input >= 0x41 && input <= 0x5a) ||
		(input >= 0x61 && input <= 0x7a))
		return (input);
	else
		return (0);
}
