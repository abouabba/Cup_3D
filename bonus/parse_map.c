/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 01:55:13 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/23 11:33:58 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int	get_max_map_width(t_line *lines)
{
	int	max;
	int	len;

	max = 0;
	while (lines)
	{
		len = ft_strlen(lines->line);
		if (len > max)
			max = len;
		lines = lines->next;
	}
	return (max);
}

int	get_map_height(t_line *lines)
{
	int	count;

	count = 0;
	while (lines)
	{
		count++;
		lines = lines->next;
	}
	return (count);
}

void	print_map(char **map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		printf("%s", map[i]);
		i++;
	}
}

void	finalize_map(t_game *game)
{
	int		i;
	t_line	*current;

	game->map_height = get_map_height(game->map_lines);
	game->map_width = get_max_map_width(game->map_lines);
	game->map = ft_malloc(sizeof(char *) * (game->map_height + 1), 1);
	(1) && (current = game->map_lines, i = 0);
	while (current)
	{
		game->map[i] = ft_malloc(game->map_width + 1, 1);
		if (!game->map[i])
		{
			perror ("game->map[i] : error");
			exit(1);
		}
		ft_memset(game->map[i], ' ', game->map_width);
		ft_memcpy(game->map[i], current->line, ft_strlen(current->line));
		game->map[i][game->map_width] = '\0';
		current = current->next;
		i++;
	}
	game->map[i] = NULL;
}
