/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 15:43:38 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/18 15:48:51 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

void	is_valid_texture(t_game *game)
{
	if (game->no != 1 || game->so != 1
		|| game->we != 1 || game->ea != 1
		|| game->f != 1 || game->c != 1)
	{
		print_error("Invalid texture");
	}
}

t_txtu	init_txtu(t_game *game, char *file)
{
	t_txtu	txtu;

	if (file == NULL)
		return (printf("passing in NULL file \n"), exit(1), txtu);
	file[ft_strlen(file) - 1] = '\0';
	if (access(file, F_OK))
	{
		printf("wrong access: |%s|\n", file);
		exit(1);
	}
	txtu.txture_p = mlx_xpm_file_to_image(game->helper->addr,
			file, &txtu.width, &txtu.height);
	if (!txtu.txture_p)
	{
		printf("texttttuer error \n");
		exit(1);
	}
	txtu.data_add = mlx_get_data_addr(txtu.txture_p,
			&txtu.bbp, &txtu.line_len, &txtu.endian);
	if (!txtu.data_add)
	{
		printf("data address error \n");
		exit(1);
	}
	return (txtu);
}

void	hookshandeling(t_game *game)
{
	is_valid_texture(game);
	finalize_map(game);
	validate_map(game);
	init_game(game);
	game->txtu[0] = init_txtu(game, game->textures.east);
	game->txtu[1] = init_txtu(game, game->textures.north);
	game->txtu[2] = init_txtu(game, game->textures.south);
	game->txtu[3] = init_txtu(game, game->textures.west);
	load_gun_sprite(game);
	mlx_hook(game->helper->win, 17, 0, close_window, game);
	mlx_hook(game->helper->win, 6, 1L << 6, mouse_move_hook, game);
	mlx_loop_hook(game->helper->mlx, &game_gun, game);
	mlx_hook(game->helper->win, 2, 1, bottoms, game);
	mlx_loop(game->helper->mlx);
}

int	main(int ac, char **av)
{
	t_game	*game;
	int		fd;
	char	*line;

	check_argument(ac, av);
	game = ft_malloc(sizeof(t_game), 1);
	if (!game)
	{
		perror("Error\n<----!Memory allocation failed---->");
		exit(1);
	}
	init_game_struct(game);
	fd = open(av[1], O_RDONLY);
	if (fd == -1)
	{
		perror("Error\n<----!Can't open file---->");
		exit(1);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		parse_line(game, line);
		free(line);
	}
	close(fd);
	return (hookshandeling(game), 0);
}
