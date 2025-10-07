/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 12:24:06 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/07 16:06:26 by abouabba         ###   ########.fr       */
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


void	draw_player_dir(t_game *game)
{
	int center_x = MINIMAP_OFFSET_X + MINIMAP_RADIUS * MINIMAP_SCALE;
	int center_y = MINIMAP_OFFSET_Y + MINIMAP_RADIUS * MINIMAP_SCALE;

	int end_x = center_x + cos(game->angle) * (MINIMAP_SCALE * 2);
	int end_y = center_y + sin(game->angle) * (MINIMAP_SCALE * 2);

	// draw a simple line from player center to direction
	for (int i = 0; i < 10; i++)
		my_mlx_pixel_put(game, center_x + (end_x - center_x) * i / 10,
								 center_y + (end_y - center_y) * i / 10, 0x00FF00);
}


void	draw_minimap(t_game *game)
{
	int px = (int)(game->player.x / TILE_SIZE);
	int py = (int)(game->player.y / TILE_SIZE);
	int start_x = px - MINIMAP_RADIUS;
	int end_x   = px + MINIMAP_RADIUS;
	int start_y = py - MINIMAP_RADIUS;
	int end_y   = py + MINIMAP_RADIUS;
	int y;
	int x;

	y = start_y;
	while (y <= end_y)
	{
		x = start_x;
		while (x <= end_x)
		{
			if (y >= 0 && y < game->map_height && x >= 0 && x < game->map_width)
			{
				char tile = game->map[y][x];
				int color = 0x000000;

				if (tile == '1')
					color = 0xFFFFFF;
				else if (tile == '0')
					color = 0x808080;
				else if (ft_strchr("NSEW", tile))
					color = 0xFF0000;

				// draw the visible map around the player
				draw_square(game,
					MINIMAP_OFFSET_X + (x - start_x) * MINIMAP_SCALE,
					MINIMAP_OFFSET_Y + (y - start_y) * MINIMAP_SCALE,
					MINIMAP_SCALE,
					color);
			}
			x++;
		}
		y++;
	}

	// Draw player at the center of the minimap (green)
	draw_square(game,
		MINIMAP_OFFSET_X + MINIMAP_RADIUS * MINIMAP_SCALE,
		MINIMAP_OFFSET_Y + MINIMAP_RADIUS * MINIMAP_SCALE,
		MINIMAP_SCALE,
		0x00FF00);
}

