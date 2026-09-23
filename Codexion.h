/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abani-am <abani-am@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 11:24:06 by abani-am          #+#    #+#             */
/*   Updated: 2026/09/22 16:31:52 by abani-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stddef.h>

typedef struct s_dongle
{
	int					id;
	pthread_mutex_t		lock;
	long long			last_used_time;
}						t_dongle;

typedef struct s_coder
{
	int				id;
	long long		last_compile_time;
	int				compiles_count;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	struct s_data	*data;
}					t_coder;

typedef struct s_data
{
	int					nb_coders;
	long long			time_to_burnout;
	long long			time_to_compile;
	long long			time_to_debug;
	long long			time_to_refactor;
	int					nb_compiles_required;
	long long			dongle_cooldown;
	int					scheduler;
	long long			start_time;
	int					simulation_stop;
	pthread_mutex_t		write_mutex;
	pthread_mutex_t		state_mutex;
	t_coder				*coders;
	t_dongle			*dongle;
}						t_data;

long long	get_current_time(void);
long long	get_time_elapsed(long long start_time);
void		ft_usleep(long long time_in_ms, t_data *data);
int			parse_args(int argc, char **argv, t_data *data);
int			init_simulation(t_data *data);

#endif
