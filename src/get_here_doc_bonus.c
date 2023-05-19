/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_here_doc_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jchakir <jchakir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 11:25:13 by jchakir           #+#    #+#             */
/*   Updated: 2022/02/05 14:02:52 by jchakir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static char	*ft_gunl_strjoin(char const *s1, char const *s2)
{
	char	*str;
	char	*save_str;

	if (! s1)
		s1 = "";
	str = malloc(ft_strlen(s1) + 2);
	if (! str)
		return (NULL);
	save_str = str;
	while (*s1)
		*str++ = *s1++;
	*str++ = *s2;
	*str = 0;
	return (save_str);
}

static char	*ft_get_until_new_line(void)
{
	char	buff[1];
	char	*str;
	char	*temp_str;
	ssize_t	read_size;

	str = NULL;
	while (1)
	{
		read_size = read(0, buff, 1);
		if (read_size <= 0)
			return (NULL);
		temp_str = ft_gunl_strjoin(str, buff);
		free(str);
		str = temp_str;
		if (! temp_str)
			return (NULL);
		if (buff[0] == '\n')
			break ;
	}
	return (str);
}

static void	ft_get_here_doc_with_gunl(char *limiter, int outfd)
{
	char	*line;

	limiter = ft_strjoin(limiter, "\n");
	if (! limiter)
		exit (1);
	while (1)
	{
		ft_putstr_fd("heredoc> ", 1);
		line = ft_get_until_new_line();
		if (! line)
			break ;
		if (! ft_strncmp(line, limiter, ft_strlen(limiter) + 1))
			break ;
		ft_putstr_fd(line, outfd);
		free(line);
	}
	free(line);
	free(limiter);
	exit (1);
}

int	ft_get_here_doc(char *limiter)
{
	int		fd_pipe[2];
	pid_t	pid;

	if (pipe(fd_pipe) < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
		return (-1);
	if (! pid)
		ft_get_here_doc_with_gunl(limiter, fd_pipe[1]);
	waitpid(pid, NULL, 0);
	close(fd_pipe[1]);
	return (fd_pipe[0]);
}
