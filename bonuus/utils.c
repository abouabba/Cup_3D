#include "cub.h"

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*s2;

	i = 0;
	s2 = malloc((ft_strlen(s1) + 1) * (sizeof(char)));
	if (!s2)
		return (NULL);
	while (s1[i])
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}

char	*ft_strchr(char *s, int c)
{
	int	i;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)s + i);
		i++;
	}
	if ((char)c == '\0')
		return ((char *)s + i);
	return (NULL);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*s_everyone;

	i = 0;
	j = 0;
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	s_everyone = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!s_everyone)
	{
		free(s1);
		return (NULL);
	}
	while (s1[i])
	{
		s_everyone[i] = s1[i];
		i++;
	}
	while (s2[j])
		s_everyone[i++] = s2[j++];
	s_everyone[i] = '\0';
	return (free(s1), s_everyone);
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_overflow(int sing)
{
	if (sing == 1)
		return (-1);   
	return (0);
}

int	ft_atoi(const char	*str)
{
	int				i;
	unsigned long	n;
	int				a;

	i = 0;
	n = 0;
	a = 1;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			a = -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		if (n > (9223372036854775807UL - (str[i] - 48)) / 10)
			return (ft_overflow(a));
		n = n * 10 + (str[i] - 48);
		i++;
	}
	return (n * a);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] || s2[i]))
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}
