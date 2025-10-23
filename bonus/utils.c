/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 11:35:58 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/23 11:58:29 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

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
	s_everyone = ft_malloc(ft_strlen(s1) + ft_strlen(s2) + 1, 1);
	if (!s_everyone)
	{
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
	return (s_everyone);
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
