/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 12:24:06 by abouabba          #+#    #+#             */
/*   Updated: 2025/08/31 10:31:41 by abouabba         ###   ########.fr       */
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
	int px = (int)game->player.x;
	int py = (int)game->player.y;
	int start_x = px - MINIMAP_RADIUS;
	int end_x   = px + MINIMAP_RADIUS;
	int start_y = py - MINIMAP_RADIUS;
	int end_y   = py + MINIMAP_RADIUS;
	int y = start_y;
	int x;
	while (y <= end_y)
	{
		x = start_x;
		while (x <= end_x)
		{
			if (y < 0 || y >= game->map_height || x < 0 || x >= game->map_width)
				continue; // skip out of map bounds

			char tile = game->map[y][x];
			int color = 0x000000; // default black (empty)

			if (tile == '1')
				color = 0xFFFFFF; // walls white
			else if (tile == '0')
				color = 0x808080; // floor gray
			else if (ft_strchr("NSEW", tile))
				color = 0xFF0000; // player start red

			draw_square(game,
				(x - start_x) * MINIMAP_SCALE,
				(y - start_y) * MINIMAP_SCALE,
				MINIMAP_SCALE, color);
			x++;
		}
		y++;
	}
	// draw player as green dot in center
	draw_square(game,
		MINIMAP_RADIUS * MINIMAP_SCALE,
		MINIMAP_RADIUS * MINIMAP_SCALE,
		MINIMAP_SCALE, 0x00FF00);
}
