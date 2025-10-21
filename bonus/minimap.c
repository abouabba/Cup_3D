/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 12:24:06 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/17 22:16:21 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	draw_square(t_game *game, int x, int y, int size, int color)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			my_mlx_pixel_put(game, x + j, y + i, color);
		}
	}
}

static void draw_minimap_frame(t_game *game)
{
	int frame_x = MINIMAP_OFFSET_X;
	int frame_y = MINIMAP_OFFSET_Y;
	int size = MINIMAP_SIZE;

	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			if (x == 0 || y == 0 || x == size - 1 || y == size - 1)
				my_mlx_pixel_put(game, frame_x + x, frame_y + y, FRAME_COLOR);
		}
	}
}

void draw_minimap(t_game *game)
{
	draw_minimap_frame(game);

	double player_x = game->player.x;
	double player_y = game->player.y;

	int tiles_visible = MINIMAP_SIZE / MINIMAP_TILE_SIZE;

	int start_x = (int)(player_x - tiles_visible / 2);
	int start_y = (int)(player_y - tiles_visible / 2);
	int end_x = start_x + tiles_visible;
	int end_y = start_y + tiles_visible;

	for (int map_y = start_y; map_y < end_y; map_y++)
	{
		for (int map_x = start_x; map_x < end_x; map_x++)
		{
			if (map_y < 0 || map_y >= game->map_height ||
				map_x < 0 || map_x >= game->map_width)
				continue;

			char tile = game->map[map_y][map_x];
			int color;

			if (tile == '1')
				color = WALL_COLOR;
			else if (tile == 'D')
				color = 0x808000;
			else
				color = FLOOR_COLOR;
			int draw_x = MINIMAP_OFFSET_X + (map_x - start_x) * MINIMAP_TILE_SIZE;
			int draw_y = MINIMAP_OFFSET_Y + (map_y - start_y) * MINIMAP_TILE_SIZE;

			draw_square(game, draw_x, draw_y, MINIMAP_TILE_SIZE, color);
		}
	}
	int player_screen_x = MINIMAP_OFFSET_X + (MINIMAP_SIZE / 2);
	int player_screen_y = MINIMAP_OFFSET_Y + (MINIMAP_SIZE / 2);
	draw_square(game, player_screen_x - 2, player_screen_y - 2, 4, PLAYER_COLOR);
}
