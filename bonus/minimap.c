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

static void	draw_square_clipped(t_game *game, int x, int y, int s, int c)
{
	int	i;
	int	j;
	int	sx;
	int	sy;
	int	ex;
	int	ey;

	sx = x;
	sy = y;
	ex = x + s;
	ey = y + s;
	if (sx < MINIMAP_OFFSET_X + 1)
		sx = MINIMAP_OFFSET_X + 1;
	if (sy < MINIMAP_OFFSET_Y + 1)
		sy = MINIMAP_OFFSET_Y + 1;
	if (ex > MINIMAP_OFFSET_X + MINIMAP_SIZE - 1)
		ex = MINIMAP_OFFSET_X + MINIMAP_SIZE - 1;
	if (ey > MINIMAP_OFFSET_Y + MINIMAP_SIZE - 1)
		ey = MINIMAP_OFFSET_Y + MINIMAP_SIZE - 1;
	i = sy - 1;
	while (++i < ey)
	{
		j = sx - 1;
		while (++j < ex)
			my_mlx_pixel_put(game, j, i, c);
	}
}

static void	draw_minimap_tiles(t_game *game, double sx, double sy)
{
	int		map_x;
	int		map_y;
	int		draw_x;
	int		draw_y;
	char	tile;
	int		color;

	map_y = (int)sy - 1;
	while (++map_y < (int)(sy + (MINIMAP_SIZE / (double)MINIMAP_TILE_SIZE)) + 1)
	{
		map_x = (int)sx - 1;
		while (++map_x < (int)(sx + (MINIMAP_SIZE / (double)MINIMAP_TILE_SIZE)) + 1)
		{
			if (map_x < 0 || map_y < 0 || map_x >= game->map_width
				|| map_y >= game->map_height)
				continue;
			tile = game->map[map_y][map_x];
			if (tile == '1')
				color = WALL_COLOR;
			else
				color = FLOOR_COLOR;
			draw_x = MINIMAP_OFFSET_X + (int)((map_x - sx) * MINIMAP_TILE_SIZE);
			draw_y = MINIMAP_OFFSET_Y + (int)((map_y - sy) * MINIMAP_TILE_SIZE);
			draw_square_clipped(game, draw_x, draw_y, MINIMAP_TILE_SIZE, color);
		}
	}
}

void	draw_minimap(t_game *game)
{
	double	sx;
	double	sy;
	int		cx;
	int		cy;

	sx = game->player.x - (MINIMAP_SIZE / (2.0 * MINIMAP_TILE_SIZE));
	sy = game->player.y - (MINIMAP_SIZE / (2.0 * MINIMAP_TILE_SIZE));
	draw_minimap_tiles(game, sx, sy);
	cx = MINIMAP_OFFSET_X + (MINIMAP_SIZE / 2);
	cy = MINIMAP_OFFSET_Y + (MINIMAP_SIZE / 2);
	draw_square_clipped(game, cx - 2, cy - 2, 4, PLAYER_COLOR);
}