/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abani-am <abani-am@learner.42.tech>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 09:12:52 by abani-am          #+#    #+#             */
/*   Updated: 2026/10/03 09:12:53 by abani-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Codexion.h"

static void	clean_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_coders)
	{
		pthread_mutex_destroy(&data->dongle[i].lock);
		pthread_cond_destroy(&data->dongle[i].cond);
		free(data->dongle[i].queue.items);
		i++;
	}
	pthread_mutex_destroy(&data->write_mutex);
	pthread_mutex_destroy(&data->state_mutex);
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
	{
		pthread_join(data.coders[i].thread, NULL);	
	}
	clean_simulation(&data);
	return (0);
}