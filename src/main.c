/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phtruong <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 11:58:24 by phtruong          #+#    #+#             */
/*   Updated: 2019/07/21 18:36:05 by phtruong         ###   ########.fr       */
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
// Merge sort linked list [✓]
// Count sort linked list ??? [X]
// Jump table for printing colors coded file type[]
// Function to append linked list[]
// Store arguments in a linked list before printing[]
// 		Sort arguments


/*
** function is_exe
** Paramters:
** 		struct stat filestat: stat structure in sys/stat.h
** Checks to see if the file is an executable
** Returns: 1 if the file is an executable and not a directory
**			0 otherwise.
*/
int	is_exe(t_stat stat)
{
	return (!S_ISLNK(stat.st_mode) && !S_ISDIR(stat.st_mode) 
			&& (stat.st_mode & S_IXUSR || stat.st_mode & S_IXGRP 
			|| stat.st_mode & S_IXOTH));
}

// Debug functions
/*----------------------------------------------------------------------------*/
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
		case sort_size: return "sort_size";
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

// End of debug functions
/*----------------------------------------------------------------------------*/
/*
** Merge sort attempt
** Creating a merge sort for linked list.
*/
t_files *sorted_merge(t_files *a, t_files *b, int (*cmp)());

void print_list(t_files *f)
{
	while (f)
	{
		ft_printf("%s%s"P_NC, f->color, f->name);
		if (!(indicator_style == none))
			ft_printf("%c", f->style);
		ft_putchar('\n');
		f = f->next;
	}
}
/*
t_files *create(char *name, char *path)
{
	t_files	*head;
	t_stat	stat;

	head = malloc(sizeof(t_files));
	head->name = ft_strdup(name);
	lstat(path, &stat);
	head->path = path;
	head->color = get_color(stat);
	head->fstat = stat;
	head->next = NULL;
	return (head);
}
void	append(t_files **head, char *str, char *path)
{
	t_files *cursor;

	cursor = *head;
	while (cursor->next)
		cursor = cursor->next;
	cursor->next = create(str, path);
}
*/
void col_format(t_files *f, t_pcol *p, unsigned short wcol)
{
	int len;

	len = 0;
	p->max = 0;
	p->no_f = 0;
	while(f)
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

/*
** function path_to_file
** Parameters:
** 		[path]: const char pointer string path from user
**		[file]: file name to be appended to path
** Takes in path and filename. Malloc a new string with 2 extra spaces: one for
** a '/' and another for '\0'. Copies path over then append the slash follow by
** file name.
** Returns: a string of full path with file name appended.
*/
char *path_to_file(const char *path, const char *file)
{
	char	*result;
	size_t	len;
	int		i;

	i = 0;
	len = ft_strlen(path) + ft_strlen(file) + 1;
	if (!(result = malloc(sizeof(char) * (len + 1))))
		return (NULL);
	result[len] = '\0';
	while (*path)
		result[i++] = *path++;
	result[i++] = '/';
	while (*file)
		result[i++] = *file++;
	return (result);
}

/*
** function file_ignored
** Parameters:
** 		[filename] : name of file to check
** Checks if the file should be ignored before adding to list.
** Returns: TRUE if should be ignored. FALSE otherwise
*/
static bool file_ignored(const char *filename)
{
	return ((ignore_mode != MINIMAL && filename[0] == '.'
	&& (ignore_mode == DEFAULT || !filename[1 + (filename[1] == '.')])));
}

void	list_globalize(t_files *f, t_pcol p)
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

void	print_global_list()
{
	t_files *f;
	int i;

	i = 0;
	while (g_sorted_file[i])
	{
		f = g_sorted_file[i];
		ft_printf("%s\n", f->name);
		i++;
	}
}

/*
** function sort_argv
** Parameters:
**		[argc]: total number of arguments
**		[argv]: array of arguments
** Sort the array of arguments using strcmp. Only triggers if user input
** multiple files or directories for arguments. 
** (Bubble sort)
** Space: O(1).
** Time: Avg: O(n^2) Best: O(n).
** Returns: void.
*/
void	sort_argv(int argc, char *argv[])
{
	int		i;
	int		j;
	char	*temp;

	i = g_argc;
	temp = NULL;
	while (i < argc)
	{
		j = i + 1;
		while (j < argc)
		{
			if (!cmp_alpha(argv[i], argv[j]))
			{
				temp = argv[i];
				argv[i] = argv[j];
				argv[j] = temp;
			}
			j++;
		}
		i++;
	}
}

static bool is_regular_file(const char *path)
{
	t_stat stat;

	if (lstat(path, &stat) < 0)
		return (false);
	return (!(S_ISDIR(stat.st_mode)) ? true : false);
}

static bool is_regular_dir(const char *path)
{
	t_stat stat;

	if (lstat(path, &stat) < 0)
		return (false);
	return ((S_ISDIR(stat.st_mode)) ? true : false);
}

t_files *list_argv(int argc, char *argv[])
{
	int		i;
	t_files	*ls;

	i = g_argc;
	ls = NULL;
	while (i < argc)
	{
		if (is_regular_file(argv[i]) || is_regular_dir(argv[i]))
			make_links(&ls, argv[i], argv[i]);
		i++;
	}
	return (ls);
}

/*
** function inner_dir
** Parameters:
**		[name]: directory name
** Opens a directory and store inside contents into a linked list
** Returns: allocated linked list with all the inside contents;
*/
t_files *inner_dir(const char *name)
{
	char			*pa;
	DIR				*dir;
	struct dirent	*dp;
	t_files			*ls;

	ls = NULL;
	pa = NULL;
	if ((dir = opendir(name)) == NULL)
	{
		printf("ft_ls: %s: %s\n", name, strerror(errno));
		return (NULL);
	}
	while ((dp = readdir(dir)) != NULL)
	{
		if (!file_ignored(dp->d_name))
		{
			pa = path_to_file(name, dp->d_name);
			make_links(&ls, dp->d_name, pa);
		}
	}
	closedir(dir);
	return (ls);
}

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

void	free_inner_dir(t_files *f)
{
	t_files *temp;

	while (f)
	{
		if (f->name)
			free(f->name);
		if (f->path)
			free(f->path);
		temp = f->next;
		free(f);
		f = temp;
	}
}

void	ls_driver(t_files *ls);

int 	is_empty_dir(t_files *f)
{
	while (f)
	{
		if (ft_strcmp(f->name, ".") && ft_strcmp(f->name, ".."))
			return (0);
		f = f->next;
	}
	return (1);
}

void	print_dir(t_files *f, int flag)
{
	t_files *in_dir;

//	in_dir = NULL;
	while(f)
	{
		if (S_ISDIR(f->fstat.st_mode) && 
		(ft_strcmp(f->name, ".") && ft_strcmp(f->name, "..")))
		{
			(flag) ? ft_printf("\n%s:\n", f->path) : ft_printf("%s:\n", f->path);
			in_dir = inner_dir(f->path);
			if (in_dir)
			{
				merge_sort_list(&in_dir);
				ls_driver(in_dir);
				if (is_empty_dir(in_dir))
					ft_putchar('\n');
				print_dir(in_dir, 1);
				free_inner_dir(in_dir);
			}
			flag = 1;
		}
		f = f->next;
	}
}

void print_classify(t_pcol p, t_files *f)
{
	if (!(indicator_style == none))
	{
		ft_printf("%s%s"P_NC, f->color, f->name);
		if (!f->style)
			ft_printf("%-*c", p.max - ft_strlen(f->name) + 2, f->style);
		else
			ft_printf("%-*c", p.max - ft_strlen(f->name) + 1, f->style);
	}
	else
		ft_printf("%s%-*s"P_NC, f->color, p.max + 1, f->name);
}

void print_columns(t_pcol p)
{
	t_files	*f;
	int i;
	int j;
	int x;

	i = 0;
	x = 0;
	while (x < p.row)
	{
		(format == many_per_line) && (i = x);
		j = 0;
		while (j < p.col && i < p.no_f)
		{
			f = g_sorted_file[i];
			print_classify(p, f);
			i += (format == many_per_line) ? p.row : 1;
			j++;
		}
		ft_printf("\n");
		x++;
	}
}

void	print_long(t_files *ls);
void	print_with_commas(t_files *ls)
{
	int width;
	struct winsize w;
	
	ioctl(0, TIOCGWINSZ, &w);
	width = 0;
	while (ls)
	{
		ft_printf("%s%s"P_NC, ls->color, ls->name);
		if (indicator_style == classify)
			width += ft_printf("%c", ls->style);
		width += ft_strlen(ls->name);
		if (ls->next)
		{
			width += ft_printf(", ") - ((indicator_style == classify) ? 1 : 0);
			if ((width + ft_strlen(ls->next->name) + 1) >= w.ws_col)
			{
				ft_printf("\n");
				width = 0;
			}
		}
		ls = ls->next;
	}
	ft_putchar('\n');
}
void	ls_driver(t_files *ls)
{
	t_pcol p;
	struct winsize w;

	if (format == long_format)
		print_long(ls);
	else if (format == one_per_line)
		print_list(ls);
	else if (format == with_commas)
		print_with_commas(ls);
	else if (format == many_per_line || format == horizontal)
	{
		ioctl(0, TIOCGWINSZ, &w);
		col_format(ls, &p, w.ws_col);
		list_globalize(ls, p);
		print_columns(p);
		free(g_sorted_file);
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

void	open_print(char *name)
{
	DIR *dir;
	struct dirent *dp;
	char *path;
	t_files *ls;

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

void	print_directories(t_files *ls)
{
	while (ls)
	{
		ft_printf("%s:\n", ls->path);
		open_print(ls->name);
		ls = ls->next;
		if (!ls)
			break;
		ft_putchar('\n');
	}
}

int		max(int a, int b)
{
	return ((a > b) ? a : b);
}

int		count_nbr(int n)
{
	int c;

	c = 1;
	if (!n)
		return (1);
	while (n /= 10)
		c++;
	return (c);
}

int  get_usr_w(int w, uid_t u)
{
	if (g_numeric_id)
		w = max(count_nbr(u), w);
	else
		w = max(ft_strlen(getpwuid(u)->pw_name), w);
	return (w);
}

int get_grp_w(int w, gid_t g)
{
	if (g_numeric_id)
		w = max(count_nbr(g), w);
	else
		w = max(ft_strlen(getgrgid(g)->gr_name), w);
	return (w);
}

void	index_details(t_files *ls, t_idx *i)
{
	int		total;
	t_files *p_ls;

	total = 0;
	p_ls = ls;
	while (ls)
	{
		total += ls->fstat.st_blocks;
		i->ino_w = max(count_nbr(ls->fstat.st_ino), i->ino_w);
		i->lin_w = max(count_nbr(ls->fstat.st_nlink), i->lin_w);
		i->usr_w = get_usr_w(i->usr_w, ls->fstat.st_uid);
		i->grp_w = get_grp_w(i->grp_w, ls->fstat.st_gid);
		i->siz_w = max(count_nbr(ls->fstat.st_size), i->siz_w);
		if (S_ISCHR(ls->fstat.st_mode) || S_ISBLK(ls->fstat.st_mode))
		{
			i->maj_w = max(count_nbr(major(ls->fstat.st_rdev)), i->maj_w);
			i->min_w = max(count_nbr(minor(ls->fstat.st_rdev)), i->min_w);
			i->siz_w = max(i->maj_w + i->min_w + 2, i->siz_w);
		}
		ls = ls->next;
	}
	if (p_ls && g_print_total)
		ft_printf("total %d\n", total);
	/*
	 * total of blocks are only printed if it's a folder
	 */
}

void	ft_ls_mode(int mode, char *path)
{
	char buf[12];

	buf[0] = get_file_type(mode);
	buf[1] = (mode & S_IRUSR) ? 'r' : '-';
	buf[2] = (mode & S_IWUSR) ? 'w' : '-';
	buf[3] = (mode & S_IXUSR) ? 'x' : '-';
	buf[4] = (mode & S_IRGRP) ? 'r' : '-';
	buf[5] = (mode & S_IWGRP) ? 'w' : '-';
	buf[6] = (mode & S_IXGRP) ? 'x' : '-';
	buf[7] = (mode & S_IROTH) ? 'r' : '-';
	buf[8] = (mode & S_IWOTH) ? 'w' : '-';
	buf[9] = (mode & S_IXOTH) ? 'x' : '-';
	if (mode & S_ISUID)
		buf[3] = (buf[3] == '-') ? 'S' : 's';
	if (mode & S_ISGID)
		buf[6] = (buf[6] == '-') ? 'S' : 's';
	if (mode & S_ISVTX)
		buf[9] = (buf[9] == '-') ? 'T' : 't';
	buf[10] = get_acl_exe(path);
	buf[11] = '\0';
	ft_printf("%s ", buf);
}

void	print_usr_grp(t_files *ls, t_idx i)
{
	if (g_numeric_id)
	{
		ft_printf("%-*d  ", i.usr_w, ls->fstat.st_uid);
		ft_printf("%-*d  ", i.grp_w, ls->fstat.st_gid);
	}
	else
	{
		ft_printf("%-*s  ", i.usr_w, getpwuid(ls->fstat.st_uid)->pw_name);
		ft_printf("%-*s  ", i.grp_w, getgrgid(ls->fstat.st_gid)->gr_name);
	}
}

void	print_date_time(char *s)
{
	char buf[14];
	int i;
	int j;

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
	char buf[13];
	int i;
	int j;

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
	time_t t;
	struct tm l_time;

	time(&t);
	localtime_r(&t, &l_time);
	if ((l_time.tm_year - f_time.tm_year) > 0 || 
	(l_time.tm_year - f_time.tm_year) < 0 ||
	(l_time.tm_mon - f_time.tm_mon) > 6 || (l_time.tm_mon - f_time.tm_mon < 0))
		return (0);
	return (1);
}

void	print_time(t_files *ls)
{
	time_t t;
	struct tm t_info;
	char buf[26];

	t = ls->fstat.st_mtime;
	localtime_r(&t, &t_info);
	if (!check_timestamp(t_info))
		print_year(ctime_r(&t, buf));
	else
		print_date_time(ctime_r(&t, buf));
}

void	print_long_name(t_files *ls)
{
	char link[NAME_MAX + 1];

	if (S_ISLNK(ls->fstat.st_mode))
	{
		ft_bzero(link, NAME_MAX + 1);
		readlink(ls->path, link, NAME_MAX);
		ft_printf("%s", ls->color);
		ft_printf("%s "P_NC, ls->name);
		ft_printf("-> %s\n", link);
	}
	else
		ft_printf("%s%s\n"P_NC, ls->color, ls->name);
}

void	print_long(t_files *ls)
{
	t_idx i;
	ft_bzero(&i, sizeof(t_idx));
	index_details(ls, &i);
	while (ls)
	{
		if (g_print_inode)
			ft_printf("%-*llu ", i.ino_w, ls->fstat.st_ino);
		ft_ls_mode(ls->fstat.st_mode, ls->path);
		ft_printf("%-*u ", i.lin_w, ls->fstat.st_nlink);
		print_usr_grp(ls, i);
		ft_printf("%*d ", i.siz_w, ls->fstat.st_size);
		print_time(ls);
		print_long_name(ls);
		ls = ls->next;
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

	open_print(str);
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

int main(int argc, char *argv[])
{
	ft_ls_init();
	if (argc > 1)
		flag_driver(argc, argv);
	//print_ls_settings();
	//printf("g_argc:%d argc: %d\n", g_argc, argc);
	if (g_argc && (g_argc < (argc - 1)))
		multi_argv(argc, argv);
	else if (g_argc && ((argc - g_argc) == 1))
		single_argv(argv[g_argc]);
	else if (!g_argc)
		single_argv(".");
	return (0);
}
