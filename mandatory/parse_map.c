# include "cub.h"

int get_max_map_width(t_line *lines)
{
    int max = 0;
    while (lines)
    {
        int len = ft_strlen(lines->line);
        if (len > max)
            max = len;
        lines = lines->next;
    }
    return max;
}

int get_map_height(t_line *lines)
{
    int count = 0;
    while (lines)
    {
        count++;
        lines = lines->next;
    }
    return count;
}

void print_map(char **map)
{
	int i = 0;

	while (map[i])
	{
		printf("%s", map[i]);
		i++;
	}
}

void finalize_map(t_game *game)
{
    game->map_height = get_map_height(game->map_lines);
    game->map_width = get_max_map_width(game->map_lines);

    game->map = malloc(sizeof(char *) * (game->map_height + 1));
    if (!game->map)
	{
		perror("Map allocation failed");
        return ;
	}

    t_line *current = game->map_lines;
    int i = 0;
    while (current)
    {
        game->map[i] = malloc(game->map_width + 1);
        ft_memset(game->map[i], ' ', game->map_width);
        ft_memcpy(game->map[i], current->line, ft_strlen(current->line));
        game->map[i][game->map_width] = '\0';

        current = current->next;
        i++;
    }
    game->map[i] = NULL;
}
