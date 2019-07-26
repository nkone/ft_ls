/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_util.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phtruong <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/20 16:09:55 by phtruong          #+#    #+#             */
/*   Updated: 2019/07/25 18:08:22 by phtruong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** function cmp_alpha()
** Parameters:
**		[s1]: string 1
**		[s2]: string 2
** Uses strcmp to sort ascending or descending base on global sort reverse bool
** Returns:
**		1 if true, 0 otherwise
*/

int	cmp_alpha(const char *s1, const char *s2)
{
	if (!g_sort_reverse)
		return (ft_strcmp(s1, s2) < 0);
	return (ft_strcmp(s1, s2) > 0);
}

int	casecmp_alpha(const char *s1, const char *s2)
{
	return (ft_strcasecmp(s1, s2) < 0);
}

/*
** function cmp_time()
** Paramters:
**		[a]: time since epoch a
**		[b]: time since Epoch b
** Checks time between a and b depending on global sort reverse bool
** Returns:
**		1 if true, 0 otherwise
*/

int	cmp_time(time_t a, time_t b)
{
	if (!g_sort_reverse)
		return (a >= b);
	return (a <= b);
}

/*
** function cmp_size()
** Paramters:
**		[a]: file size a;
**		[b]: file size b;
** Checks size between a and b depending on global sort reverse bool
** Returns:
**		1 if true, 0 otherwise
*/

int	cmp_size(off_t a, off_t b)
{
	if (!g_sort_reverse)
		return (a >= b);
	return (a <= b);
}

/*
** function select_sort()
** Paramaters:
**			[a]: pointer to struct t_files
**			[b]: pointer to struct t_files
** Select which type of sort base on global sort type
** Returns:
**		Return value of appropriate compare function, 0 if sort_type == none
*/

int	select_sort(t_files *a, t_files *b)
{
	if (sort_type == sort_name)
		return (cmp_alpha(a->name, b->name));
	else if (sort_type == sort_time)
		return (cmp_time(a->fstat.st_mtime, b->fstat.st_mtime));
	else if (sort_type == sort_size)
		return (cmp_size(a->fstat.st_size, b->fstat.st_size));
	else if (sort_type == sort_none)
		return (casecmp_alpha(a->name, b->name));
	return (1);
}
