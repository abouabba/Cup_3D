/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 12:24:06 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/14 19:58:17 by abouabba         ###   ########.fr       */
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
	int map_x, map_y, color;

	// fixed minimap position
	int minimap_x = 10;
	int minimap_y = 10;

	// fixed size (visible area)
	int tiles_visible_x = MINIMAP_SIZE / MINIMAP_TILE_SIZE;
	int tiles_visible_y = MINIMAP_SIZE / MINIMAP_TILE_SIZE;

	// center view around player
	double start_x = game->player.x - tiles_visible_x / 2.0;
	double start_y = game->player.y - tiles_visible_y / 2.0;

	// 🟥 draw frame first (dark background)
	draw_square(game,
		minimap_x - 3,
		minimap_y - 3,
		MINIMAP_SIZE + 6,
		0x202020); // dark gray frame

	// 🟩 draw map inside the frame
	for (map_y = 0; map_y < tiles_visible_y; map_y++)
	{
		for (map_x = 0; map_x < tiles_visible_x; map_x++)
		{
			int real_x = (int)(start_x + map_x);
			int real_y = (int)(start_y + map_y);

			// skip if outside map
			if (real_y < 0 || real_y >= game->map_height ||
				real_x < 0 || real_x >= game->map_width)
				continue;

			char tile = game->map[real_y][real_x];
			if (tile == '1')
				color = 0x800000; // wall = dark red
			else if (tile == 'D')
				color = 0xFFD700; // door = gold
			else
				color = 0x000000; // floor = black

			draw_square(game,
				minimap_x + map_x * MINIMAP_TILE_SIZE,
				minimap_y + map_y * MINIMAP_TILE_SIZE,
				MINIMAP_TILE_SIZE,
				color);
		}
	}

	// 🟢 draw player in center of minimap
	int player_screen_x = minimap_x + (tiles_visible_x / 2) * MINIMAP_TILE_SIZE;
	int player_screen_y = minimap_y + (tiles_visible_y / 2) * MINIMAP_TILE_SIZE;

	draw_square(game,
		player_screen_x - 2,
		player_screen_y - 2,
		4,
		0x00FF00); // green
}

