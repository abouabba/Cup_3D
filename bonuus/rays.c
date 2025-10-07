/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 08:55:50 by rlamlaik          #+#    #+#             */
/*   Updated: 2025/10/07 10:55:49 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

t_casting    *loop_helper(t_game *game, t_ray *ray, t_casting *holder)
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

double cast_dda(t_game *game, t_ray *ray, t_casting *holder)
{
	double dist_to_wall;

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

void	draw_vertical_line(t_game *game, int x, int start, int end, int color)
{
	(void)game;
	char	*pixel;
	int		y;

	y = start;
	while (y <= end)
	{
		pixel = game->helper->addr + (y * game->helper->line_len + x * (game->helper->bpp / 8));
		*(unsigned int *)pixel = color;
		y++;
	}
}


double cast_ray(t_game *game, t_ray *ray, int ray_id)
{
	int step_x, step_y;
	double side_dist_x, side_dist_y;
	int hit = 0;
	int side;
	(void)ray_id;

	if (ray->ray_dir_x < 0)
	{
		step_x = -1;
		side_dist_x = (ray->ray_x / TILE_SIZE - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		step_x = 1;
		side_dist_x = (ray->map_x + 1.0 - ray->ray_x / TILE_SIZE) * ray->delta_dist_x;
	}

	if (ray->ray_dir_y < 0)
	{
		step_y = -1;
		side_dist_y = (ray->ray_y / TILE_SIZE - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		step_y = 1;
		side_dist_y = (ray->map_y + 1.0 - ray->ray_y / TILE_SIZE) * ray->delta_dist_y;
	}
	while (!hit)
	{
		if (side_dist_x < side_dist_y)
		{
			side_dist_x += ray->delta_dist_x;
			ray->map_x += step_x;
			side = 0;
		}
		else
		{
			side_dist_y += ray->delta_dist_y;
			ray->map_y += step_y;
			side = 1;
		}
		if (game->map[ray->map_y][ray->map_x] == '1')
			hit = 1;
	}
	double perp_wall_dist;
	if (side == 0)
		perp_wall_dist = (ray->map_x * TILE_SIZE - ray->ray_x + (1 - step_x) * TILE_SIZE / 2) / ray->ray_dir_x;
	else
		perp_wall_dist = (ray->map_y * TILE_SIZE - ray->ray_y + (1 - step_y) * TILE_SIZE / 2) / ray->ray_dir_y;
	ray->perp_wall_dist = perp_wall_dist;
	ray->side = side;
	ray->hit_x = ray->ray_x + ray->ray_dir_x * perp_wall_dist;
	ray->hit_y = ray->ray_y + ray->ray_dir_y * perp_wall_dist;


	return perp_wall_dist;
}

double normalize_angle(double angle)
{
	while (angle < 0)
		angle += 2 * M_PI;
	while (angle >= 2 * M_PI)
		angle -= 2 * M_PI;
	return angle;
}

t_ray    prepare_vars(t_ray ray, t_game *game, int i_loop)
{
	double fov;
	double angle_step;
	double start_angle;

	fov = M_PI / 3.0; // witc is the view 60
	angle_step = fov / (double)SCREEN_WIDTH; // to represent every dgree one oriziotal line
	start_angle = normalize_angle(game->angle - fov / 2);

	ray.ray_angle = normalize_angle(start_angle + i_loop * angle_step);
	ray.ray_x  =  game->player.x * TILE_SIZE;
	ray.ray_y  =  game->player.y * TILE_SIZE;
	ray.ray_dir_x = cos(ray.ray_angle);
	ray.ray_dir_y = sin(ray.ray_angle);
	ray.map_x = (int)(ray.ray_x / TILE_SIZE);
	ray.map_y = (int)(ray.ray_y / TILE_SIZE);
	ray.delta_dist_x = (fabs(ray.ray_dir_x) < 1e-6) ? 1e30 : fabs(1 / ray.ray_dir_x);
	ray.delta_dist_y = (fabs(ray.ray_dir_y) < 1e-6) ? 1e30 : fabs(1 / ray.ray_dir_y);
	return (ray);
}

void texture_pass(t_game *game, int x, int start, int end,
				  t_ray ray, t_casting *holder, t_txtu *texture, int line_height)
{
	(void) holder;
	int tex_width = texture->width;
	int tex_height = texture->height;
	double wall_x;

	if (x < 0 || x >= SCREEN_WIDTH)
		return;
	if (start < 0)
		start = 0;
	if (end >= SCREEN_HEIGHT)
		end = SCREEN_HEIGHT - 1;
	if (ray.side == 0)
		wall_x = ray.hit_y / TILE_SIZE - floor(ray.hit_y / TILE_SIZE);
	else
		wall_x = ray.hit_x / TILE_SIZE - floor(ray.hit_x / TILE_SIZE);

	int tex_x = (int)(wall_x * tex_width);
	if ((ray.side == 0 && ray.ray_dir_x > 0) || (ray.side == 1 && ray.ray_dir_y < 0))
		tex_x = tex_width - tex_x - 1;

	double step = (double)tex_height / (double)line_height;
	double tex_pos = (start - SCREEN_HEIGHT / 2 + line_height / 2) * step;

	for (int y = start; y <= end; y++)
	{
		int tex_y = (int)tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		else if (tex_y >= tex_height)
			tex_y = tex_height - 1;
		tex_pos += step;
		char *pixel = texture->data_add + tex_y * texture->line_len + tex_x * (texture->bbp / 8);
		unsigned int color = *(unsigned int *)pixel;
		if (ray.side == 1)
			color = (color >> 1) & 0x7F7F7F;
		pixel = game->helper->addr + (y * game->helper->line_len + x * (game->helper->bpp / 8));
		*(unsigned int *)pixel = color;
	}
}

void draw_floor_and_ceiling(t_game *game, int x, int screen_h,
                            int draw_start, int draw_end,
                            unsigned int floor_color, unsigned int ceiling_color)
{
    char *addr = game->helper->addr;
    int stride = game->helper->line_len;
    int bpp_bytes = game->helper->bpp / 8;
    int img_w = SCREEN_WIDTH;

    if (x < 0 || x >= img_w)
		return;
    if (draw_start < 0)
		draw_start = 0;
    if (draw_start > screen_h)
		draw_start = screen_h;
    if (draw_end < -1)
		draw_end = -1;
    if (draw_end >= screen_h)
		draw_end = screen_h - 1;
    for (int y = 0; y < draw_start; ++y) {
        *(unsigned int *)(addr + y * stride + x * bpp_bytes) = ceiling_color;
    }
    for (int y = draw_end + 1; y < screen_h; ++y) {
        *(unsigned int *)(addr + y * stride + x * bpp_bytes) = floor_color;
    }
}
void    the_3dview(t_game *game)
{
	t_ray		ray;
	int			i_loop;
	t_casting	*holder;

	holder = malloc(sizeof(t_casting));
	holder->hited =0;
	holder->side =0;
	holder->side_dist_x =0;
	holder->side_dist_y =0;
	i_loop = 0;

	while (i_loop < SCREEN_WIDTH)
	{
		ray  = prepare_vars(ray, game, i_loop);
		double dist = cast_dda(game, &ray, holder);
		double distance = dist * cos(ray.ray_angle - game->angle);
		ray.perp_wall_dist = distance;
		int line_height = (int)(TILE_SIZE * SCREEN_HEIGHT / distance);
		int draw_start = - line_height / 2 + SCREEN_HEIGHT / 2;
		int draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
		if (draw_start < 0)
			draw_start = 0;
		if (draw_end >= SCREEN_HEIGHT)
			draw_end = SCREEN_HEIGHT - 1;
		int texture_index;     
		if (ray.side == 0)
		{
			if (ray.ray_dir_x > 0)
				texture_index = EAST_TEXTURE;
			else
				texture_index = WEST_TEXTURE;
		}
		else 
		{
			if (ray.ray_dir_y > 0)
				texture_index = SOUTH_TEXTURE;
			else
				texture_index = NORTH_TEXTURE;
		}
		game->ray = ray;
		int color = (holder->side == 1) ? 0xAAAAAA : 0xFFFFFF;
		draw_floor_and_ceiling(game, i_loop , SCREEN_HEIGHT + 1, draw_start, draw_end,game->floor_color, game->ceiling_color);
		t_txtu *current_texture = &game->txtu[texture_index];
		draw_vertical_line(game, i_loop,draw_start, draw_end, color);
		texture_pass(game, i_loop, draw_start, draw_end, ray, holder, current_texture, line_height);
		i_loop++;
	}
}



/* Mini-map raycaster: draws player and rays on a top-down map instead of 3D view.
   Uses prepare_vars() and cast_dda() already present in your code to compute hits.
   Assumes game->helper is the image buffer (addr, line_len, bpp) as used elsewhere.
*/

#include <stdlib.h>
#include <math.h>

/* Put one pixel into the game->helper image (safe bounds check) */
static void mini_put_pixel(t_game *game, int x, int y, unsigned int color)
{
    if (!game || !game->helper)
        return;
    if (x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
        return;
    char *pixel = game->helper->addr + (y * game->helper->line_len + x * (game->helper->bpp / 8));
    *(unsigned int *)pixel = color;
}

/* Bresenham line drawing between (x0,y0) and (x1,y1) on the minimap */
static void mini_draw_line(t_game *game, int x0, int y0, int x1, int y1, unsigned int color)
{
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy; /* error value e_xy */

    while (1)
    {
        mini_put_pixel(game, x0, y0, color);
        if (x0 == x1 && y0 == y1)
            break;
        int e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}



/* Draw the minimap with rays:
   - Draws player as a small filled square
   - Casts sampled rays across FOV, draws each ray line from player to hit
   - Marks hit points
*/
void draw_minimap_with_rays(t_game *game)
{
    if (!game || !game->helper)
        return;

    /* Configuration: tweak these to change minimap size and ray density */
    const int MINI_TILE = 8;       /* number of minimap pixels per map tile */
    const int MAP_OFFSET_X = 10;   /* minimap top-left X on screen */
    const int MAP_OFFSET_Y = 10;   /* minimap top-left Y on screen */
    const int PLAYER_SIZE = 3;     /* half-size of player square in pixels on minimap */
    const int RAY_STEP = 4;        /* ray sampling step across screen (1 = all rays, >1 = sparser) */

    t_ray ray;
    t_casting holder;
    /* initialize holder so cast_dda / loop_helper behave safely */
    holder.hited = 0;
    holder.side = 0;
    holder.side_dist_x = 0;
    holder.side_dist_y = 0;

    /* compute player center in world pixels then convert to minimap pixels */
    double player_world_x = game->player.x * TILE_SIZE + TILE_SIZE / 2.0;
    double player_world_y = game->player.y * TILE_SIZE + TILE_SIZE / 2.0;
    int player_mx = MAP_OFFSET_X + (int)( (player_world_x / (double)TILE_SIZE) * MINI_TILE );
    int player_my = MAP_OFFSET_Y + (int)( (player_world_y / (double)TILE_SIZE) * MINI_TILE );

    /* Draw a small square for the player */
    for (int dy = -PLAYER_SIZE; dy <= PLAYER_SIZE; dy++)
        for (int dx = -PLAYER_SIZE; dx <= PLAYER_SIZE; dx++)
            mini_put_pixel(game, player_mx + dx, player_my + dy, 0x00FF00); /* green player */

    /* Cast rays across the FOV — reuse your prepare_vars and cast_dda.
       We iterate similarly to the_3dview but only sample every RAY_STEP columns
       to avoid over-drawing on the minimap. */
    for (int i_loop = 0; i_loop < SCREEN_WIDTH; i_loop += RAY_STEP)
    {
        /* prepare_vars expects a t_ray by value; it returns populated ray */
        ray = prepare_vars(ray, game, i_loop);

        /* reset holder for each ray */
        holder.hited = 0;
        holder.side = 0;
        holder.side_dist_x = 0;
        holder.side_dist_y = 0;

        /* cast_dda will fill ray.hit_x, ray.hit_y and ray.perp_wall_dist */
        (void)cast_dda(game, &ray, &holder);

        /* Convert hit world coordinates (pixels) into minimap pixels */
        int hit_mx = MAP_OFFSET_X + (int)((ray.hit_x / (double)TILE_SIZE) * MINI_TILE);
        int hit_my = MAP_OFFSET_Y + (int)((ray.hit_y / (double)TILE_SIZE) * MINI_TILE);

        /* Color choice: red for rays, blue for hit points; shade if side (optional) */
        unsigned int ray_color = 0xFF0000;   /* red */
        unsigned int hit_color = 0x0000FF;   /* blue */

        if (holder.side == 1) /* if hit was horizontal wall, dim the colors a bit */
        {
            ray_color = 0x880000;
            hit_color = 0x000088;
        }

        /* Draw the ray line from player center to hit point */
        mini_draw_line(game, player_mx, player_my, hit_mx, hit_my, ray_color);

        /* Mark the hit point with a small cross */
        mini_put_pixel(game, hit_mx, hit_my, hit_color);
        mini_put_pixel(game, hit_mx + 1, hit_my, hit_color);
        mini_put_pixel(game, hit_mx - 1, hit_my, hit_color);
        mini_put_pixel(game, hit_mx, hit_my + 1, hit_color);
        mini_put_pixel(game, hit_mx, hit_my - 1, hit_color);
    }

    /* Optionally: draw the map grid (tile outlines) for reference */
    /* You can uncomment to draw tile borders:
    int map_h = (int)ft_strlen(game->map[0]); // careful: replace with proper width/height
    for (int y = 0; game->map[y]; y++)
    {
        for (int x = 0; game->map[y][x]; x++)
        {
            int tx = MAP_OFFSET_X + x * MINI_TILE;
            int ty = MAP_OFFSET_Y + y * MINI_TILE;
            // draw 4 borders with a subtle color
            for (int i = 0; i < MINI_TILE; i++)
            {
                mini_put_pixel(game, tx + i, ty, 0x202020);                 // top
                mini_put_pixel(game, tx + i, ty + MINI_TILE - 1, 0x202020); // bottom
                mini_put_pixel(game, tx, ty + i, 0x202020);                 // left
                mini_put_pixel(game, tx + MINI_TILE - 1, ty + i, 0x202020); // right
            }
        }
    }
    */

    /* End of minimap */
}
















// loop over each vertical stripe of the screen (x = 0 to SCREEN_WIDTH)
// 1. calculate ray direction based on camera plane and current x
// 2. calculate ray position and direction increment (stepX, stepY)
// 3. calculate initial side distances to first grid line (sideDistX, sideDistY)
// 4. perform DDA loop to find where ray hits a wall
// 5. determine which side was hit (vertical or horizontal wall)
// 6. calculate perpendicular wall distance (to avoid fish-eye effect)
// 7. calculate line height based on distance
// 8. calculate draw start and end positions on screen (clamp inside window)
// 9. choose wall color/texture based on side hit
// 10. draw the vertical line (pixel by pixel or by filling a column)
// 11. (optional) store distance in z-buffer for later use (sprites, etc.)


// t_casting *prepare_cast(t_game *game)
// {
	




// }

// void    the_3dview(t_game *game)
// {
// 	int i_loop;
// 	t_casting *helper;

// 	i_loop = 0;
// 	helper = malloc(sizeof(t_casting));
// 	while (i_loop < SCREEN_WIDTH)
// 	{
// 		//prepare the varibles
// 		helper = prepare_cast(game);
// 		// take the dda





// 		i_loop++;
// 	}
// }
