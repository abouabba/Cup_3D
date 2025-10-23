/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animated.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:04:16 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/23 11:42:52 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	init_gun_files(char **files)
{
	files[0] = "textures/animat/1.xpm";
	files[1] = "textures/animat/2.xpm";
	files[2] = "textures/animat/3.xpm";
	files[3] = "textures/animat/4.xpm";
	files[4] = "textures/animat/5.xpm";
	files[5] = "textures/animat/6.xpm";
	files[6] = "textures/animat/7.xpm";
	files[7] = "textures/animat/8.xpm";
	files[8] = "textures/animat/9.xpm";
	files[9] = "textures/animat/10.xpm";
	files[10] = "textures/animat/11.xpm";
	files[11] = "textures/animat/12.xpm";
	files[12] = "textures/animat/13.xpm";
	files[13] = "textures/animat/14.xpm";
	files[14] = "textures/animat/15.xpm";
	files[15] = "textures/animat/16.xpm";
	files[16] = "textures/animat/17.xpm";
}

void	load_gun_sprite(t_game *game)
{
	int		i;
	char	*files[17];

	init_gun_files(files);
	i = 0;
	while (i < 17)
	{
		game->gun[i].img = mlx_xpm_file_to_image(game->helper->mlx, files[i],
				&game->gun[i].width, &game->gun[i].height);
		if (!game->gun[i].img)
		{
			ft_malloc(0, 0);
			printf("Error: failed to load %s\n", files[i]);
			exit(1);
		}
		i++;
	}
	game->frame = 0;
}

void	draw_gun(t_game *game)
{
	t_sprite	*gun;

	gun = &game->gun[game->frame];
	mlx_put_image_to_window(game->helper->mlx, game->helper->win, gun->img,
		(SCREEN_WIDTH / 2) - (gun->width / 2),
		SCREEN_HEIGHT - gun->height);
}

int	animate_gun(t_game *game)
{
	static int	counter;
	static int	idle_frame;

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

int	game_gun(t_game *game)
{
	render_map(game);
	draw_gun(game);
	animate_gun(game);
	return (0);
}
