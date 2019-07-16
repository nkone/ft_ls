/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phtruong <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 11:59:34 by phtruong          #+#    #+#             */
/*   Updated: 2019/07/15 19:38:08 by phtruong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <sys/types.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <dirent.h>
# include <errno.h>
# include <stdbool.h>
# include <termios.h>
# include "libft/libft.h"
# include "libft/ft_printf.h"

# define LS_FLAGS "AFRSafilmnprtx1"


/*
** These macros are for color printing
** P_NC for no colors (end color printing mode)
*/
# define P_RED "\033[31m"
# define P_GREEN "\033[32m"
# define P_YELLOW "\033[33m"
# define P_BLUE "\033[34m"
# define P_MAGENTA "\033[35m"
# define P_CYAN "\033[36m"
# define P_NC "\033[0m"

/*
** These macros are for format printing
** Not compatible with all terminals
*/
# define P_BOLD "\033[1m"
# define P_DIM "\033[2m"
# define P_UNDERLINE "\033[4m"
# define P_BLINK "\033[5m"
# define P_REVERSE "\033[7m"
# define P_HIDDEN "\033[8m"

/*
** recreating switch operation using jump table
*/
void				case_ua(void);
void				case_uf(void);
void				case_ur(void);
void				case_us(void);
void				case_a(void);
void				case_f(void);
void				case_i(void);
void				case_l(void);
void				case_m(void);
void				case_n(void);
void				case_p(void);
void				case_r(void);
void				case_t(void);
void				case_x(void);
void				case_1(void);

/*
** jump table g_ls_flags
** Table consists of void return functions with void paramters
** Recreating switch operation (switch is forbidden in 42 school)
** uf means uppercase F (uppercase is forbidden in snake casting)
** t_ for table and g_ for global variables
*/
typedef void		t_ls_jump(void);

static t_ls_jump	*g_ls_flags[] =
{
	case_ua,
	case_uf,
	case_ur,
	case_us,
	case_a,
	case_f,
	case_i,
	case_l,
	case_m,
	case_n,
	case_p,
	case_r,
	case_t,
	case_x,
	case_1
};

/*
**===========================================================|
**|                 	FORMAT TABLE                         |
**|---------------+------------------------------------------|
**| Name          | Description                              |
**|---------------+------------------------------------------|
**| long_format   | list in details for -l and similar flags |
**| one_per_line  | for -1                                   |
**| many_per_line | default options sort vertically          |
**| horizontal    | for -x sort horizontally                 |
**| with_commas   | for -m print with commas                 |
**|---------------+------------------------------------------|
*/

/*
** e_ is naming covention for enum structure
*/
enum		e_format
{
	long_format,
	one_per_line,
	many_per_line,
	horizontal,
	with_commas
};

/*
** rename e_ structure to desirable name
*/
static enum e_format format;

/*
** sort_name is default
** sort_time for -t
*/
enum	e_sort_type
{
	sort_none = -1,
	sort_name,
	sort_time,
	sort_size
};

static enum e_sort_type sort_type;

/*
** none is default
** slash for -p
** classify for -F
*/

enum	e_indicator_style
{
	none,
	slash,
	classify
};

static enum e_indicator_style indicator_style;

/*
** DOT_AND_DOTDOT for -A
** Prints hidden files starting with '.' except for '.' and '..' themselves.
** MINIMAL for -a
** Prints everything including '.' and '..'
*/

enum	e_ignore_mode
{
	DEFAULT,
	DOT_AND_DOTDOT,
	MINIMAL
};

static enum e_ignore_mode ignore_mode;

/*
** for -r
** comparison is done using strcmp;
** 10 preceeds 2;
** if true then the opposite;
*/

static bool g_sort_reverse;

/*
** for -i
** print inode number before each file
*/

static bool g_print_inode;

/*
** for -R
** Display contents inside each directories found.
*/

static bool g_recursive;

/*
** for -n
** print user and group id as number
*/

static bool g_numeric_id;

/*
** to find starting argc
*/

static int	g_argc = 0;

static void	**g_sorted_file;

/*
** typedef struct stat to t_stat for naming convention
*/
typedef struct stat	t_stat;

/*
** struct s_files
** linked list structure that stores filenames and their stats
** Parameters:
** 		[name]: name of file or directory
** 		[fstat]: built in stat structure
**		[next]: pointer to next node
*/
typedef struct		s_files
{
	char			*name;
	char			*path;
	struct stat		fstat;
	char			*color;
	struct s_files	*next;
}					t_files;

/*
** s_pcol (print columns)
** a helper struct for column printing support
** Paramters:
** 		[col]: columns to be printed
**		[row]: total rows
**		[no_f]: number of files
**		[max]: file name max len
*/
typedef struct		s_pcol
{
	int				col;
	int				row;
	int				no_f;
	int				max;
}					t_pcol;

#endif
