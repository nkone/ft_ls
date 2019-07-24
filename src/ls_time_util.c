/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_time_util.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phtruong <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 19:48:26 by phtruong          #+#    #+#             */
/*   Updated: 2019/07/23 19:49:16 by phtruong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_date_time(char *s)
{
	char	buf[14];
	int		i;
	int		j;

	i = 4;
	j = 0;
	ft_bzero(buf, sizeof(buf));
	while (i < 16)
		buf[j++] = s[i++];
	buf[j] = '\0';
	ft_printf("%s ", buf);
}

void	print_year(char *s)
{
	char	buf[13];
	int		i;
	int		j;

	i = 4;
	j = 0;
	while (i < 11)
		buf[j++] = s[i++];
	i = 20;
	buf[j++] = ' ';
	while (i < 24)
		buf[j++] = s[i++];
	buf[j] = '\0';
	ft_printf("%s ", buf);
}

int		check_timestamp(struct tm f_time)
{
	time_t		t;
	struct tm	l_time;

	time(&t);
	localtime_r(&t, &l_time);
	if ((l_time.tm_year - f_time.tm_year) > 0 ||
	(l_time.tm_year - f_time.tm_year) < 0 ||
	(l_time.tm_mon - f_time.tm_mon) > 6 || (l_time.tm_mon - f_time.tm_mon < 0))
		return (0);
	return (1);
}
