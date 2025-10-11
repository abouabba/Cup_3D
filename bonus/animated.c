/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animated.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:04:16 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/11 16:52:27 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void    load_gun_sprite(t_game *game)
{
	char *files[15] = {"animat/gun_1.xpm", "animat/gun_2.xpm", "animat/gun_3.xpm",
                        "animat/gun_4.xpm", "animat/gun_5.xpm", "animat/gun_6.xpm",
                        "animat/gun_7.xpm", "animat/gun_8.xpm", "animat/gun_9.xpm",
                        "animat/gun_10.xpm", "animat/gun_11.xpm", "animat/gun_12.xpm",
                        "animat/gun_13.xpm", "animat/gun_14.xpm", "animat/gun_15.xpm"};
	int i = 0;
	while (i < 15)
	{
		game->gun[i].img = mlx_xpm_file_to_image(
			game->helper->mlx, files[i], &game->gun[i].width, &game->gun[i].height);
		if (!game->gun[i].img)
		{
			printf("Error: failed to load %s\n", files[i]);
			exit(1);
		}
		i++;
	}
	game->frame = 0;
}

void draw_gun(t_game *game)
{
    t_sprite *gun;

    if (game->frame == 14)
        gun = &game->gun[14];
    else
        gun = &game->gun[game->frame];

    mlx_put_image_to_window(
        game->helper->mlx,
        game->helper->win,
        gun->img,
        (SCREEN_WIDTH / 2) - (gun->width / 2),
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
    static int idle_frame = 0;

    counter++;

    if (game->frame == 14)
    {
        if (counter > 15)
        {
            game->frame = 0;
            counter = 0;
        }
    }
    else
    {
        if (counter > 10)
        {
            idle_frame = (idle_frame + 1) % 14;
            game->frame = idle_frame;
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