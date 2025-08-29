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
	game->player.x = game->player.y + 0.5;
	game->player.y = game->player.x + 0.5;
}

void move_player(t_game *game, int dx, int dy)
{
	int new_x = game->player.x + dx;
	int new_y = game->player.y + dy;

	char next_tile = game->map[new_y][new_x];

	if (next_tile == '1')
		return;
	if (next_tile == 'C')
		game->player.c_count--;
	if (next_tile == 'E' && game->player.c_count != 0)
		return;
	if (next_tile == 'E' && game->player.c_count == 0)
		exit(1);

	game->map[game->player.y][game->player.x] = '0';
	game->player.x = new_x;
	game->player.y = new_y;
	game->map[new_y][new_x] = 'P';
	game->player.move++;
	ft_putnbr(game->player.move);
	ft_putchar('\n');
}
 
#define ROTATION_SPEED 0.5

int bottoms(int keycode, t_game *game)
{
	if (keycode == LEFT)
	    game->angle -= ROTATION_SPEED;
	else if (keycode == RIGHT)
	    game->angle += ROTATION_SPEED;
	if ( keycode == KEY_D)
		move_player(game, 0, -1);
	else if (keycode == KEY_W )
		move_player(game, 0, 1);
	else if (keycode == KEY_A)
		move_player(game, -1, 0);
	else if (keycode == KEY_S )
		move_player(game, 1, 0);
	else if (keycode == 65307)
		exit(0);
	
	mlx_clear_window(game->helper->mlx, game->helper->win);
	render_map(game);
	return 0;
}


