/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phtruong <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 20:07:03 by phtruong          #+#    #+#             */
/*   Updated: 2019/07/24 13:35:48 by phtruong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	free_list_argv(t_files *f)
{
	t_files *temp;

	while (f)
	{
		if (f->name)
			free(f->name);
		temp = f->next;
		free(f);
		f = temp;
	}
}

void	parse_multi_argv(int argc, char *argv[], t_files **files, t_files **dir)
{
	int		i;
	t_stat	stat;

	i = g_argc;
	while (i < argc)
	{
		if (lstat(argv[i], &stat) < 0)
			ft_printf("ft_ls: %s: %s\n", argv[i], strerror(errno));
		else
		{
			if (is_regular_file(argv[i]))
				make_links(files, argv[i], argv[i]);
			else
				make_links(dir, argv[i], argv[i]);
		}
		i++;
	}
}

void	multi_argv(int argc, char *argv[])
{
	t_files *files;
	t_files *dir;

	files = NULL;
	dir = NULL;
	parse_multi_argv(argc, argv, &files, &dir);
	merge_sort_list(&files);
	merge_sort_list(&dir);
	(files) && (g_print_total = false);
	ls_driver(files);
	g_print_total = true;
	(files && dir) && ft_putchar('\n');
	if (!g_recursive)
		print_directories(dir);
	if (g_recursive)
		print_dir(dir, 0);
	free_list_argv(files);
	free_list_argv(dir);
}

void	single_argv(char *str)
{
	t_files *ls;

	ls = NULL;
	make_links(&ls, str, str);
	if (is_regular_file(ls->name))
		ft_printf("%s%s"P_NC"%c\n", ls->color, ls->name, ls->style);
	else
		open_print(str);
	free_list_argv(ls);
	if (g_recursive)
	{
		ls = inner_dir(str);
		if (ls)
		{
			merge_sort_list(&ls);
			ft_putchar('\n');
			print_dir(ls, 0);
			free_inner_dir(ls);
		}
	}
}

int		main(int argc, char *argv[])
{
	ft_ls_init();
	if (argc > 1)
		flag_driver(argc, argv);
	if (g_argc && (g_argc < (argc - 1)))
		multi_argv(argc, argv);
	else if (g_argc && ((argc - g_argc) == 1))
		single_argv(argv[g_argc]);
	else if (!g_argc)
		single_argv(".");
	return (0);
}
