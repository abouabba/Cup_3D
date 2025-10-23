/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 12:24:06 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/23 11:47:01 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int	get_tile_color(char tile)
{
	if (tile == '1')
		return (WALL_COLOR);
	return (FLOOR_COLOR);
}

void	draw_tile(t_game *game, t_tile *t)
{
	int	draw_x;
	int	draw_y;
	int	color;
	int	args[2];

	color = get_tile_color(game->map[t->y][t->x]);
	draw_x = MINIMAP_OFFSET_X + (int)((t->x - t->sx) * MINIMAP_TILE_SIZE);
	draw_y = MINIMAP_OFFSET_Y + (int)((t->y - t->sy) * MINIMAP_TILE_SIZE);
	args[0] = MINIMAP_TILE_SIZE;
	args[1] = color;
	draw_square_clipped(game, draw_x, draw_y, args);
}

void	draw_minimap_tiles(t_game *game, double sx, double sy)
{
	t_tile	t;

	t.y = (int)sy - 1;
	while (++t.y < (int)(sy + (MINIMAP_SIZE / MINIMAP_TILE_SIZE)) + 1)
	{
		t.x = (int)sx - 1;
		while (++t.x < (int)(sx + (MINIMAP_SIZE / MINIMAP_TILE_SIZE)) + 1)
		{
			if (t.x < 0 || t.y < 0
				|| t.x >= game->map_width
				|| t.y >= game->map_height)
				continue ;
			t.sx = sx;
			t.sy = sy;
			draw_tile(game, &t);
		}
	}
}

void	draw_minimap(t_game *game)
{
	double	sx;
	double	sy;
	int		cx;
	int		cy;
	int		args[2];

	sx = game->player.x - (MINIMAP_SIZE / (2.0 * MINIMAP_TILE_SIZE));
	sy = game->player.y - (MINIMAP_SIZE / (2.0 * MINIMAP_TILE_SIZE));
	draw_minimap_tiles(game, sx, sy);
	cx = MINIMAP_OFFSET_X + (MINIMAP_SIZE / 2);
	cy = MINIMAP_OFFSET_Y + (MINIMAP_SIZE / 2);
	args[0] = 4;
	args[1] = PLAYER_COLOR;
	draw_square_clipped(game, cx - 2, cy - 2, args);
}
