#include "cub.h"

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr(int n)
{
	if (n == -2147483648)
	{
		write(1, "-2147483648", 11);
		return;
	}
	if (n < 0)
	{
		ft_putchar('-');
		n = -n;
	}
	if (n >= 10)
		ft_putnbr(n / 10);
	ft_putchar((n % 10) + '0');
}

void update_position(t_game *game)
{
	game->player.x = game->player.x + 0.5;
	game->player.y = game->player.y + 0.5;
}

#define MOVE_SPEED 1.5
// Updated move_player: works with fractional movement and sliding
void move_player(t_game *game, double dx, double dy)
{
    // Future position (world coords)
    double new_x = game->player.x + dx * MOVE_SPEED;
    double new_y = game->player.y + dy * MOVE_SPEED;

    // Convert to map grid indices
    int map_x = new_x;
    int map_y =  new_y ;

    // Current map position
    int curr_x = (int)(game->player.x / TILE_SIZE);
    int curr_y = (int)(game->player.y / TILE_SIZE);

    printf("Trying to move to [%d][%d]\n", map_y, map_x);

    // Check collision
    printf("this is the %c\n",game->map[map_y][map_x] );
    if (game->map[map_y][map_x] != '1') {
        game->player.x = new_x;
        game->player.y = new_y;
    }
}

#define ROTATION_SPEED 0.09


int bottoms(int keycode, t_game *game)
{
    double dx = 0.0;
    double dy = 0.0;

    if (keycode == LEFT)
        game->angle -= ROTATION_SPEED;
    else if (keycode == RIGHT)
        game->angle += ROTATION_SPEED;

    // normalize angle into [0, 2*PI)
    if (game->angle < 0)
        game->angle += 2.0 * M_PI;
    if (game->angle >= 2.0 * M_PI)
        game->angle -= 2.0 * M_PI;

    if (keycode == KEY_W) 
	{ // forward
        dx = cos(game->angle) * 1;
        dy = sin(game->angle) * 1;
    }
	if (keycode == KEY_S) { // backward
        dx = -cos(game->angle) * 1;
        dy = -sin(game->angle) * 1;
    } if (keycode == KEY_D) { // strafe left
        dx = -sin(game->angle) * 1;
        dy =  cos(game->angle) * 1;
    } if (keycode == KEY_A) { // strafe right
        dx =  sin(game->angle) * 1;
        dy = -cos(game->angle) * 1;
    } if (keycode == 65307) {
        exit(0);
    }

    move_player(game, dx, dy);

    mlx_clear_window(game->helper->mlx, game->helper->win);
    render_map(game); // ensure this uses player.x/player.y for drawing
    return 0;
}
