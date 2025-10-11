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
void move_player(t_game *game, double dx, double dy)
{
    double new_x = game->player.x + (dx / TILE_SIZE) *MOVE_SPEED;
    double new_y = game->player.y + (dy / TILE_SIZE)* MOVE_SPEED;

    int map_x = (int)new_x;
    int map_y = (int)new_y;

    // printf("this is the next DX|%f|   DY|%f|\n", new_x, new_y);
    if (game->map[map_y][map_x] != '1') // check wall
    {
        game->player.x = new_x;
        game->player.y = new_y;
    }
}


int bottoms(int keycode, t_game *game)
{
    double dx = 0.0;
    double dy = 0.0;

    if (keycode == LEFT)
        game->angle -= ROTATION_SPEED;
    else if (keycode == RIGHT)
        game->angle += ROTATION_SPEED;

    if (game->angle < 0)
        game->angle += 2.0 * M_PI;
    if (game->angle >= 2.0 * M_PI)
        game->angle -= 2.0 * M_PI;

    if (keycode == KEY_W) 
	{
        dx = cos(game->angle);
        dy = sin(game->angle);
    }
	else if (keycode == KEY_S) { // backward
        dx = -cos(game->angle);
        dy = -sin(game->angle);
    } else if (keycode == KEY_D) { // strafe left
        dx = -sin(game->angle);
        dy =  cos(game->angle);
    } else if (keycode == KEY_A) { // strafe right
        dx =  sin(game->angle);
        dy = -cos(game->angle);
    } else if (keycode == 49)
        game->frame = 14;
    else if (keycode == 53) {
        exit(0);
    }
    else 
            return 0;

    move_player(game, dx, dy);
    mlx_clear_window(game->helper->mlx, game->helper->win);
    // segfault
    // render_map(game); 
    rander(game); 

    return 0;
}
