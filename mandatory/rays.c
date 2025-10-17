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

double	cast_dda(t_game *game, t_ray *ray, t_casting *holder)
{
	double	dist_to_wall;

	if (ray->ray_dir_x < 0)
	{
		holder->step_x = -1;
		holder->side_dist_x = (ray->ray_x / TILE_SIZE - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		holder->step_x = 1;
		holder->side_dist_x = (ray->map_x + 1.0 - ray->ray_x / TILE_SIZE) * ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		holder->step_y = -1;
		holder->side_dist_y = (ray->ray_y / TILE_SIZE - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		holder->step_y = 1;
		holder->side_dist_y = (ray->map_y + 1.0 - ray->ray_y / TILE_SIZE) * ray->delta_dist_y;
	}
	holder = loop_helper(game, ray, holder);
	if (holder->side == 0)
		dist_to_wall = (ray->map_x * TILE_SIZE - ray->ray_x + (1 - holder->step_x) * TILE_SIZE / 2) / ray->ray_dir_x;
	else
		dist_to_wall = (ray->map_y * TILE_SIZE - ray->ray_y + (1 - holder->step_y) * TILE_SIZE / 2) / ray->ray_dir_y;
	ray->perp_wall_dist = dist_to_wall;
	ray->side = holder->side;
	ray->hit_x = ray->ray_x + ray->ray_dir_x * dist_to_wall;
	ray->hit_y = ray->ray_y + ray->ray_dir_y * dist_to_wall;
	return (dist_to_wall);
}

double	normalize_angle(double angle)
{
	while (angle < 0)
		angle += 2 * M_PI;
	while (angle >= 2 * M_PI)
		angle -= 2 * M_PI;
	return (angle);
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
	ray.ray_y  = ((game->player.y) * TILE_SIZE);
	ray.ray_dir_x = cos(ray.ray_angle);
	ray.ray_dir_y = sin(ray.ray_angle);
	ray.map_x = (int)(ray.ray_x / TILE_SIZE);
	ray.map_y = (int)(ray.ray_y / TILE_SIZE);
	ray.delta_dist_x = (fabs(ray.ray_dir_x) < 1e-6) ? 1e30 : fabs(1 / ray.ray_dir_x);
	ray.delta_dist_y = (fabs(ray.ray_dir_y) < 1e-6) ? 1e30 : fabs(1 / ray.ray_dir_y);
	return (ray);
}

void texture_loop(t_loopvars *vars ,t_game *game, t_ray ray, t_texthelper *helper)
{
	unsigned int	color;
	int				tex_y;

	while (helper->y <= vars->draw_end)
	{
		tex_y = (int)helper->tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		else if (tex_y >= helper->tex_height)
			tex_y = helper->tex_height - 1;
		helper->tex_pos += helper->step;
		char	*pixel = vars->texture->data_add + tex_y * vars->texture->line_len + helper->tex_x * (vars->texture->bbp / 8);
		color = *(unsigned int *)pixel;
		if (ray.side == 1)
			color = (color >> 1) & 0x7F7F7F;
		pixel = game->helper->addr + (helper->y * game->helper->line_len + helper->x * (game->helper->bpp / 8));
		*(unsigned int *)pixel = color;
		helper->y++;
	}
}

void	texture_pass(t_game *game, int x,t_ray ray, t_loopvars *vars)
{
	t_texthelper	*helper;

	helper = ft_malloc(sizeof(t_texthelper), 1);
	helper->tex_width = vars->texture->width;
	helper->tex_height = vars->texture->height;
	if (x < 0 || x >= SCREEN_WIDTH)
		return ;
	if (vars->draw_start < 0)
		vars->draw_start = 0;
	if (vars->draw_end >= SCREEN_HEIGHT)
		vars->draw_end = SCREEN_HEIGHT - 1;
	if (ray.side == 0)
		helper->wall_x = ray.hit_y / TILE_SIZE - floor(ray.hit_y / TILE_SIZE);
	else
		helper->wall_x = ray.hit_x / TILE_SIZE - floor(ray.hit_x / TILE_SIZE);
	helper->tex_x = (int)(helper->wall_x * helper->tex_width);
	if ((ray.side == 0 && ray.ray_dir_x > 0) || (ray.side == 1 && ray.ray_dir_y < 0))
		helper->tex_x = helper->tex_width - helper->tex_x - 1;
	helper->step = (double)helper->tex_height / (double)vars->line_height;
	helper->tex_pos = (vars->draw_start - SCREEN_HEIGHT / 2 + vars->line_height / 2) * helper->step;
	helper->y = vars->draw_start;
	helper->x = x;
	texture_loop(vars, game, ray, helper);
}

void	draw_floor_and_ceiling(t_game *game, int x, t_loopvars *vars)
{
	char	*addr;
	int		stride;
	int		bpp_bytes;
	int		y;
	int		yyx;

	yyx = 0;
	y = vars->draw_end + 1;
	stride = game->helper->line_len;
	bpp_bytes = game->helper->bpp / 8;
	addr = game->helper->addr;
	if (x < 0 || x >= SCREEN_WIDTH)
		return ;
	if (vars->draw_start < 0)
		vars->draw_start = 0;
	if (vars->draw_start > (SCREEN_HEIGHT + 1))
		vars->draw_start = (SCREEN_HEIGHT + 1);
	if (vars->draw_end < -1)
		vars->draw_end = -1;
	if (vars->draw_end >= (SCREEN_HEIGHT + 1))
		vars->draw_end = (SCREEN_HEIGHT + 1) - 1;
	while (yyx < vars->draw_start)
	{
		*(unsigned int *)(addr + yyx * stride + x * bpp_bytes) = game->ceiling_color;
		++yyx;
	}
	while (y < (SCREEN_HEIGHT + 1))
	{
		*(unsigned int *)(addr + y * stride + x * bpp_bytes) = game->floor_color;
		y++;
	}
}

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

void	varsinit(t_loopvars *vars, double distance, t_casting *holder)
{
	vars->line_height = (int)(TILE_SIZE * SCREEN_HEIGHT / distance);
	vars->draw_start = - vars->line_height / 2 + SCREEN_HEIGHT / 2;
	vars->draw_end = vars->line_height / 2 + SCREEN_HEIGHT / 2;
	vars->color = (holder->side == 1) ? 0xAAAAAA : 0xFFFFFF;
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
	double		distance;
	double		dist;

	vars = ft_malloc(sizeof(t_loopvars), 1);
	i_loop = 0;
	while (i_loop < SCREEN_WIDTH)
	{
		ray = prepare_vars(ray, game, i_loop);
		dist = cast_dda(game, &ray, holder);
		distance = dist * cos(ray.ray_angle - game->angle);
		ray.perp_wall_dist = distance;
		varsinit(vars, distance, holder);
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
	holder->hited = 0;
	holder->side = 0;
	holder->side_dist_x = 0;
	holder->side_dist_y = 0;
	i_loop = 0;
	loophandeler(holder, game);
}
