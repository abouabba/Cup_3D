#include "cub.h"

typedef struct s_casting
{
    int     hited;
    int     side;
    double  side_dist_x;
    double  side_dist_y;
    int     step_x;
    int     step_y;
}   t_casting;  

t_casting    loop_helper(t_game *game, t_ray *ray, t_casting holder)
{
    holder.hited = 0;
    while (!holder.hited)
    {
        if (holder.side_dist_x < holder.side_dist_y)
        {
            holder.side_dist_x += ray->delta_dist_x;
            ray->map_x += holder.step_x;
            holder.side = 0;
        }
        else
        {
            holder.side_dist_y += ray->delta_dist_y;
            ray->map_y += holder.step_y;
            holder.side = 1;
        }
        if (game->map[ray->map_y][ray->map_x] == '1')
            holder.hited = 1;
    }
    return (holder);
}

double cast_dda(t_game *game, t_ray *ray, t_casting holder)
{
    double dist_to_wall;

    if (ray->ray_dir_x < 0)
    {
        holder.step_x = -1;
        holder.side_dist_x = (ray->ray_x / TILE_SIZE - ray->map_x) * ray->delta_dist_x;
    }
    else
    {
        holder.step_x = 1;
        holder.side_dist_x = (ray->map_x + 1.0 - ray->ray_x / TILE_SIZE) * ray->delta_dist_x;
    }
    if (ray->ray_dir_y < 0)
    {
        holder.step_y = -1;
        holder.side_dist_y = (ray->ray_y / TILE_SIZE - ray->map_y) * ray->delta_dist_y;
    }
    else
    {
        holder.step_y = 1;
        holder.side_dist_y = (ray->map_y + 1.0 - ray->ray_y / TILE_SIZE) * ray->delta_dist_y;
    }
    
    holder = loop_helper(game, ray, holder);
    if (holder.side == 0)
        dist_to_wall = (ray->map_x * TILE_SIZE - ray->ray_x + (1 - holder.step_x) * TILE_SIZE / 2) / ray->ray_dir_x;
    else
        dist_to_wall = (ray->map_y * TILE_SIZE - ray->ray_y + (1 - holder.step_y) * TILE_SIZE / 2) / ray->ray_dir_y;

    ray->perp_wall_dist = dist_to_wall;
    ray->side = holder.side;
    ray->hit_x = ray->ray_x + ray->ray_dir_x * dist_to_wall;
    ray->hit_y = ray->ray_y + ray->ray_dir_y * dist_to_wall;

    return (dist_to_wall);    
}

void	draw_vertical_line(t_game *game, int x, int start, int end, int color)
{
	if (x < 0 || x >= SCREEN_WIDTH)
    {
        printf("this is x: %d, start: %d, end: %d \n", x, start, end);
        // printf("(((((((((())))))))))");
        return ;
    }
		
	if (start < 0)
		start = 0;
	if (end >= SCREEN_HEIGHT)
		end = SCREEN_HEIGHT - 12;

	char	*pixel;
	int		y;

	y = start;
	while (y <= end)
	{
		pixel = game->helper->addr + (y * game->helper->line_len + x * (game->helper->bpp / 8));
		*(unsigned int *)pixel = color;
        // my_mlx_pixel_put(game,x, y,color);
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
            side = 0; // Vertical wall
        }
        else
        {
            side_dist_y += ray->delta_dist_y;
            ray->map_y += step_y;
            side = 1; // Horizontal wall
        }

        // Check if ray has hit a wall (map[y][x] == '1')
        if (game->map[ray->map_y][ray->map_x] == '1')
        {
            hit = 1;
        }
    }

    // Calculate perpendicular distance to the wall
    double perp_wall_dist;
    if (side == 0)
        perp_wall_dist = (ray->map_x * TILE_SIZE - ray->ray_x + (1 - step_x) * TILE_SIZE / 2) / ray->ray_dir_x;
    else
        perp_wall_dist = (ray->map_y * TILE_SIZE - ray->ray_y + (1 - step_y) * TILE_SIZE / 2) / ray->ray_dir_y;



    // Optional: Save result in ray struct
    ray->perp_wall_dist = perp_wall_dist;
    ray->side = side;

    // Calculate exact hit point (for drawing or texture mapping)
    ray->hit_x = ray->ray_x + ray->ray_dir_x * perp_wall_dist;
    ray->hit_y = ray->ray_y + ray->ray_dir_y * perp_wall_dist;

    // You can now use ray->hit_x and ray->hit_y to draw your ray line or walls
    // double start_x = game->player.x * TILE_SIZE + TILE_SIZE / 2;
    // double start_y = game->player.y * TILE_SIZE + TILE_SIZE / 2;

    // double end_x = ray->hit_x;
    // double end_y = ray->hit_y;
    // (void)ray_id;

    // printf("hit x and y (tiles):     (%f, %f)\n", ray->hit_x, ray->hit_y);
    // printf("\n=========== RAYCAST DEBUG INFO ===========\n");
    // printf("Player Position (pixels):   (%.2f, %.2f)\n", start_x, start_y);
    // printf("Ray Start (pixels):         (%.2f, %.2f)\n", ray->ray_x, ray->ray_y);
    // printf("Ray Direction:              (%.5f, %.5f)\n", ray->ray_dir_x, ray->ray_dir_y);
    // printf("Tile Map Position:          (%d, %d)\n", ray->map_x, ray->map_y);
    // printf("Step Direction:             (step_x = %d, step_y = %d)\n", (ray->ray_dir_x < 0 ? -1 : 1), (ray->ray_dir_y < 0 ? -1 : 1));
    // printf("Delta Distances:            (dx = %.5f, dy = %.5f)\n", ray->delta_dist_x, ray->delta_dist_y);
    // printf("Initial Side Distances:     (side_dx = %.5f, side_dy = %.5f)\n", side_dist_x, side_dist_y);
    // printf("Wall Hit Side:              %s\n", ray->side == 0 ? "Vertical (X)" : "Horizontal (Y)");
    // printf("Perpendicular Wall Dist:    %.5f\n", ray->perp_wall_dist);
    // printf("Hit Position (pixels):      (%.2f, %.2f)\n", ray->hit_x, ray->hit_y);
    // printf("==========================================\n\n");
    // sleep(1);
    // draw_ray_line(game, start_x, start_y, end_x, end_y, 0xFF0000);
    // draw_vertical_line(game, ray_id, perp_wall_dist, side);
    
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
    angle_step = fov / (double)SCREEN_WIDTH;// to represent every dgree one oriziotal line
    start_angle = normalize_angle(game->angle - fov / 2);

    ray.ray_angle = normalize_angle(start_angle + i_loop * angle_step);
    ray.ray_x  =  game->player.x * TILE_SIZE + TILE_SIZE / 2;
    ray.ray_y  =  game->player.y * TILE_SIZE + TILE_SIZE / 2;
    ray.ray_dir_x = cos(ray.ray_angle);
    ray.ray_dir_y = sin(ray.ray_angle);
    ray.map_x = (int)(ray.ray_x / TILE_SIZE);
    ray.map_y = (int)(ray.ray_y / TILE_SIZE);
    ray.delta_dist_x = (fabs(ray.ray_dir_x) < 1e-6) ? 1e30 : fabs(1 / ray.ray_dir_x);
    ray.delta_dist_y = (fabs(ray.ray_dir_y) < 1e-6) ? 1e30 : fabs(1 / ray.ray_dir_y);
    return ray;
}

// void texture_pass(t_game *game, int screen_x, int draw_start, int draw_end, t_ray ray, t_casting holder, t_txtu *texture)
// {
//     double wall_x;
//     if (holder.side == 0)
//         wall_x = game->player.y + ray.perp_wall_dist * ray.ray_dir_y;
//     else
//         wall_x = game->player.x + ray.perp_wall_dist * ray.ray_dir_x;
//     wall_x -= floor(wall_x);
//     // texture x position
//     int tex_x = (int)(wall_x * (double)(texture->width));
//     if ((holder.side == 0 && ray.ray_dir_x < 0) || (holder.side == 1 && ray.ray_dir_y > 0))
//         tex_x = texture->width - tex_x - 1;

//     int line_height = (int)(TILE_SIZE * SCREEN_HEIGHT / ray.perp_wall_dist);
//     double step = 1.0 * texture->height / line_height;
//     // double tex_pos = (draw_start - SCREEN_HEIGHT / 2 + line_height / 2) * step;
//     double tex_pos = (draw_start - SCREEN_HEIGHT / 2 + line_height / 2) * step;


//     for (int y = draw_start; y < draw_end; y++)
//     {
//         int tex_y = (int)tex_pos % texture->height;



//         char *pixel = texture->data_add + (tex_y * texture->line_len + tex_x * (texture->bbp / 8));
//         int color = *(int *)pixel;

//         my_mlx_pixel_put(game, screen_x, y, color); // Make sure this exists
//     }
// }
void texture_pass(t_game *game, int screen_x, int draw_start, int draw_end,
                  t_ray ray, t_casting holder, t_txtu *texture, int line_height)
{
    double wall_x;

    if (holder.side == 0)
        wall_x = game->player.y + ray.perp_wall_dist * ray.ray_dir_y;
    else
        wall_x = game->player.x + ray.perp_wall_dist * ray.ray_dir_x;
    wall_x -= floor(wall_x);

    int tex_x = (int)(wall_x * (double)(texture->width));
    if ((holder.side == 0 && ray.ray_dir_x < 0) || (holder.side == 1 && ray.ray_dir_y > 0))
        tex_x = texture->width - tex_x - 1;

    double step = 1.0 * texture->height / line_height;
    double tex_pos = (draw_start - SCREEN_HEIGHT / 2 + line_height / 2) * step;

    for (int y = draw_start; y < draw_end; y++)
    {
        int tex_y = (int)tex_pos;
        if (tex_y >= texture->height)
            tex_y = texture->height - 1;
        tex_pos += step;

        char *pixel = texture->data_add + (tex_y * texture->line_len + tex_x * (texture->bbp / 8));
        int color = *(int *)pixel;

        my_mlx_pixel_put(game, screen_x, y, color);
    }
}


void    the_3dview(t_game *game)
{
    t_ray ray;
    int     i_loop;
    t_casting holder;

    i_loop = 0;
    while(i_loop <= SCREEN_WIDTH)
    {
        ray  = prepare_vars(ray, game, i_loop);
        double distance = cast_dda(game, &ray, holder);
        // double corrected_distance = distance * cos(ray.ray_angle - game->angle);
    
        int line_height = (int)(TILE_SIZE * SCREEN_HEIGHT / distance);
        int draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
        int draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
        if (draw_start < 0)
            draw_start = 0;
        if (draw_end >= SCREEN_HEIGHT)
            draw_end = SCREEN_HEIGHT - 1;
        // holder.side if == 1 is hit the y axess 0 if hited the x axess
        int texture ;
        if (holder.side == 0) // vertical wall (X-axis side)
        {
            if (ray.ray_dir_x > 0)
                texture = EAST_TEXTURE;  // ray moving right
            else
                texture = WEST_TEXTURE;  // ray moving left
        }
        else // horizontal wall (Y-axis side)
        {
            if (ray.ray_dir_y > 0)
                texture = SOUTH_TEXTURE; // ray moving down
            else
                texture = NORTH_TEXTURE; // ray moving up
        }
        // int color = texture_pass(game, ray, texture, holder);
        int texture_index;
        if (holder.side == 0)
            texture_index = (ray.ray_dir_x > 0) ? EAST_TEXTURE : WEST_TEXTURE;
        else
            texture_index = (ray.ray_dir_y > 0) ? SOUTH_TEXTURE : NORTH_TEXTURE;
            
        t_txtu *current_texture = &game->txtu[texture_index];
        // texture_pass(game, i_loop, draw_start, draw_end, ray, holder, current_texture);
        texture_pass(game, i_loop, draw_start, draw_end, ray, holder, current_texture, line_height);

        // draw_vertical_line(game, i_loop, draw_start, draw_end, color);
        i_loop++;
    }
}
