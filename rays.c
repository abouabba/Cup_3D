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


// void texture_pass(t_game *game, int screen_x, int draw_start, int draw_end,
//                   t_ray ray, t_casting holder, t_txtu *texture, int line_height)
// {

//     // double wall_x;
//     // if (ray.side == 0)
//     //     wall_x = ray.ray_y - floor(ray.ray_y); // hit vertical wall
//     // else
//     //     wall_x = ray.ray_x - floor(ray.ray_x);
//     // printf("Ray hit at X: %f | Y: %f\n", ray.ray_x, ray.ray_y);
//     // printf("\n--- Texture Debug ---\n");
//     // printf("Screen column (x): %d\n", screen_x);
//     // printf("Draw start: %d | Draw end: %d\n", draw_start, draw_end);
//     // printf("Line height: %d\n", line_height);
//     // printf("Texture width: %d | Texture height: %d\n", texture->width, texture->height);
//     // printf("Wall hit position (wall_x): %f\n", wall_x);

//     // int tex_x = (int)(wall_x * texture->width);
//     // if ((holder.side == 0 && ray.ray_dir_x < 0) || (holder.side == 1 && ray.ray_dir_y > 0))
//     //     tex_x = texture->width - tex_x - 1;

//     // printf("Calculated tex_x: %d\n", tex_x);
//     // printf("Ray dir X: %f | Ray dir Y: %f\n", ray.ray_dir_x, ray.ray_dir_y);
//     // printf("Wall side (0=X, 1=Y): %d\n", holder.side);

    

    

    

    

//     // // double step = (double)texture->height / line_height;
//     // // double tex_pos = (draw_start - SCREEN_HEIGHT / 2 + line_height / 2) * step;

//     // // for (int y = draw_start; y < draw_end; y++)
//     // // {
//     // //     int tex_y = (int)tex_pos & (texture->height - 1);
//     // //     tex_pos += step;

//     // //     char *pixel = texture->data_add + (tex_y * texture->line_len + tex_x * (texture->bbp / 8));
//     // //     int color = *(int *)pixel;

//     // //     my_mlx_pixel_put(game, screen_x , y, color);
//     // // }

//     // double wall_x;
//     // if (holder.side == 0)  // or ray.side if you prefer
//     //     wall_x = ray.ray_y - floor(ray.ray_y); // vertical wall hit
//     // else
//     //     wall_x = ray.ray_x - floor(ray.ray_x);

//     // // printf("Ray hit at X: %f | Y: %f\n", ray.ray_x, ray.ray_y);
//     // // printf("\n--- Texture Debug ---\n");
//     // // printf("Screen column (x): %d\n", screen_x);
//     // // printf("Draw start: %d | Draw end: %d\n", draw_start, draw_end);
//     // // printf("Line height: %d\n", line_height);
//     // // printf("Texture width: %d | Texture height: %d\n", texture->width, texture->height);
//     // // printf("Wall hit position (wall_x): %f\n", wall_x);

//     // int tex_x = (int)(wall_x * texture->width);
//     // if ((holder.side == 0 && ray.ray_dir_x < 0) || (holder.side == 1 && ray.ray_dir_y > 0))
//     //     tex_x = texture->width - tex_x - 1;

//     // printf("Calculated tex_x: %d\n", tex_x);
//     // printf("Ray dir X: %f | Ray dir Y: %f\n", ray.ray_dir_x, ray.ray_dir_y);
//     // printf("Wall side (0=X, 1=Y): %d\n", holder.side);

//     // double step = (double)texture->height / line_height;
//     // double tex_pos = (draw_start - SCREEN_HEIGHT / 2 + line_height / 2) * step;

//     // for (int y = draw_start; y < draw_end; y++)
//     // {
//     //     int tex_y = (int)tex_pos & (texture->height - 1);
//     //     tex_pos += step;

//     //     char *pixel = texture->data_add+ (tex_y * texture->line_len + tex_x * (texture->bbp / 8));
//     //     int color = *(int *)pixel;

//     //     my_mlx_pixel_put(game, screen_x, y, color);
//     // }




//     //     // printf("this is the sec_x:%d, drawstart:%d, draw_end:%d ,linelienght :%d\n", screen_x, draw_start, draw_end, line_height);


//     tex_x = (int)(wall_x * (double)(texture->width));
//     if ((ray.side == 0 && ray.ray_dir_x < 0) || (ray.side == 1 && ray.ray_dir_y > 0))
//         tex_x = texture->width - tex_x - 1;

//     double step = 1.0 * texture->height / line_height;
//     printf("the texture hight: %d and steps is %f anf this is he line light %d \n", texture->height, step, line_height);
//     double tex_pos = (draw_start - SCREEN_HEIGHT / 2 + line_height / 2) * step;

//     for (int y = draw_start; y < draw_end; y++)
//     {
//         int tex_y = (int)tex_pos;
//         if (tex_y < 0)
//             tex_y = 0;
//         if (tex_y >= texture->height)
//             tex_y = texture->height - 1;
    
//         tex_pos += step;
    
//         char *pixel = texture->data_add + (tex_y * texture->line_len + tex_x * (texture->bbp / 8));
//         int color = *(int *)pixel;
    
//         my_mlx_pixel_put(game, screen_x, y, color);
//     }
// }


// void texture_pass(t_game *game, int screen_x, int draw_start, int draw_end, t_ray ray, t_casting holder, t_txtu *texture, int line_height)
// {
//     double wall_x;
//     if (ray.side == 0)
//         wall_x = (ray.hit_y / TILE_SIZE) - floor(ray.hit_y / TILE_SIZE);
//     else
//         wall_x = (ray.hit_x / TILE_SIZE) - floor(ray.hit_x / TILE_SIZE);
//     int tex_width = texture->width;
//     int tex_x = (int)(wall_x * (double)tex_width);
//     if ((ray.side == 0 && ray.ray_dir_x > 0) || 
//     (ray.side == 1 && ray.ray_dir_y < 0))
//     {
//         tex_x = tex_width - tex_x - 1;
//     }
//     printf("Texture add: %p\n", texture->data_add);
//     printf("Texture width: %d\n", tex_width);
//     printf("wall_x: %.6f\n", wall_x);
//     printf("tex_x: %d\n", tex_x);

//     int tex_y;
//     int d;
//     line_height = draw_end - draw_start;
//     int tex_height = texture->height;
//     // Start the texture position
//     double step = 1.0 * tex_height / line_height;
//     double tex_pos = (draw_start - SCREEN_HEIGHT / 2 + line_height / 2) * step;

//     for (int y = draw_start; y < draw_end; y++)
//     {
//         tex_y = (int)tex_pos & (tex_height - 1);
//         tex_pos += step;

//         // Get color from texture pixel
//         char *pixel = texture->data_add + 
//                       (tex_y * texture->line_len + tex_x * (texture->bbp / 8));

//         int color = *(int *)pixel;

//         // Plot to screen buffer at (x, y)
//         my_mlx_pixel_put(game, screen_x, y, color);
//     }
// }
void texture_pass(t_game *game, int screen_x, int draw_start, int draw_end, t_ray ray, t_casting holder, t_txtu *texture, int line_height)
{
    double wall_x;
    if (ray.side == 0)
        wall_x = (ray.hit_y / TILE_SIZE) - floor(ray.hit_y / TILE_SIZE);
    else
        wall_x = (ray.hit_x / TILE_SIZE) - floor(ray.hit_x / TILE_SIZE);

    int tex_width = texture->width;
    int tex_x = (int)(wall_x * (double)tex_width);

    if ((ray.side == 0 && ray.ray_dir_x > 0) || 
        (ray.side == 1 && ray.ray_dir_y < 0))
    {
        tex_x = tex_width - tex_x - 1;
    }

    printf("Texture add: %p\n", texture->data_add);
    printf("Texture width: %d, height: %d\n", texture->width, texture->height);
    printf("wall_x: %.6f, tex_x: %d\n", wall_x, tex_x);
    printf("draw_start: %d, draw_end: %d, line_height: %d\n", draw_start, draw_end, line_height);

    int tex_height = texture->height;
    double step = 1.0 * tex_height / line_height;
    double tex_pos = (draw_start - SCREEN_HEIGHT / 2 + line_height / 2) * step;

    for (int y = draw_start; y < draw_end; y++)
    {
        int tex_y = (int)tex_pos;
        if (tex_y >= tex_height)
            tex_y = tex_height - 1;
        tex_pos += step;

        char *pixel = texture->data_add + 
                      (tex_y * texture->line_len + tex_x * (texture->bbp / 8));
        int color = *(int *)pixel;

        my_mlx_pixel_put(game, screen_x, y, color);
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
    t_casting holder;

    holder.hited =0;
    holder.side =0;
    holder.side_dist_x =0;
    holder.side_dist_y =0;
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
        int texture_index;     
        if (ray.side == 0) // vertical wall (X-axis side)
        {
            if (ray.ray_dir_x > 0)
                texture_index = EAST_TEXTURE;  // ray moving right
            else
                texture_index = WEST_TEXTURE;  // ray moving left
        }
        else // horizontal wall (Y-axis side)
        {
            if (ray.ray_dir_y > 0)
                texture_index = SOUTH_TEXTURE; // ray moving down
            else
                texture_index = NORTH_TEXTURE; // ray moving up
        }
        // int color = (holder.side == 1) ? 0xAAAAAA : 0xFFFFFF;
        // printf("this is the sidde :%d (EAST :0 WEST :2 SOUTH :3 NORTH :1) \n", texture_index);

        draw_floor_and_ceiling(game, i_loop , SCREEN_HEIGHT, draw_start, draw_end, 0xFFDFFFF, 0x0000D00);


        t_txtu *current_texture = &game->txtu[texture_index];
        printf("this is he side indx :%d\n", texture_index);
        texture_pass(game, i_loop, draw_start, draw_end, ray, holder, current_texture, line_height);
        i_loop++;
    }
}
// int draw_column(t_ray *ray, t_game *game, int x)
// {
//     int y;
//     // int color;
//     t_img *texture;

//     texture = get_dir_texture(ray, game->textures);
//     // 3. Sécuriser draw_start / draw_end
//     if (ray->column.draw_start < 0)
//         ray->column.draw_start = 0;
//     if (ray->column.draw_end >= HEIGHT)
//         ray->column.draw_end = HEIGHT - 1;
//     if (initialize_texture(ray, texture))
//         return (-1);
//     // 5. Boucle de dessin

//     y = ray->column.draw_start;
//     while (y <= ray->column.draw_end)
//     {
//         /**
//          * À chaque y, on :
//          * Cherche le pixel correspondant dans l’image (via tex_x, tex_y),
//          * Affiche ce pixel à l’écran.
//         */
//         texture->tex_y = (int)texture->tex_pos;
//         if (texture->tex_y < 0)
//             texture->tex_y = 0;
//         if (texture->tex_y >= texture->height)
//             texture->tex_y = texture->height - 1; // & pour rester dans l’image (si height est puissance de 2)
//         texture->tex_pos += texture->step;
//         ray->column.color = get_texture_pixels(texture, texture->tex_x, texture->tex_y); // lecture couleur
//         ray->column.color = apply_shadow(ray->column.color, ray->perp_wall_dist, ray->side);
//         if (put_pixel(&game->mlx.screen, x, y, ray->column.color))
//             return (print_err("Ray casting error\n"));
//         y++;
//     }
//     return (0);
// }