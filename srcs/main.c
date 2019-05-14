/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcorwin <jcorwin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 10:50:06 by sschmele          #+#    #+#             */
/*   Updated: 2019/05/14 16:20:17 by jcorwin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <dirent.h>

#include "ft_ls.h"

static void		print_dir(char *dirname, int flags);

static void putFileName(t_file *file)
{
	ft_putnbr(file->fromstart);
	ft_putstr(" - ");
	ft_putendl(file->name);
}

void			print_inner(t_file *file)
{
	if (ft_strcmp(file->name, "..") && ft_strcmp(file->name, ".") && file->buf.st_mode & S_IFDIR)
	{
		ft_putstr(file->name);
		ft_putstr(":\n");
//		print_dir(file->name);
		ft_putendl("");
	}
}

static void		print_dir(char *dirname, int flags)
{
	t_param			param;
	DIR				*dir;
	struct dirent	*entry;

	ft_bzero(&param, sizeof(param));
	dir = opendir(dirname);
	if (errno == ENOENT)
		ft_putstr("no file\n");
	else if (errno == ENOTDIR)
		ft_putstr("no dir\n");

	while ((entry = readdir(dir)))
	{
		if (((flags >> 4) & 1) == 0)
			if (*entry->d_name == '.')
				continue ;
		param.file = file_new(param.file);
		param.file->prev->name = entry->d_name;
		stat(entry->d_name, &(param.file->prev->buf));
	}
	file_count(param.file);
	if (param.file->prev->fromstart > 2)
		quickSort(param.file, param.file, param.file->prev, file_strcmp);
	param.file = file_slip(param.file, file_strcmp);
	file_foreach(param.file, putFileName);
	file_foreach(param.file, print_inner);
	file_del(param.file);
	closedir(dir);
}

int		main(int argc, char **argv)
{
	int		i;
	int		flags;

	flags = 0;
	i = get_args(&flags, argc, argv);
	if (i == argc)
		print_dir(".", flags);
	else
		while (i < argc)
			print_dir(argv[i++], flags);
	return (0);
}
