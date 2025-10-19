/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 15:40:00 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/18 16:03:08 by abouabba         ###   ########.fr       */
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
		free(arry[i]);
		i++;
	}
	free(arry);
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

static void	check_commas(char *line)
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

static void	check_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		i++;
	if (i != 3)
	{
		perror("Error\n<---- Invalid color format3 ---->");
		free_split(split);
		exit(1);
	}
}

static void	validate_rgb(char **split, int r, int g, int b)
{
	if (!split[0][0] || !split[1][0] || !split[2][0]
		|| !is_digit_str(split[0]) || !is_digit_str(split[1])
		|| !is_digit_str(split[2]))
	{
		free_split(split);
		print_error("Error: RGB values must contain only digits");
	}
	if (invalid_rgb(r, g, b))
	{
		perror("Error\n<---- Invalid color format4 ---->");
		free_split(split);
		exit(1);
	}
}

int	parse_color(char *line)
{
	int		r;
	int		g;
	int		b;
	int		len;
	char	**split;

	while (*line == ' ')
		line++;
	check_commas(line);
	split = ft_split(line, ',');
	if (!split)
	{
		perror("Error\n<---- Invalid color format2 ---->");
		exit(1);
	}
	check_split(split);
	r = ft_atoi(split[0]);
	g = ft_atoi(split[1]);
	b = ft_atoi(split[2]);
	len = ft_strlen(split[2]);
	if (split[2][len - 1] == '\n')
		split[2][len - 1] = '\0';
	validate_rgb(split, r, g, b);
	free_split(split);
	return ((r << 16) | (g << 8) | b);
}

void	parse_line(t_game *game, char *line)
{
	if (is_config_line(line))
	{
		if (game->map_started)
			print_error("Error: Config line found after map started");
		parse_config_line(game, line);
	}
	else if (is_empty_line(line))
	{
		if (game->map_started)
			game->map_ended = 1;
	}
	else
	{
		if (game->map_ended)
			print_error("Error: Non-empty map line \
after empty line (split map)");
		game->map_started = 1;
		save_map_line(game, line);
	}
}

int	is_empty_line(char *line)
{
	while (*line)
	{
		if (*line != '\n')
			return (0);
		line++;
	}
	return (1);
}
