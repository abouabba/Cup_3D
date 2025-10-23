/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 15:42:21 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/18 15:42:22 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t		len_s;
	size_t		i;
	char		*s1;

	if (!s)
		return (NULL);
	len_s = ft_strlen(s);
	if (start >= len_s || (len == 0))
		return (ft_strdup(""));
	i = 0;
	if (len > len_s - start)
		len = len_s - start;
	s1 = ft_malloc((len + 1) * sizeof(char), 1);
	if (!s1)
		return (NULL);
	while (i < len)
	{
		s1[i] = s[i + start];
		i++;
	}
	s1[i] = '\0';
	return (s1);
}

static int	counter_word(char const *s, char dilimiter)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != dilimiter && (i == 0 || s[i - 1] == dilimiter))
			j++;
		i++;
	}
	return (j);
}

// static void	ft_free(char **arry)
// {
// 	int	i;

// 	i = 0;
// 	while (arry[i])
// 	{
// 		// free (arry[i]);
// 		i++;
// 	}
// 	// free (arry);
// }

static char	**split(const char *s, char **big_arry, char c)
{
	int		i;
	int		start;
	int		len;

	i = 0;
	start = 0;
	while (s[i])
	{
		len = 0;
		while (s[i] == c)
			i++;
		while (s[i + len] != c && s[i + len])
			len++;
		if (len > 0)
		{
			big_arry[start] = ft_substr(s, i, len);
			if (!big_arry[start++])
				return ( NULL);//ft_free (big_arry),
		}
		i += len;
	}
	big_arry[start] = NULL;
	return (big_arry);
}

char	**ft_split(char const *s, char c)
{
	char	**big_arry;

	if (!s)
		return (NULL);
	big_arry = (char **)ft_malloc((counter_word(s, c) + 1) * sizeof(char *), 1);
	if (!big_arry)
		return (NULL);
	big_arry = split(s, big_arry, c);
	return (big_arry);
}
