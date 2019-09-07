#include <libft.h>

size_t	ft_strlcpy(char *s1, char const *s2, size_t n)
{
	*s1 = '\0';
	return (ft_strlcat(s1, s2, n));
}
