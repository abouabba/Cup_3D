/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 08:55:50 by rlamlaik          #+#    #+#             */
/*   Updated: 2025/10/07 11:58:58 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int	texture_side(t_ray ray)
{
	int	index;

	index = 0;
	if (ray.side == 0)
	{
		if (ray.ray_dir_x > 0)
			index = EAST_TEXTURE;
		else
			index = WEST_TEXTURE;
	}
	else
	{
		if (ray.ray_dir_y > 0)
			index = SOUTH_TEXTURE;
		else
			index = NORTH_TEXTURE;
	}
	return (index);
}

void	varsinit(t_loopvars *vars, double distance)
{
	vars->line_height = (int)(TILE_SIZE * SCREEN_HEIGHT / distance);
	vars->draw_start = -vars->line_height / 2 + SCREEN_HEIGHT / 2;
	vars->draw_end = vars->line_height / 2 + SCREEN_HEIGHT / 2;
	if (vars->draw_start < 0)
		vars->draw_start = 0;
	if (vars->draw_end >= SCREEN_HEIGHT)
		vars->draw_end = SCREEN_HEIGHT - 1;
}

void	init_player_direction(t_game *game, char dir)
{
	if (dir == 'N')
	{
		game->angle = M_PI / 2;
		game->player.dir_x = 0;
		game->player.dir_y = -1;
	}
	else if (dir == 'S')
	{
		game->angle = 3 * M_PI / 2;
		game->player.dir_x = 0;
		game->player.dir_y = 1;
	}
	else if (dir == 'E')
	{
		game->angle = 0;
		game->player.dir_x = 1;
		game->player.dir_y = 0;
	}
	else if (dir == 'W')
	{
		game->angle = M_PI;
		game->player.dir_x = -1;
		game->player.dir_y = 0;
	}
}

int	loophandeler(t_casting	*holder, t_game *game)
{
	int			i_loop;
	t_ray		ray;
	int			texture_index;
	t_loopvars	*vars;
	double		dist;

	vars = ft_malloc(sizeof(t_loopvars), 1);
	if (!vars)
		exit(write(2, "Error initializing mlx\n", 24));
	i_loop = 0;
	while (i_loop < SCREEN_WIDTH)
	{
		ray = prepare_vars(ray, game, i_loop);
		dist = cast_dda(game, &ray, holder);
		dist = dist * cos(ray.ray_angle - game->angle);
		ray.perp_wall_dist = dist;
		varsinit(vars, dist);
		texture_index = texture_side(ray);
		game->ray = ray;
		draw_floor_and_ceiling(game, i_loop, vars);
		vars->texture = &game->txtu[texture_index];
		texture_pass(game, i_loop, ray, vars);
		i_loop++;
	}
	return (1);
}

void	the_3dview(t_game *game)
{
	int			i_loop;
	t_casting	*holder;

	holder = ft_malloc(sizeof(t_casting), 1);
	if (!holder)
		exit(write(2, "Error initializing mlx\n", 24));
	holder->hited = 0;
	holder->side = 0;
	holder->side_dist_x = 0;
	holder->side_dist_y = 0;
	i_loop = 0;
	loophandeler(holder, game);
}
