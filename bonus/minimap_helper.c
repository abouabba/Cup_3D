/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 11:46:29 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/23 11:47:06 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	init_clip(t_clip *clip, int x, int y, int s)
{
	clip->sx = x;
	clip->sy = y;
	clip->ex = x + s;
	clip->ey = y + s;
	if (clip->sx < MINIMAP_OFFSET_X + 1)
		clip->sx = MINIMAP_OFFSET_X + 1;
	if (clip->sy < MINIMAP_OFFSET_Y + 1)
		clip->sy = MINIMAP_OFFSET_Y + 1;
	if (clip->ex > MINIMAP_OFFSET_X + MINIMAP_SIZE - 1)
		clip->ex = MINIMAP_OFFSET_X + MINIMAP_SIZE - 1;
	if (clip->ey > MINIMAP_OFFSET_Y + MINIMAP_SIZE - 1)
		clip->ey = MINIMAP_OFFSET_Y + MINIMAP_SIZE - 1;
}

void	draw_square_clipped(t_game *game, int x, int y, int s_color[2])
{
	t_clip	c;
	int		i;
	int		j;

	init_clip(&c, x, y, s_color[0]);
	i = c.sy - 1;
	while (++i < c.ey)
	{
		j = c.sx - 1;
		while (++j < c.ex)
			my_mlx_pixel_put(game, j, i, s_color[1]);
	}
}
