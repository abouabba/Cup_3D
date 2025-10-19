/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_move.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 15:52:55 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/18 15:56:04 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int	mouse_move_hook(int x, int y, t_game *game)
{
	static int	last_x;
	int			delta_x;
	double		retation_speed;

	(void)y;
	retation_speed = 0.005;
	if (last_x == 0)
		last_x = x;
	delta_x = x - last_x;
	if (delta_x != 0)
	{
		game->angle += delta_x * retation_speed;
		if (game->angle < 0)
			game->angle += 2 * M_PI;
		if (game->angle >= 2 * M_PI)
			game->angle -= 2 * M_PI;
		game->player.dir_x = cos(game->angle);
		game->player.dir_y = sin(game->angle);
	}
	last_x = x;
	return (0);
}
