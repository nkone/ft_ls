/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_print_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phtruong <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 19:36:36 by phtruong          #+#    #+#             */
/*   Updated: 2019/07/23 20:00:50 by phtruong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_mode(int mode, char *path)
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

void	print_classify(t_pcol p, t_files *f)
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

void	print_usr_grp(t_idx i, t_files *f)
{
	if (g_numeric_id)
	{
		ft_printf("%-*d  ", i.usr_w, f->fstat.st_uid);
		ft_printf("%-*d  ", i.grp_w, f->fstat.st_gid);
	}
	else
	{
		ft_printf("%-*s  ", i.usr_w, getpwuid(f->fstat.st_uid)->pw_name);
		ft_printf("%-*s  ", i.grp_w, getgrgid(f->fstat.st_gid)->gr_name);
	}
}

void	print_long_name(t_files *f)
{
	char link[NAME_MAX + 1];

	if (S_ISLNK(f->fstat.st_mode))
	{
		ft_bzero(link, NAME_MAX + 1);
		readlink(f->path, link, NAME_MAX);
		ft_printf("%s%s"P_NC"%c -> %s\n", f->color, f->name, f->style, link);
	}
	else
		ft_printf("%s%s"P_NC"%c\n", f->color, f->name, f->style);
}

void	print_time(t_files *f)
{
	time_t		t;
	struct tm	t_info;
	char		buf[26];

	t = f->fstat.st_mtime;
	localtime_r(&t, &t_info);
	if (!check_timestamp(t_info))
		print_year(ctime_r(&t, buf));
	else
		print_date_time(ctime_r(&t, buf));
}
