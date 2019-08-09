char				digit(char input)
{
	if (input >= 0x30 && input <= 0x39)
		return (input);
	else
		return (0);
}
