#include "cub.h"

void	print_error(const char *message)
{
	fprintf(stderr, "Error: %s\n", message);
	exit(EXIT_FAILURE);
}

int	is_valid_char(char c)
{
	return (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == ' ' || c == 'D');
}

int	is_player_char(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

void	validate_map(t_game *game)
{
	int	y = 0;
	int	player_count = 0;

	while (y < game->map_height)
	{
		int	x = 0;
		while (x < game->map_width)
		{
			char c = game->map[y][x];

			if (!is_valid_char(c))
            {
                fprintf(stderr, "Error: Invalid character '%c' at [%d][%d]\n", c, y, x);
                exit(EXIT_FAILURE);
            }

			if (is_player_char(c))
			{
				player_count++;
				game->player.x = x + 0.5;
				game->player.y = y + 0.5;
				game->player.direction = c;
			}

			if ((c == '0' || is_player_char(c)) && (
				y == 0 || x == 0 || y >= game->map_height - 1 || x >= game->map_width - 1 ||
				game->map[y - 1][x] == ' ' || game->map[y + 1][x] == ' ' ||
				game->map[y][x - 1] == ' ' || game->map[y][x + 1] == ' '))
			{
				print_error("Map is not closed");
			}
			if (c == 'D')
			{
				int vertical = (game->map[y - 1][x] == '1' && game->map[y + 1][x] == '1');
				int horizontal = (game->map[y][x - 1] == '1' && game->map[y][x + 1] == '1');
				if (y == 0 || x == 0 || y >= game->map_height - 1 || x >= game->map_width - 1)
					print_error("Door on map border");

				if (!(vertical || horizontal))
					print_error("Door must be placed between two walls (either vertical or horizontal)");

				if (game->map[y - 1][x] == ' ' || game->map[y + 1][x] == ' ' ||
					game->map[y][x - 1] == ' ' || game->map[y][x + 1] == ' ')
					print_error("Door cannot touch empty space");
			}
			x++;
		}
		y++;
	}

	if (player_count != 1)
		print_error("There must be exactly one player");
}
