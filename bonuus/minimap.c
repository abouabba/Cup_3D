/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 12:24:06 by abouabba          #+#    #+#             */
/*   Updated: 2025/08/30 14:20:02 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	my_mlx_pixel_put(void	*img, int x, int y, int color)
{
	
}

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
			my_mlx_pixel_put(&game->helper->img, x + j, y + i, color);
			j++;
		}
		i++;
	}
}

void	draw_minimap(t_game *game)
{
	int row;
	int col;
	int tile_size = TILE_SIZE * MINIMAP_SCALE;
	int x;
	int y;
	
	row = 0;
	while(game->map[row])
	{
		col = 0;
		while(game->map[row][col])
		{
			x = MINIMAP_OFFSET_X + col * tile_size;
			y = MINIMAP_OFFSET_Y + row * tile_size;

			if (game->map[row][col])
				draw_square(game, x, y, tile_size, COL_WALL);
			else
				draw_square(game, x, y, tile_size, COL_VOID);
			col++;
		}
		row++;
	}
}
