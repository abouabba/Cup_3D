/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 15:34:40 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/18 15:37:01 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int	in_valid_ber_file(char *file_name)
{
	int	len;

	len = ft_strlen(file_name);
	if (!file_name || len < 4)
		return (0);
	return (file_name[len - 4] == '.'
		&& file_name[len - 3] == 'c'
		&& file_name[len - 2] == 'u'
		&& file_name[len - 1] == 'b');
}

void	check_argument(int ac, char **av)
{
	if (ac != 2)
	{
		write(2, "Error\nInvalid number of arguments\n", 34);
		exit(1);
	}
	if (!in_valid_ber_file(av[1]))
	{
		write(2, "Error\nInvalid number of arguments\n", 34);
		exit(1);
	}
}
