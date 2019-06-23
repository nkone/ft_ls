/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phtruong <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 11:58:24 by phtruong          #+#    #+#             */
/*   Updated: 2019/06/22 19:47:34 by phtruong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
// collect flags from multiple arguments [✓]
// if first argument is not flag then the rest are files or dir names
// flags can override one another ex. -a -la -a
// find starting arguments with file names [✓]
// Loop through each argument to see if not flags then returns that argc
// If all arguments are flags then current folder is main selection.
// Need to recreate getopt function from getopt [X] <-Fuck this
// Use enum for super global variables [✓]
// LS printing guide [ ]
// Sort first before printing (use stat and fstat)
// No such file or directory is printed first;
// Single files are next (alphabetical order);
// Folders are next;
// Permission denied last in the folder or by itself;
// Print file stats [ ]
// File permissions are stored as a number and uses bitwise to find exact
// permission types;
//
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

void	ft_ls_initialize(void)
{
	format = many_per_line;
	sort_type = sort_name;
	indicator_style = none;
	g_sort_reverse = false;
	g_recursive = false;
	g_print_inode = false;
	g_numeric_id = false;
	ignore_mode = DEFAULT;
}

void	check_legal_opt(char c)
{
	if (!(ft_strchr(LS_FLAGS, c)))
	{
		ft_printf("ls: illegal option -- %c\nusage: ft_ls [-%s]"
					" [file ...]\n", c, LS_FLAGS);
		exit(1);
	}
}

void	case_F(void)
{
	indicator_style = classify;
}

void	case_R(void)
{
	g_recursive = true;
}

void	case_a(void)
{
	ignore_mode = MINIMAL;
}

void	case_f(void)
{
	ignore_mode = MINIMAL;
	sort_type = sort_none;
}

void	case_i(void)
{
	g_print_inode = true;
}

void	case_l(void)
{
	format = long_format;
}

void	case_m(void)
{
	format = with_commas;
}

void	case_n(void)
{
	g_numeric_id = true;
	format = long_format;
}

void	case_p(void)
{
	indicator_style = slash;
}

void	case_r(void)
{
	g_sort_reverse = true;
}

void	case_t(void)
{
	sort_type = sort_time;
}

void	case_x(void)
{
	format = horizontal;
}

void	case_1(void)
{
	format = one_per_line;
}

void	flag_collector(char *argv[], int c)
{
	int i;
	char *ref;

	i = 0;
	if (argv[c][i] == '-')
		i++;
	else
		return ;
	while (argv[c][i])
	{
		check_legal_opt(argv[c][i]);
		ref = ft_strchr(LS_FLAGS, argv[c][i]);
		g_ls_flags[(ptrdiff_t)(ref - LS_FLAGS)]();
		i++;
	}
	return ;
}

void	flag_driver(int argc, char *argv[])
{
	int c;

	c = 0;
	// Loop through arguments
	while (++c <= (argc - 1))
		flag_collector(argv, c);
}

char	*get_format(enum e_format format)
{
	switch(format)
	{
		case long_format: return "long_format";
		case one_per_line: return "one_per_line";
		case many_per_line: return "many_per_line";
		case horizontal: return "horizontal";
		case with_commas: return "with_commas";
	}
}

char	*get_sort_type(enum e_sort_type sort_type)
{
	switch(sort_type)
	{
		case sort_none: return "sort_none";
		case sort_name: return "sort_name";
		case sort_time: return "sort_time";
	}
}

char	*get_indicator(enum e_indicator_style indicator)
{
	switch(indicator)
	{
		case none: return "none";
		case slash: return "slash";
		case classify: return "classify";
	}
}

char	*get_ignore_mode(enum e_ignore_mode mode)
{
	switch(mode)
	{
		case DEFAULT: return "DEFAULT";
		case DOT_AND_DOTDOT: return "DOT_AND_DOTDOT";
		case MINIMAL: return "MINIMAL";
	}
}

void	print_ls_settings(void)
{
	printf(
			"Format: %s\n"
			"Sort type: %s\n"
			"Indicator: %s\n"
			"Ignore mode: %s\n",
			get_format(format),
			get_sort_type(sort_type),
			get_indicator(indicator_style),
			get_ignore_mode(ignore_mode));
	printf("Reverse sort: ");
	printf( (g_sort_reverse) ? "True\n" : "False\n");
	printf("Recursive: ");
	printf( (g_recursive) ? "True\n" : "False\n");
	printf("Print inode: ");
	printf( (g_print_inode) ? "True\n" : "False\n");
	printf("Print numeric id: ");
	printf( (g_numeric_id) ? "True\n" : "False\n");
	
}
int main(int argc, char *argv[])
{
	char	*dirname;
	DIR		*dir;
	struct stat filestat;
	ft_ls_initialize();
	if (argc > 1)
		flag_driver(argc, argv);
	/*if (stat(argv[1], &filestat) < 0)
		return 1;
	printf("File Permissions: \t\n");
    ft_printf( (S_ISDIR(filestat.st_mode)) ? "d" : "-");
    printf( (filestat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (filestat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (filestat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (filestat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (filestat.st_mode & S_IWGRP) ? "w" : "-");	
    printf( (filestat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (filestat.st_mode & S_IROTH) ? "r" : "-");
    printf( (filestat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (filestat.st_mode & S_IXOTH) ? "x" : "-");
	printf("\nFile or Directory: \t");
	printf( (S_ISDIR(filestat.st_mode)) ? "Dir" : "File");*/
//	ft_printf("start: %d argc: %d dirname: %s\n", start, argc, dirname);
//	while(1);
	//format = horizontal;
	print_ls_settings();
	return (0);
}
