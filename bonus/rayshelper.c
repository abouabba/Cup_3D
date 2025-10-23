/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rayshelper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlamlaik <rlamlaik@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-10-23 09:41:09 by rlamlaik          #+#    #+#             */
/*   Updated: 2025-10-23 09:41:09 by rlamlaik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

t_casting	*loop_helper(t_game *game, t_ray *ray, t_casting *holder)
{
	holder->hited = 0;
	while (!holder->hited)
	{
		if (holder->side_dist_x < holder->side_dist_y)
		{
			holder->side_dist_x += ray->delta_dist_x;
			ray->map_x += holder->step_x;
			holder->side = 0;
		}
		else
		{
			holder->side_dist_y += ray->delta_dist_y;
			ray->map_y += holder->step_y;
			holder->side = 1;
		}
		if (game->map[ray->map_y][ray->map_x] == '1')
			holder->hited = 1;
	}
	return (holder);
}

void	move_side_dda(t_ray *ray, t_casting *holder)
{
	if (ray->ray_dir_x < 0)
	{
		holder->step_x = -1;
		holder->side_dist_x = \
		(ray->ray_x / TILE_SIZE - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		holder->step_x = 1;
		holder->side_dist_x = \
		(ray->map_x + 1.0 - ray->ray_x / TILE_SIZE) * ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		holder->step_y = -1;
		holder->side_dist_y = \
		(ray->ray_y / TILE_SIZE - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		holder->step_y = 1;
		holder->side_dist_y = \
		(ray->map_y + 1.0 - ray->ray_y / TILE_SIZE) * ray->delta_dist_y;
	}
}

double	cast_dda(t_game *game, t_ray *ray, t_casting *holder)
{
	double	dist_to_wall;

	move_side_dda(ray, holder);
	holder = loop_helper(game, ray, holder);
	if (holder->side == 0)
		dist_to_wall = \
		(ray->map_x * TILE_SIZE - ray->ray_x + \
		(1 - holder->step_x) * TILE_SIZE / 2) / ray->ray_dir_x;
	else
		dist_to_wall = \
		(ray->map_y * TILE_SIZE - ray->ray_y + \
		(1 - holder->step_y) * TILE_SIZE / 2) / ray->ray_dir_y;
	ray->perp_wall_dist = dist_to_wall;
	ray->side = holder->side;
	ray->hit_x = ray->ray_x + ray->ray_dir_x * dist_to_wall;
	ray->hit_y = ray->ray_y + ray->ray_dir_y * dist_to_wall;
	return (dist_to_wall);
}

t_ray	prepare_vars(t_ray ray, t_game *game, int i_loop)
{
	double	fov;
	double	angle_step;
	double	start_angle;

	fov = M_PI / 3.0;
	angle_step = fov / (double)SCREEN_WIDTH;
	start_angle = normalize_angle(game->angle - fov / 2);
	ray.ray_angle = normalize_angle(start_angle + i_loop * angle_step);
	ray.ray_x = ((game->player.x) * TILE_SIZE);
	ray.ray_y = ((game->player.y) * TILE_SIZE);
	ray.ray_dir_x = cos(ray.ray_angle);
	ray.ray_dir_y = sin(ray.ray_angle);
	ray.map_x = (int)(ray.ray_x / TILE_SIZE);
	ray.map_y = (int)(ray.ray_y / TILE_SIZE);
	ray.delta_dist_x = fabs(1 / ray.ray_dir_x);
	ray.delta_dist_y = fabs(1 / ray.ray_dir_y);
	return (ray);
}

void	texture_loop(t_loopvars*vars, t_game*game, \
t_ray ray, t_texthelper*helper)
{
	unsigned int	color;
	int				tex_y;
	char			*pixel;

	while (helper->y <= vars->draw_end)
	{
		tex_y = (int)helper->tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		else if (tex_y >= helper->tex_height)
			tex_y = helper->tex_height - 1;
		helper->tex_pos += helper->step;
		pixel = vars->texture->data_add + \
	tex_y * vars->texture->line_len + helper->tex_x * (vars->texture->bbp / 8);
		color = *(unsigned int *)pixel;
		if (ray.side == 1)
			color = (color >> 1) & 0x7F7F7F;
		pixel = game->helper->addr + \
	(helper->y * game->helper->line_len + helper->x * (game->helper->bpp / 8));
		*(unsigned int *)pixel = color;
		helper->y++;
	}
}
