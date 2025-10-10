 #include "cub.h"

void	is_valid_texture(t_game *game)
{
	if (game->no != 1 || game->so != 1 || game->we != 1 || game->ea != 1 || game->f != 1 || game->c != 1)
	{
		printf("[%d], [%d], [%d], [%d], [%d], [%d], \n", game->no, game->so, game->we, game->ea, game->f, game->c);
		print_error("Invalid texture");
	}
}

t_txtu	init_txtu(t_game *game, char *file)
{
	t_txtu	txtu;

	if (access(file, F_OK))
	{
		printf(" wrong: %s\n", file);
		exit(1);

	}
	txtu.txture_p = mlx_xpm_file_to_image(game->helper->addr, file, &txtu.width , &txtu.height);
	if  (!txtu.txture_p )
	{
		printf("texttttuer error \n");
		exit(1);
	}
	txtu.data_add =  mlx_get_data_addr(txtu.txture_p, &txtu.bbp, &txtu.line_len, &txtu.endian);
	return txtu;
}


int	main(int ac, char **av)
{
	t_game	*game;
	int		fd;
	char	*line;

	check_argument(ac, av);

	game = malloc(sizeof(t_game));
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

	// Next steps would be:
	is_valid_texture(game);
	finalize_map(game);
	// print_map(game->map);
	validate_map(game);
	// and parse the map in file map.cub in function validate_map

	// rayn's part's 3d_view
	init_game(game);
	int i = 0;
	game->txt = ft_split("east.xpm  north.xpm south.xpm west.xpm", ' ');
	
	while (i < 4)
	{
		char *uuu= ft_strjoin(ft_strdup("textures/"), game->txt[i]);
		printf("%s\n", uuu);
		game->txtu[i] = init_txtu(game, uuu);
		i++;
	}
	mlx_hook(game->helper->win, 17, 0, close_window, game);
	mlx_loop_hook(game->helper->mlx,&render_map, game);
	
	mlx_hook(game->helper->win, 2, 1, bottoms, game);
	mlx_hook(game->helper->win, 6, 1L << 6, mouse_move_hook, game);
	load_gun_sprite(game);
	mlx_loop_hook(game->helper->mlx, game_gun, game);

	mlx_loop(game->helper->mlx);
	return (0);
}
