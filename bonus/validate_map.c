/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 16:07:50 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/18 19:12:42 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	print_error(const char *message)
{
	fprintf(stderr, "Error: %s\n", message);
	exit(EXIT_FAILURE);
}

int	is_valid_char(char c)
{
	return (c == '0' || c == '1'
		|| c == 'N' || c == 'S'
		|| c == 'E' || c == 'W'
		|| c == ' ');
}

int	is_player_char(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

void	check_player(t_game *game, int c, int x, int y)
{
	int	player_count;

	player_count = 0;
	if (is_player_char(c))
	{
		player_count++;
		game->player.x = x + 0.5;
		game->player.y = y + 0.5;
		game->player.direction = c;
	}
	else if ((c == '0' || is_player_char(c)) && (y == 0 || x == 0
			|| y >= game->map_height - 1 || x >= game->map_width - 1
			|| game->map[y - 1][x] == ' ' || game->map[y + 1][x] == ' '
			|| game->map[y][x - 1] == ' '
			|| game->map[y][x + 1] == ' '))
		print_error("Map is not closed");
	if (player_count != 1)
		print_error("There must be exactly one player");
}

void	validate_map(t_game *game)
{
	char	c;
	int		x;
	int		y;
	int		player_count;

	(1) && (y = 0, player_count = 0);
	while (y < game->map_height)
	{
		x = 0;
		while (x < game->map_width)
		{
			c = game->map[y][x];
			if (!is_valid_char(c))
			{
				perror("Error: Invalid character");
				exit(EXIT_FAILURE);
			}
			else if (is_player_char(c))
				check_player(game, c, x, y);
			x++;
		}
		y++;
	}
}
