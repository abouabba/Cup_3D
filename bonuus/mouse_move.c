#include "cub.h"

int mouse_move_hook(int x, int y, t_game *game)
{
	static int last_x = -1;
	int delta_x;
	(void)y;

	if (last_x == -1)
		last_x = x;

	delta_x = x - last_x;

	if (delta_x != 0)
	{
		game->angle += delta_x * ROTATION_SPEED;
		if (game->angle < 0)
				game->angle += 2 * M_PI;
		if (game->angle >= 2 * M_PI)
				game->angle -= 2 * M_PI;

		game->player.dir_x = cos(game->angle);
		game->player.dir_y = sin(game->angle);
	}
	last_x = x;
	return (0);
}
