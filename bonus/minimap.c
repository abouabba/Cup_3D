/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 12:24:06 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/09 16:23:57 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	draw_square(t_game *game, int x, int y, int size, int color)
{
	int i;
	int j;

	i = 0;
	while(i < size)
	{
		j = 0;
		while(j < size)
		{
			my_mlx_pixel_put(game, x + j, y + i, color);
			j++;
		}
		i++;
	}
}

void	draw_minimap(t_game *game)
{
	int map_x, map_y;
	int start_x = game->player.x - (MINIMAP_SIZE / (2 * MINIMAP_TILE_SIZE));
	int start_y = game->player.y - (MINIMAP_SIZE / (2 * MINIMAP_TILE_SIZE));
	int end_x = game->player.x + (MINIMAP_SIZE / (2 * MINIMAP_TILE_SIZE));
	int end_y = game->player.y + (MINIMAP_SIZE / (2 * MINIMAP_TILE_SIZE));
	int color;
	map_y = start_y;
	while (map_y < end_y)
	{
		++map_y;
		map_x = start_x;
		while (map_x < end_x)
		{
			++map_x;
			if (map_y < 0 || map_y >= game->map_height ||
				map_x < 0 || map_x >= game->map_width)
				continue;

			char tile = game->map[map_y][map_x];
			if (tile == '1')
				color = 0x800000;
			else
				color = 0x000000;

			draw_square(game,
				MINIMAP_OFFSET_X + (map_x - start_x) * MINIMAP_TILE_SIZE,
				MINIMAP_OFFSET_Y + (map_y - start_y) * MINIMAP_TILE_SIZE,
				MINIMAP_TILE_SIZE,
				color);
		}
	}

	//  Player always in center
	int player_screen_x = MINIMAP_OFFSET_X + (MINIMAP_SIZE / 2);
	int player_screen_y = MINIMAP_OFFSET_Y + (MINIMAP_SIZE / 2);

	draw_square(game,
		player_screen_x - 2,
		player_screen_y - 2,
		4,
		0x00FF00);
}
