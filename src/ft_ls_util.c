/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phtruong <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 20:15:05 by phtruong          #+#    #+#             */
/*   Updated: 2019/07/24 13:35:30 by phtruong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	open_print(char *name)
{
	DIR				*dir;
	struct dirent	*dp;
	char			*path;
	t_files			*ls;

	ls = NULL;
	if ((dir = opendir(name)) == NULL)
		ft_printf("ft_ls: %s: %s\n", name, strerror(errno));
	else
	{
		while ((dp = readdir(dir)) != NULL)
		{
			if (!file_ignored(dp->d_name))
			{
				path = path_to_file(name, dp->d_name);
				make_links(&ls, dp->d_name, path);
			}
		}
		closedir(dir);
	}
	merge_sort_list(&ls);
	ls_driver(ls);
	free_inner_dir(ls);
}

void	print_directories(t_files *f)
{
	while (f)
	{
		ft_printf("%s:\n", f->path);
		open_print(f->name);
		f = f->next;
		if (!f)
			break ;
		ft_putchar('\n');
	}
}
