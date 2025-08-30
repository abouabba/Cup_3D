#include "cub.h"

int	in_valid_ber_file(char *file_name)
{
	int	len = ft_strlen(file_name);

	if (!file_name || len < 4)
		return (0);
	return (file_name[len - 4] == '.' &&
			file_name[len - 3] == 'c' &&
			file_name[len - 2] == 'u' &&
			file_name[len - 1] == 'b');
}

void	check_argument(int ac, char **av)
{
	if (ac != 2)
	{
		printf("Error\nInvalid number of arguments\n");
		exit(1);
	}
	if (!in_valid_ber_file(av[1]))
	{
		printf("Error\n<----!Invalid file extension---->\n");
		exit(1);
	}
}
