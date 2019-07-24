/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_col_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phtruong <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 19:56:10 by phtruong          #+#    #+#             */
/*   Updated: 2019/07/24 13:37:22 by phtruong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	col_format(t_files *f, t_pcol *p, unsigned short wcol)
{
	int len;

	len = 0;
	p->max = 0;
	p->no_f = 0;
	while (f)
	{
		len = (int)ft_strlen(f->name);
		len += (int)ft_strlen(&f->style);
		p->max = (len > p->max) ? len : p->max;
		p->no_f += 1;
		f = f->next;
	}
	p->col = (wcol >= (p->max + 1)) ? (wcol / (p->max + 1)) : 1;
	p->row = (p->no_f % p->col) ? (p->no_f / p->col + 1) : (p->no_f / p->col);
}

void	ls_list_globalize(t_files *f, t_pcol p)
{
	int i;

	i = 0;
	g_sorted_file = malloc(sizeof(*g_sorted_file) * (p.no_f + 1));
	while (f)
	{
		g_sorted_file[i++] = f;
		f = f->next;
	}
	g_sorted_file[i] = NULL;
}
