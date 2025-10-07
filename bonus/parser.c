#include "cub.h"

int	invalid_rgb(int r, int g, int b)
{
	return (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255);
}

void	free_split(char **arry)
{
	int	i;

	i = 0;
	while (arry[i])
	{
		free (arry[i]);
		i++;
	}
	free (arry);
}


int	is_digit_str(char *s)
{
	while (*s)
	{
		if (!ft_isdigit(*s))
		return (0);
		s++;
	}
	return (1);
}

int	parse_color(char *line)
{
	int i = 0;
	char **split;
	int r = 0;
	int g = 0;
	int b = 0;
	while (*line == ' ')
	line++;
	int j = 0;
	while (line[i])
	{
		if (line[i] == ',')
			j++;
		i++;
	}
	i = 0;
	if (j != 2)
	{
		perror("Error\n<---- Invalid color format1 ---->");
		exit (1);
	}
	split = ft_split(line, ',');
	if (!split)
	{
		perror("Error\n<---- Invalid color format2 ---->");
		exit (1);
	}
	while (split[i])
		i++;
	if (i != 3)
	{
		printf ("----------------------------->\n");
		perror("Error\n<---- Invalid color format3 ---->");
		free_split(split);
		exit (1);
	}
	r = ft_atoi(split[0]);
	g = ft_atoi(split[1]);
	b = ft_atoi(split[2]);
	int len = ft_strlen(split[2]);
	if (split[2][len - 1] == '\n')
	split[2][len - 1] = '\0';
	printf ("r = [%s],g = [%s],b = [%s]\n", split[0], split[1], split[2]);

	if (!is_digit_str(split[0]) || !is_digit_str(split[1]) || !is_digit_str(split[2]))
	{
		free_split(split);
		print_error("Error: RGB values must contain only digits");
	}
	if (invalid_rgb(r, g, b))
	{
		perror("Error\n<---- Invalid color format4 ---->");
		free_split(split);
		exit (1);
	}
	free_split(split);
	return ((r << 16) | (g << 8) | b);
}

void	parse_line(t_game *game, char *line)
{
	if (is_config_line(line))
		parse_config_line(game, line);
	else if (is_empty_line(line))
	{
		if (game->map_started)
			game->map_ended = 1;
	}
	else
	{
		if (game->map_ended)
			print_error("Error: Non-empty map line after empty line (split map)");
		game->map_started = 1;
		save_map_line(game, line); 
	}
}

int	is_empty_line(char *line)
{
	while (*line)
	{
		if (*line != '\n')
			return (0);
		line++;
	}
	return (1);
}
