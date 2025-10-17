#include "cub.h"

int	is_config_line(char *line)
{
	while (*line == ' ')
		line++;
	return (
		ft_strncmp(line, "NO ", 3) == 0 ||
		ft_strncmp(line, "SO ", 3) == 0 ||
		ft_strncmp(line, "WE ", 3) == 0 ||
		ft_strncmp(line, "EA ", 3) == 0 ||
		ft_strncmp(line, "F ", 2) == 0 ||
		ft_strncmp(line, "C ", 2) == 0
	);
}

void	parse_config_line(t_game *game, char *line)
{
	while (*line == ' ')
		line++;
	if (ft_strncmp(line, "NO ", 3) == 0)
	{
		game->textures.north = ft_strdup(line + 3);
		game->no++;
	}
	else if (ft_strncmp(line, "SO ", 3) == 0)
	{	
		game->textures.south = ft_strdup(line + 3);
		game->so++;
	}
	else if (ft_strncmp(line, "WE ", 3) == 0)
	{
		game->textures.west = ft_strdup(line + 3);
		game->we++;
	}
	else if (ft_strncmp(line, "EA ", 3) == 0)
	{
		game->textures.east = ft_strdup(line + 3);
		game->ea++;
	}
	else if (ft_strncmp(line, "F ", 2) == 0)
	{
		game->floor_color = parse_color(line + 2);
		game->f++;
	}
	else if (ft_strncmp(line, "C ", 2) == 0)
	{
		game->ceiling_color = parse_color(line + 2);
		game->c++;
	}
}
