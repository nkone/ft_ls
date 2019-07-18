/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phtruong <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 11:58:24 by phtruong          #+#    #+#             */
/*   Updated: 2019/07/17 20:53:31 by phtruong         ###   ########.fr       */
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
** function ft_ls_init:
** Parameters: NONE
** Creates the initial settings for ft_ls.
** Settings here are default unless changed by collected flags.
** Returns: void.
*/
void	ft_ls_init(void)
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

void	case_ua(void)
{
	ignore_mode = DOT_AND_DOTDOT;
}

void	case_uf(void)
{
	indicator_style = classify;
}

void	case_ur(void)
{
	g_recursive = true;
}

void	case_us(void)
{
	sort_type = sort_size;
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

/*
** function get_flags
** Parameters:
** 		char *argv[]: char pointer to stream of argument values.
**		int c: index to access a specific argument value.
** Parses through an argument, with given index (c), checks for illegal flags,
** alters settings for legal flags.
** Returns:	value of c if argument does not start with '-' follow by a character
** 			0 if success.
*/
int		get_flags(char *argv[], int c)
{
	int i;
	char *ref;

	i = 0;
	if (argv[c][i] == '-' && argv[c][i+1] != '\0')
		i++;
	else if (argv[c][i])
		return (c);
	while (argv[c][i])
	{
		if (!(ref = ft_strchr(LS_FLAGS, argv[c][i])))
		{
			ft_printf("ls: illegal option -- %c\nusage: ft_ls [-%s]"
					" [file ...]\n", argv[c][i], LS_FLAGS);
			exit(1);
		}
		g_ls_flags[(ptrdiff_t)(ref - LS_FLAGS)]();
		i++;
	}
	return (0);
}

/*
** function flag_driver
** Parameters:
**		int argc: total number of arguments in main
**		char *argv[]: char pointer to argument values
** Loop through all the arguments and update global starting argument.
** If argument consists of flags only, then starting argument will be zero
** as the get_flags() returns 0.
** Stop collecting flags when encounters a non flag argument.
** Returns: void.
*/
void	flag_driver(int argc, char *argv[])
{
	int c;

	c = 0;
	// Loop through arguments
	while (++c <= (argc - 1))
	{
		g_argc = get_flags(argv, c);
		if (g_argc != 0)
			break;
	}
}

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
t_files *sorted_merge(t_files *a, t_files *b);

void print_list(t_files *ls)
{
	while (ls)
	{
		if (is_exe(ls->fstat))
			printf(P_RED"%s\n"P_NC, ls->name);
		else if (S_ISDIR(ls->fstat.st_mode))
			printf(P_CYAN"%s\n"P_NC, ls->name);
		else if (S_ISLNK(ls->fstat.st_mode))
			printf(P_MAGENTA"%s\n"P_NC, ls->name);
		else
		{
			printf("%s\n", ls->name);
		}
		//printf("path: %s\n", ls->path);
		ls = ls->next;
	}
}

char	*get_color(t_stat stat)
{
	if (is_exe(stat))
		return (P_RED);
	else if (S_ISDIR(stat.st_mode))
		return (P_CYAN);
	else if (S_ISLNK(stat.st_mode))
		return(P_MAGENTA);
	else
		return ("");
}

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

void make_node(t_files **ls, char *name, char *path)
{
	t_stat stat;

	*ls = malloc(sizeof(t_files));
	(*ls)->name = ft_strdup(name);
	lstat(path, &stat);
	(*ls)->path = path;
	(*ls)->color = get_color(stat);
	(*ls)->fstat = stat;
	(*ls)->next = NULL;
}
void make_links(t_files **ls, char *name, char *path)
{
	t_files *cursor;
	t_stat	stat;

	if (!*ls)
		make_node(ls, name, path);
	else if (*ls)
	{
		cursor = *ls;
		while (cursor->next)
			cursor = cursor->next;
		make_node(&(cursor->next), name, path);
	}
}
void	split_list(t_files *source, t_files **front, t_files **back)
{
	t_files *fast;
	t_files *slow;

	slow = source;
	fast = source->next;
	while (fast)
	{
		fast = fast->next;
		if (fast)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	*front = source;
	*back = slow->next;
	slow->next = NULL;
}

int	cmp_alpha(const char *s1, const char *s2)
{
	if (!g_sort_reverse)
		return (ft_strcmp(s1, s2) < 0);
	return(ft_strcmp(s1, s2) > 0);
}

int cmp_time(time_t a, time_t b)
{
	if (!g_sort_reverse)
		return (a > b);
	return(a < b);
}

int cmp_size(off_t a, off_t b)
{
	if (!g_sort_reverse)
		return (a > b);
	return (a < b);
}

int		select_sort(t_files *a, t_files *b)
{
	if (sort_type == sort_name)
		return (cmp_alpha(a->name, b->name));
	else if(sort_type == sort_time)
		return (cmp_time(a->fstat.st_mtime, b->fstat.st_mtime));
	else if (sort_type == sort_size)
		return (cmp_size(a->fstat.st_size, b->fstat.st_size));
	return (0);
}

t_files	*sorted_merge(t_files *a, t_files *b)
{
	t_files *result;

	result = NULL;
	if (!a)
		return (b);
	else if (!b)
		return (a); 
	if (select_sort(a, b))
	{
		result = a;
		result->next = sorted_merge(a->next, b);
	}
	else
	{
		result = b;
		result->next = sorted_merge(a, b->next);
	}
	return (result);
}

void merge_sort_list(t_files **ls)
{
	t_files *head;
	t_files *a;
	t_files *b;

	head = *ls;
	if (head == NULL || head->next == NULL)
		return;
	split_list(head, &a, &b);
	merge_sort_list(&a);
	merge_sort_list(&b);
	*ls = sorted_merge(a, b);
}

void col_format(t_files *f, t_pcol *p, unsigned short wcol)
{
	int len;

	len = 0;
	p->max = 0;
	p->no_f = 0;
	while(f)
	{
		len = (int)ft_strlen(f->name);
		p->max = (len > p->max) ? len : p->max;
		p->no_f += 1;
		f = f->next;
	}
	p->col = (wcol >= (p->max + 1)) ? (wcol / (p->max + 1)) : 1;
	//printf("p->col: %d wcol: %hd p->max: %d\n", p->col, wcol, p->max);
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
		printf("%s\n", f->name);
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
		{
			if (!ls)
				ls = create(argv[i], argv[i]);
			else
				append(&ls, argv[i], argv[i]);
		}
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
	t_stat			stat;

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
			if (!ls)
				ls = create(dp->d_name, pa);
			else
				append(&ls, dp->d_name, pa);
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

void	print_dir(t_files *f, int flag)
{
	t_files *in_dir;

	in_dir = NULL;
	while(f)
	{
		if (S_ISDIR(f->fstat.st_mode))
		{
			if (flag)
				printf("\n%s:\n", f->path);
			else
				printf("%s:\n", f->path);
			in_dir = inner_dir(f->path);
			if (in_dir)
			{
				merge_sort_list(&in_dir);
				ls_driver(in_dir);
				print_dir(in_dir, 1);
				free_inner_dir(in_dir);
			}
		}
		f = f->next;
	}
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
			printf("%s", f->color);
			printf("%-*s"P_NC, p.max + 1, f->name);
			i += (format == many_per_line) ? p.row : 1;
			j++;
		}
		printf("\n");
		x++;
	}
}

void	print_long(t_files *ls);
void	ls_driver(t_files *ls)
{
	t_pcol p;
	struct winsize w;

	ioctl(0, TIOCGWINSZ, &w);
	col_format(ls, &p, w.ws_col);
	list_globalize(ls, p);
//	print_global_list();
//	printf("col: %d row: %d max: %d no_f: %d\n", p.col, p.row, p.max, p.no_f);

	if (format == long_format)
		print_long(ls);
	else if (format == many_per_line)
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
			printf("ft_ls: %s: %s\n", argv[i], strerror(errno));
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
	t_stat stat;
	DIR *dir;
	struct dirent *dp;
	char *path;
	t_files *ls;

	ls = NULL;
	if ((dir = opendir(name)) == NULL)
		printf("ft_ls: %s: %s\n", name, strerror(errno));
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
	ls_driver(ls);
	free_inner_dir(ls);
}

void	print_directories(t_files *ls)
{
	while (ls)
	{
		printf("%s:\n", ls->path);
		open_print(ls->name);
		ls = ls->next;
		if (!ls)
			break;
		printf("\n");
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

	total = 0;
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
	printf("total %d\n", total);
	/*
	 * total of blocks are only printed if it's a folder
	 */
}

char	get_file_type(int mode)
{
	if (S_ISDIR(mode))
		return ('d');
	else if (S_ISCHR(mode))
		return ('c');
	else if (S_ISBLK(mode))
		return ('b');
	else if (S_ISFIFO(mode))
		return ('p');
	else if (S_ISLNK(mode))
		return ('l');
	else if (S_ISSOCK(mode))
		return ('s');
	else
		return ('-');
}

char	get_acl_exe(char *path)
{
	acl_t acl;

	acl = NULL;
	if (listxattr(path, NULL, 0, XATTR_NOFOLLOW) > 0)
		return ('@');
	if ((acl = acl_get_link_np(path, ACL_TYPE_EXTENDED)))
	{
		acl_free(acl);
		return ('+');
	}
	return (' ');
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
	(l_time.tm_mon - f_time.tm_mon) > 5 || (l_time.tm_mon - f_time.tm_mon < 0)
	|| (l_time.tm_wday - f_time.tm_wday) < 0)
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

void	print_long(t_files *ls)
{
	t_idx i;
	if (ls)
	{
		ft_bzero(&i, sizeof(t_idx));
		index_details(ls, &i);
		if (g_print_inode)
			ft_printf("%-*llu ", i.ino_w, ls->fstat.st_ino);
		ft_ls_mode(ls->fstat.st_mode, ls->path);
		ft_printf("%-*d ", i.lin_w, ls->fstat.st_nlink);
		print_usr_grp(ls, i);
		ft_printf("%-*d ", i.siz_w, ls->fstat.st_size);
		print_time(ls);
		ft_printf("%s", ls->color);
		ft_printf("%s\n"P_NC, ls->name);
	//	ls = ls->next;
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
	ls_driver(files);
	if (!g_recursive)
		print_directories(dir);
	if (g_recursive)
		print_dir(dir, 0);
	print_long(dir);
	free_list_argv(files);
	free_list_argv(dir);
}

int main(int argc, char *argv[])
{
	ft_ls_init();
	if (argc > 1)
		flag_driver(argc, argv);
//	print_ls_settings();
	printf("g_argc:%d argc: %d\n", g_argc, argc);
	if (g_argc && (g_argc < (argc - 1)))
		multi_argv(argc, argv);
	else if ((argc - g_argc) == 1)
		open_print(argv[g_argc]);
	else if (!g_argc)
		open_print(".");
/*
use the sorted list and index it into an index struct containing the max len
of each fields.
calculate total blocks
*/
//	ls_driver(files);
//	ls_driver(d);
	//print_dir(d);
	//ls_driver(d);
//	ls = inner_dir(".");
//	free_inner_dir(ls);
	//print_dir(ls);
//	for (int i = g_argc; i < argc; i++)
//		printf("%s\n", argv[i]);
//	print_list(ls);
//	print_dir(ls);
	/*if (stat(argv[1], &filestat) < 0)
		return 1;
	printf("File Permissions: \t\n");
    printf( (S_ISDIR(filestat.st_mode)) ? "d" : "-");
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
//	char *pa = argv[g_argc];
//	(!g_argc) && (pa = ".");
//	if ((dir = opendir(pa)) == NULL)
//	{
//		read_file(pa);
//		printf("ft_ls: %s:%s\n", argv[g_argc],strerror(errno));
//	}
//	while ((dp = readdir(dir)) != NULL)
//	{
//		if (!file_ignored(dp->d_name))
//		{
//		char *path = path_to_file(pa, dp->d_name);
//		//printf("path: %s\n", path);
//		if (lstat(path, &filestat) < 0)
//		{
//			printf("ft_ls: %s\n", strerror(errno));
//		}
//		if (!ls)
//			ls = create(dp->d_name, path, filestat);
//		else
//			append(&ls, dp->d_name, path, filestat);
//		}
//	}
//	print_list(ls);
//	printf("errno: %d\n", errno);
//	print_ls_settings();
//	printf("start argc: %d\nargc: %d\n", g_argc, argc);
//	(g_sort_reverse) ? merge_sort_list(&ls, strcmp_descend) : merge_sort_list(&ls, strcmp_ascend);
//	merge_sort_list(&ls);
//	print_list(ls);
//	ioctl(0, TIOCGWINSZ, &w);
//	t_pcol p;
//	printf("col: %hd\n", w.ws_col); 
//	col_format(ls, &p, w.ws_col);
//	list_globalize(ls, p);
//	test = g_sorted_file[0];
//	printf("test: %s\n", test->name);
//	print_global_list();
//	printf("count: %d max: %d col: %d row: %d\n", p.no_f, p.max, p.col, p.row);
	return (0);
}
