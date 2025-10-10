/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animated.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:04:16 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/10 12:30:12 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void    load_gun_sprite(t_game *game)
{
    game->gun[0].img = mlx_xpm_file_to_image(game->helper->mlx, "animat/gun_2.xpm", &game->gun[0].width, &game->gun[0].height);
    if (!game->gun[0].img)
    {
        printf("Error: failed to load animat/gun_2.xpm\n");
        exit(1);
    }
    game->gun[1].img = mlx_xpm_file_to_image(game->helper->mlx, "animat/gun_1.xpm", &game->gun[1].width, &game->gun[1].height);
    if (!game->gun[1].img)
    {
        printf("Error: failed to load animat/gun_1.xpm\n");
        exit(1);
    }
    game->frame = 0;
}

void draw_gun(t_game *game)
{
    t_sprite *gun;

    if (game->frame == 0)
        gun = &game->gun[0];
    else
        gun = &game->gun[game->frame];

    mlx_put_image_to_window(game->helper->mlx,
        game->helper->win,
        gun->img,
        0,
        SCREEN_HEIGHT - gun->height
    );
}


int rander(t_game *game)
{
    render_map(game);
    draw_gun(game);
    return (0);
}

int animate_gun(t_game *game)
{
	static int counter = 0;

	if (game->frame == 1)
	{
		counter++;
		if (counter > 5)
		{
			game->frame = 0;
			counter = 0;
		}
	}
	return (0);
}

int game_gun(t_game *game)
{
    rander(game);
    animate_gun(game);
    return (0);
}