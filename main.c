/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phtruong <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 11:58:24 by phtruong          #+#    #+#             */
/*   Updated: 2019/06/20 19:27:41 by phtruong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
// collect flags from multiple arguments [✓]
// if first argument is not flag then the rest are files or dir names
// flags can override one another ex. -a -la -a
// find starting arguments with file names [✓]
// Loop through each argument to see if not flags then returns that argc
// If all arguments are flags then current folder is main selection.
// LS printing guide [ ]
// Sort first before printing (use stat and fstat)
// No such file or directory is printed first;
// Single files are next (alphabetical order);
// Folders are next;
// Permission denied last in the folder or by itself;
int find_start_arg(int argc, char *argv[])
{
	int c;
	int i;

	i = 0;
	c = 0;
	while (++c <= (argc - 1))
	{
//		ft_printf("checking argv: %s\n", argv[c]);
		if (argv[c][i] == '-' && argv[c][i+1])
			continue;
		else
			return (c);
	}
//	printf("c: %d\n", c);
	return (0);
}

int	collect_ls_flags(char *argv[], int c)
{
	char	*ref;
	int		flag;
	int		i;

	flag = 0;
	i = 0;
	if (argv[c][i] == '-')
		i++;
	else
		return (flag);
	while (argv[c][i])
	{
		if (!(ref = ft_strchr(LS_FLAGS, argv[c][i])))
		{
			ft_printf("ls: illegal option -- %c\nusage: ft_ls [-%s]"
						" [file ...]\n", argv[c][i], LS_FLAGS);
			exit(0);
		}
		if (!argv[c][i])
			return (flag);
		flag |= g_ls_flags[(ptrdiff_t)(ref - LS_FLAGS)];
		i++;
	}
	return (flag);
}

int ls_flags(int argc, char *argv[])
{
	int		flag;
	int		c;

	c = 0;
	flag = 0;
	while (++c <= (argc - 1)) {
		flag |= collect_ls_flags(argv, c);
		// stops collecting flag if there's no '-' or just '-'
		if (!flag || !collect_ls_flags(argv, c))
			return (flag);
	}
	return (flag);
}

int main(int argc, char *argv[])
{
	int		flag;
	int		start;
	char	*dirname;
	DIR		*dir;
	flag = 0;
	if (argc > 1)
		flag |= ls_flags(argc, argv);
//	ft_printf("start: %d argc: %d dirname: %s\n", start, argc, dirname);
//	while(1);
	return (0);
}
