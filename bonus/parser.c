/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 15:40:00 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/23 11:57:41 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static void	check_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
		i++;
	if (i != 3)
	{
		perror("Error\n<---- Invalid color format3 ---->");
		exit(1);
	}
}

static void	validate_rgb(char **split, int r, int g, int b)
{
	if (!split[0][0] || !split[1][0] || !split[2][0]
		|| !is_digit_str(split[0]) || !is_digit_str(split[1])
		|| !is_digit_str(split[2]))
	{
		print_error("Error: RGB values must contain only digits");
	}
	if (invalid_rgb(r, g, b))
	{
		perror("Error\n<---- Invalid color format4 ---->");
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
