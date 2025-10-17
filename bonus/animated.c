/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animated.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:04:16 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/16 10:18:29 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"


void    load_gun_sprite(t_game *game)
{
	char *files[17] = {"textures/animat/1.xpm",  "textures/animat/2.xpm",  "textures/animat/3.xpm",  "textures/animat/4.xpm",
                        "textures/animat/5.xpm",  "textures/animat/6.xpm",  "textures/animat/7.xpm",  "textures/animat/8.xpm",
                        "textures/animat/9.xpm",  "textures/animat/10.xpm",  "textures/animat/11.xpm",  "textures/animat/12.xpm",
                        "textures/animat/13.xpm",  "textures/animat/14.xpm",  "textures/animat/15.xpm",  "textures/animat/16.xpm",
                        "textures/animat/17.xpm",};
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