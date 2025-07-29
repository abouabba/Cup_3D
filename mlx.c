#include "cub.h"


void init_game(t_game *game)
{
    game->helper = malloc(sizeof(t_mlx));
    game->player.c_count = 0;
    game->player.move = 0;
	game->helper->mlx = mlx_init();
	if (!game->helper->mlx)
		exit(write(2, "Error initializing mlx\n", 24));
	game->helper->win = mlx_new_window(game->helper->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "Cub3D");
	game->helper->img = mlx_new_image(game->helper->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	game->helper->addr = mlx_get_data_addr(game->helper->img, &game->helper->bpp, &game->helper->line_len, &game->helper->endian);
}

int close_window(t_game *game)
{
    (void)game;
    // need to free all of the 
    exit(1);
}

void my_mlx_pixel_put(t_game *game, int x, int y, int color)
{
	if (x < 0 || y < 0 || x >= game->map_width * TILE_SIZE || y >= game->map_height * TILE_SIZE)
		return;

	char *dst = game->helper->addr + (y * game->helper->line_len + x * (game->helper->bpp / 8));
	*(unsigned int *)dst = color;
}

// void render_tile(t_game *cub, int tile_y,int tile_x,  int color)
// {
//     int dx = 0;
//     int dy = 0;

//     while (dy < TILE_SIZE)
//     {
//         dx = 0;
//         while (dx < TILE_SIZE)
//         {
//             int pixel_x = tile_x * TILE_SIZE + dx;
//             int pixel_y = tile_y * TILE_SIZE + dy;
//             my_mlx_pixel_put(cub, pixel_x, pixel_y, color);
//             dx++;
//         }
//         dy++;
//     }
// }

// void draw_player_dot(t_game *game, double px, double py, int color)
// {
//     int size = TILE_P;
//     for (int dy = -size; dy <= size; dy++)
//     {
//         for (int dx = -size; dx <= size; dx++)
//         {
//             my_mlx_pixel_put(game, px + dx, py + dy, color);
//         }
//     }
// }

// void set_player_angle(t_game *player, char dir)
// {
//     if (dir == 'N')
//         player->angle = 3 * M_PI / 2;
//     else if (dir == 'S')
//         player->angle = M_PI / 2;
//     else if (dir == 'E')
//         player->angle = 0;
//     else if (dir == 'W')
//         player->angle = M_PI;
// }


int  render_map(t_game *game)
{

	mlx_destroy_image(game->helper->mlx, game->helper->img);
	mlx_clear_window(game->helper->addr, game->helper->win);
	game->helper->img = mlx_new_image(game->helper->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	game->helper->addr = mlx_get_data_addr(game->helper->img, &game->helper->bpp, &game->helper->line_len, &game->helper->endian);

    the_3dview(game);
    mlx_put_image_to_window(game->helper->mlx, game->helper->win, game->helper->img, 0, 0);
    return (1);
}


