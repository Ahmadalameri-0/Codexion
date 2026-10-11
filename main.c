/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abani-am <abani-am@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 09:12:52 by abani-am          #+#    #+#             */
/*   Updated: 2026/10/10 12:35:33 by abani-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abani-am <abani-am@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 18:02:36 by abani-am          #+#    #+#             */
/*   Updated: 2026/10/04 12:49:13 by abani-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Codexion.h"

static void	clean_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_coders)
	{
		free(data->dongle[i].queue.items);
		i++;
	}
	pthread_mutex_destroy(&data->write_mutex);
	pthread_mutex_destroy(&data->state_mutex);
	pthread_mutex_destroy(&data->table_lock);
	pthread_cond_destroy(&data->table_cond);
	free(data->dongle);
	free(data->coders);
}

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	if (parse_args(argc, argv, &data) != 0)
		return (1);
	if (init_simulation(&data) != 0)
		return (1);
	i = -1;
	while (++i < data.nb_coders)
	{
		if (pthread_create(&data.coders[i].thread, NULL,
				coder_routine, &data.coders[i]) != 0)
			return (1);
	}
	monitor_routine(&data);
	i = -1;
	while (++i < data.nb_coders)
		pthread_join(data.coders[i].thread, NULL);
	clean_simulation(&data);
	return (0);
}
