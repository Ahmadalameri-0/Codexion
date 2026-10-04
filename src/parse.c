/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abani-am <abani-am@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 11:53:49 by abani-am          #+#    #+#             */
/*   Updated: 2026/09/24 18:15:16 by abani-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Codexion.h"

static int	ft_strcmp(const char *str1, const char *str2)
{
	while (*str1 && *str1 == *str2)
	{
		str1++;
		str2++;
	}
	return (*(unsigned char *)str1 - *(unsigned char *)str2);
}

// Converts a string to a number, returns -1 on error
static long long	ft_atol(const char *str)
{
	long long	res;
	int			i;

	res = 0;
	i = 0;
	if (str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (-1);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	return (res);
}

static int	check_values(t_data *data)
{
	if (data->nb_coders <= 0
		|| data->time_to_burnout <= 0
		|| data->time_to_compile <= 0
		|| data->time_to_debug <= 0
		|| data->time_to_refactor <= 0
		|| data->nb_compiles_required <= 0
		|| data->dongle_cooldown < 0)
	{
		printf("[Error] Invalid numeric value provided.\n");
		return (2);
	}
	return (0);
}

static int	set_scheduler(char *arg, t_data *data)
{
	if (ft_strcmp(arg, "fifo") == 0 || ft_strcmp(arg, "FIFO") == 0)
		data->scheduler = 0;
	else if (ft_strcmp(arg, "edf") == 0 || ft_strcmp(arg, "EDF") == 0)
		data->scheduler = 1;
	else
	{
		printf("[Error] Invalid '%s' scheduler. "
			"It should be 'edf' or 'fifo'\n", arg);
		return (3);
	}
	return (0);
}

int	parse_args(int argc, char **argv, t_data *data)
{
	int		status;

	if (argc != 9)
	{
		printf(
			"[Error] Expected 8 arguments, "
			"but received %d.\n", argc - 1);
		return (1);
	}
	data->nb_coders = ft_atol(argv[1]);
	data->time_to_burnout = ft_atol(argv[2]);
	data->time_to_compile = ft_atol(argv[3]);
	data->time_to_debug = ft_atol(argv[4]);
	data->time_to_refactor = ft_atol(argv[5]);
	data->nb_compiles_required = ft_atol(argv[6]);
	data->dongle_cooldown = ft_atol(argv[7]);
	status = check_values(data);
	if (status != 0)
		return (status);
	return (set_scheduler(argv[8], data));
}
