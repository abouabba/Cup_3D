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

// // draw border around minimap
static void draw_minimap_frame(t_game *game)
{
	int x, y;
	int frame_x = MINIMAP_OFFSET_X;
	int frame_y = MINIMAP_OFFSET_Y;
	int size = MINIMAP_SIZE;

	for (y = 0; y < size; y++)
	{
		for (x = 0; x < size; x++)
		{
			if (x == 0 || y == 0 || x == size - 1 || y == size - 1)
				my_mlx_pixel_put(game, frame_x + x, frame_y + y, FRAME_COLOR);
		}
	}
}

// main minimap
void draw_minimap(t_game *game)
{
	draw_minimap_frame(game);

	double player_x = game->player.x;
	double player_y = game->player.y;

	double start_x = player_x - (MINIMAP_SIZE / (2 * MINIMAP_TILE_SIZE));
	double start_y = player_y - (MINIMAP_SIZE / (2 * MINIMAP_TILE_SIZE));
	double end_x = player_x + (MINIMAP_SIZE / (2 * MINIMAP_TILE_SIZE));
	double end_y = player_y + (MINIMAP_SIZE / (2 * MINIMAP_TILE_SIZE));

	for (int map_y = (int)start_y; map_y < end_y; map_y++)
	{
		for (int map_x = (int)start_x; map_x < end_x; map_x++)
		{
			if (map_y < 0 || map_y >= game->map_height ||
				map_x < 0 || map_x >= game->map_width)
				continue;

			char tile = game->map[map_y][map_x];
			int color;

			if (tile == '1')
				color = WALL_COLOR;
			else if (tile == 'D') // door
				color = 0x808000;
			else
				color = FLOOR_COLOR;

			int draw_x = MINIMAP_OFFSET_X + (map_x - start_x) * MINIMAP_TILE_SIZE;
			int draw_y = MINIMAP_OFFSET_Y + (map_y - start_y) * MINIMAP_TILE_SIZE;

			if (draw_x >= MINIMAP_OFFSET_X  + 4 && draw_x + MINIMAP_TILE_SIZE < MINIMAP_OFFSET_X + MINIMAP_SIZE - 1 &&
				draw_y >= MINIMAP_OFFSET_Y + 4 && draw_y + MINIMAP_TILE_SIZE < MINIMAP_OFFSET_Y + MINIMAP_SIZE - 1)
			{
				draw_square(game, draw_x, draw_y, MINIMAP_TILE_SIZE, color);
			}
		}
	}

	// Draw player at center (only if not inside a wall)
	int player_screen_x = MINIMAP_OFFSET_X + (MINIMAP_SIZE / 2);
	int player_screen_y = MINIMAP_OFFSET_Y + (MINIMAP_SIZE / 2);

	if (game->map[(int)player_y][(int)player_x] != '1' &&
		game->map[(int)player_y][(int)player_x] != ' ')
	{
		draw_square(game,
			player_screen_x - 2,
			player_screen_y - 2,
			4,
			PLAYER_COLOR);
	}
}
