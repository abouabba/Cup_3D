/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 11:50:17 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/23 11:57:13 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int	invalid_rgb(int r, int g, int b)
{
	return (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255);
}

void	free_split(char **arry)
{
	int	i;

	i = 0;
	while (arry[i])
	{
		i++;
	}
}

int	is_digit_str(char *s)
{
	while (*s)
	{
		if (!ft_isdigit(*s))
			return (0);
		s++;
	}
	return (1);
}

void	check_commas(char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i])
	{
		if (line[i] == ',')
			count++;
		i++;
	}
	if (count != 2)
	{
		perror("Error\n<---- Invalid color format1 ---->");
		exit(1);
	}
}
