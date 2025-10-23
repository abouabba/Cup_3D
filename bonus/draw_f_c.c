/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rayhelper2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rlamlaik <rlamlaik@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-10-23 09:44:22 by rlamlaik          #+#    #+#             */
/*   Updated: 2025-10-23 09:44:22 by rlamlaik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

double	normalize_angle(double angle)
{
	while (angle < 0)
		angle += 2 * M_PI;
	while (angle >= 2 * M_PI)
		angle -= 2 * M_PI;
	return (angle);
}

void	texture_pass(t_game *game, int x, t_ray ray, t_loopvars *vars)
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
	if ((ray.side == 0 && ray.ray_dir_x > 0) || \
	(ray.side == 1 && ray.ray_dir_y < 0))
		helper->tex_x = helper->tex_width - helper->tex_x - 1;
	helper->step = (double)helper->tex_height / (double)vars->line_height;
	helper->tex_pos = (vars->draw_start - \
	SCREEN_HEIGHT / 2 + vars->line_height / 2) * helper->step;
	(1) && (helper->y = vars->draw_start, helper->x = x);
	texture_loop(vars, game, ray, helper);
}

void	helper_draw_ceilling(t_loopvars *vars)
{
	if (vars->draw_start < 0)
		vars->draw_start = 0;
	if (vars->draw_start > (SCREEN_HEIGHT + 1))
		vars->draw_start = (SCREEN_HEIGHT + 1);
	if (vars->draw_end < -1)
		vars->draw_end = -1;
	if (vars->draw_end >= (SCREEN_HEIGHT + 1))
		vars->draw_end = (SCREEN_HEIGHT + 1) - 1;
}

void	draw_floor_and_ceiling(t_game *game, int x, t_loopvars *vars)
{
	char	*addr;
	int		stride;
	int		bpp_bytes;
	int		y;
	int		yyx;

	(1) && (yyx = 0, y = vars->draw_end + 1);
	stride = game->helper->line_len;
	bpp_bytes = game->helper->bpp / 8;
	addr = game->helper->addr;
	if (x < 0 || x >= SCREEN_WIDTH)
		return ;
	helper_draw_ceilling(vars);
	while (yyx < vars->draw_start)
	{
		*(unsigned int *) \
		(addr + yyx * stride + x * bpp_bytes) = game->ceiling_color;
		++yyx;
	}
	while (y < (SCREEN_HEIGHT))
	{
		*(unsigned int *) \
		(addr + y * stride + x * bpp_bytes) = game->floor_color;
		y++;
	}
}
