/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animated.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:04:16 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/14 17:39:00 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"


void    load_gun_sprite(t_game *game)
{
	char *files[17] = {"animat/1.xpm",  "animat/2.xpm",  "animat/3.xpm",  "animat/4.xpm",
                        "animat/5.xpm",  "animat/6.xpm",  "animat/7.xpm",  "animat/8.xpm",
                        "animat/9.xpm",  "animat/10.xpm",  "animat/11.xpm",  "animat/12.xpm",
                        "animat/13.xpm",  "animat/14.xpm",  "animat/15.xpm",  "animat/16.xpm",
                        "animat/17.xpm",};
	int i = 0;
	while (i < 17)
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

    gun = &game->gun[game->frame];

    mlx_put_image_to_window(
        game->helper->mlx,
        game->helper->win,
        gun->img,
        (SCREEN_WIDTH / 2) - (gun->width / 2),
        SCREEN_HEIGHT - gun->height
    );
}

int animate_gun(t_game *game)
{
    static int counter = 0;
    static int idle_frame = 0;

    counter++;

    if (game->frame == 0)
    {
        if (counter > 10)
        {
            idle_frame = (idle_frame + 1) % 1;
            game->frame = 0;
            counter = 0;
        }
    }
    else
    {
        if (counter > 2)
        {
            game->frame++;
            if (game->frame >= 17)
                game->frame = 0;
            counter = 0;
        }
    }
    return (0);
}

int game_gun(t_game *game)
{
    render_map(game);
	draw_gun(game);
	animate_gun(game);
	return (0);
}