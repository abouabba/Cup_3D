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


// void texture_pass(t_game *game, int screen_x, int draw_start, int draw_end, t_ray ray, t_casting *holder, t_txtu *texture, int line_height)
// {
//     double wall_x;
//     if (ray.side == 0)
//         wall_x = (ray.hit_y / TILE_SIZE) - floor(ray.hit_y / TILE_SIZE);
//     else
//         wall_x = (ray.hit_x / TILE_SIZE) - floor(ray.hit_x / TILE_SIZE);

//     int tex_width = texture->width;
//     int tex_x = (int)(wall_x  * (double)tex_width);

//     if ((ray.side == 0 && ray.ray_dir_x > 0) || 
//         (ray.side == 1 && ray.ray_dir_y < 0))
//     {
//         tex_x = tex_width - tex_x - 1;
//     }
//     if (line_height < 1)
//         line_height = 1;
//     if (draw_end >= SCREEN_HEIGHT)
// 		draw_end = SCREEN_HEIGHT - 1;
//     if (draw_start < 0)
// 		draw_start = 0;
//     int tex_height = texture->height;
//     double step = 1.0 * tex_height / line_height;
//     double tex_pos = (draw_start - SCREEN_HEIGHT / 2 + line_height / 2) * step;
    
//     for (int y = draw_start; y < draw_end; y++)
//     {
//         int tex_y = (int)tex_pos;
//         if (tex_y >= tex_height)
//             tex_y = 0;
//         tex_pos += step;

//         char *pixel = texture->data_add + 
//                       (tex_y * texture->line_len + tex_x * (texture->bbp / 8));
//         int color = *(int *)pixel;

//         my_mlx_pixel_put(game, screen_x, y, color);
//     }
// }

// void	texture_pass(t_game *game, int x, int start, int end,
// 					t_ray ray, t_casting *holder, t_txtu *texture, int line_height)
// {
// 	if (x < 0 || x >= SCREEN_WIDTH)
// 		return;

// 	if (start < 0)
// 		start = 0;
// 	if (end >= SCREEN_HEIGHT)
// 		end = SCREEN_HEIGHT - 1;
//     double projection_plane_dist = (SCREEN_WIDTH / 2.0) / tan(FOV / 2.0);
//     double wall_stirp =  TILE_SIZE / ray.perp_wall_dist * projection_plane_dist;
//     double wall_hight = (int)(SCREEN_HEIGHT / ray.perp_wall_dist);
//     double wall_top = (SCREEN_HEIGHT /2) - (wall_hight /2);
//     double wall_bottom = (SCREEN_HEIGHT /2) + (wall_hight /2);
//     printf("thsi is the data projection_plane_dist:%f   wall_strip:%f   wall_height:%f, wall_top:%f, wall_bottom:%f\n", projection_plane_dist,wall_stirp, wall_hight, wall_top, wall_bottom);
// }
void texture_pass(t_game *game, int x, int start, int end,
                  t_ray ray, t_casting *holder, t_txtu *texture, int line_height)
{
    if (x < 0 || x >= SCREEN_WIDTH)
        return;
    if (start < 0)
        start = 0;
    if (end >= SCREEN_HEIGHT)
        end = SCREEN_HEIGHT - 1;

    int tex_width = texture->width;
    int tex_height = texture->height;

    // Determine exact hit location on the wall
    double wall_x;
    if (ray.side == 0)
        wall_x = ray.hit_y / TILE_SIZE - floor(ray.hit_y / TILE_SIZE);
    else
        wall_x = ray.hit_x / TILE_SIZE - floor(ray.hit_x / TILE_SIZE);

    int tex_x = (int)(wall_x * tex_width);
    if ((ray.side == 0 && ray.ray_dir_x > 0) || (ray.side == 1 && ray.ray_dir_y < 0))
        tex_x = tex_width - tex_x - 1;

    // Step: how much to increase the texture coordinate per screen pixel
    double step = (double)tex_height / (double)line_height;

    // Starting texture coordinate (shift so middle of wall aligns properly)
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

        // Shade if side==1 (horizontal wall)
        if (ray.side == 1)
            color = (color >> 1) & 0x7F7F7F;

        my_mlx_pixel_put(game, x, y, color);
    }
}


void	draw_floor_and_ceiling(t_game *img, int x, int screen_height, int draw_start, int draw_end, int floor_color, int ceiling_color)
{
	for (int y = 0; y < draw_start; y++) // ceiling
		my_mlx_pixel_put(img, x, y, ceiling_color);
	for (int y = draw_end; y < screen_height; y++) // floor
		my_mlx_pixel_put(img, x, y, floor_color);
}

void    the_3dview(t_game *game)
{
    t_ray ray;
    int     i_loop;
    t_casting *holder;

    holder = malloc(sizeof(t_casting));
    holder->hited =0;
    holder->side =0;
    holder->side_dist_x =0;
    holder->side_dist_y =0;
    i_loop = 0;
    
    while(i_loop <= SCREEN_WIDTH)
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

        // holder.side if == 1 is hit the y axess 0 if hited the x axess
        int texture_index;     
        if (ray.side == 0) // vertical wall (X-axis side)
        {
            if (ray.ray_dir_x > 0)
                texture_index = EAST_TEXTURE;  // ray moving right
            else
                texture_index = WEST_TEXTURE;  // ray moving left
        }
        else // horizontal wall (y-axis side)
        {
            if (ray.ray_dir_y > 0)
                texture_index = SOUTH_TEXTURE; // ray moving down
            else
                texture_index = NORTH_TEXTURE; // ray moving up
        }
        int color = (holder->side == 1) ? 0xAAAAAA : 0xFFFFFF;
    

        draw_floor_and_ceiling(game, i_loop , SCREEN_HEIGHT, draw_start, draw_end, 0xFFDFFFF, 0x0A0D00);
        t_txtu *current_texture = &game->txtu[texture_index];
        draw_vertical_line(game, i_loop,draw_start, draw_end, color);
        texture_pass(game, i_loop, draw_start, draw_end, ray, holder, current_texture, line_height);
        i_loop++;
    }
}
