/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phtruong <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 11:59:34 by phtruong          #+#    #+#             */
/*   Updated: 2019/06/22 19:46:54 by phtruong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <dirent.h>
# include <errno.h>
# include <stdbool.h>
# include "libft/libft.h"
# include "libft/ft_printf.h"

# define LS_FLAGS "FRafilmnprtx1"
# define LS_UR (1U << 0)
# define LS_LA (1U << 1)
# define LS_LL (1U << 2)
# define LS_LR (1U << 3)
# define LS_LT (1U << 4)

void				case_F(void);
void				case_R(void);
void				case_a(void);
void				case_f(void);
void				case_l(void);
void				case_m(void);
void				case_n(void);
void				case_p(void);
void				case_r(void);
void				case_t(void);
void				case_x(void);
void				case_1(void);

typedef void		t_ls_jump(void);

static t_ls_jump	*g_ls_flags[] =
{
	case_F,
    case_R,
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

enum	e_format
{
	long_format,
	one_per_line,
	many_per_line,
	horizontal,
	with_commas
};

static enum e_format format;

/*
** sort_name is default
** sort_time for -t
*/

enum	e_sort_type
{
	sort_none = -1,
	sort_name,
	sort_time
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
** MINIMAL for -a
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
** if false then lowest first character comes first;
** 1 preceeds 10;
** if true then highest alphabet comes first;
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
** print user and group id as number
*/

static bool g_numeric_id;

#endif
