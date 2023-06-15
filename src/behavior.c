/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   behavior.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ode-cleb <ode-cleb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:24:04 by ode-cleb          #+#    #+#             */
/*   Updated: 2023/06/15 16:55:34 by ode-cleb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_the_complet_path(t_all *all)
{
	int		len;
	char	*current_path;
	char	*tmp;

	current_path = NULL;
	tmp = all->order.lst->content[0];
	if (all->order.lst->content[0][0] == '/')
		return (current_path = tmp);
	len = ft_strlen(all->order.path_lst->pcontent) + ft_strlen(tmp);
	current_path = ft_strnjoin(all->order.path_lst->pcontent, tmp, len);
	return (current_path);
}

void	execute(t_all *all, char *c_p)
{
	int	i;

	i = 0;
	while (i != all->order.nb_path
		&& execve(c_p, all->order.lst->content, all->order.envp) == -1)
	{
		free(c_p);
		all->order.path_lst = all->order.path_lst->next;
		++i;
		c_p = get_the_complet_path(all);
	}
}

void	child_behavior(t_all *all)
{
	char	*current_path;

	current_path = NULL;
	if (dup2(all->fd.write_end, STDOUT_FILENO) == -1)
	{
		perror("Error during redirection of the writing in the pipe");
		exit(EXIT_FAILURE);
	}
	if (dup2(all->fd.fd_file1, STDIN_FILENO) == -1)
	{
		perror("Error during redirection of the writing file");
		exit(EXIT_FAILURE);
	}
	close(all->fd.write_end);
	close(all->fd.read_end);
	current_path = get_the_complet_path(all);
	execute(all, current_path);
	perror("Command 1 not found");
	exit(EXIT_FAILURE);
}

void	child2_behavior(t_all *all)
{
	char	*current_path;

	current_path = NULL;
	if (dup2(all->fd.read_end, STDIN_FILENO) == -1)
	{
		perror("Error during redirection of the reading of the pipe");
		exit(EXIT_FAILURE);
	}
	if (dup2(all->fd.fd_file2, STDOUT_FILENO) == -1)
	{
		perror("Error during redirection of final file");
		exit(EXIT_FAILURE);
	}
	close(all->fd.write_end);
	close(all->fd.read_end);
	all->order.lst = all->order.lst->next;
	current_path = get_the_complet_path(all);
	execute(all, current_path);
	perror("Command not found");
	exit(EXIT_FAILURE);
}

int	parents_behavior(t_all *all)
{
	close(all->fd.write_end);
	close(all->fd.read_end);
	waitpid(all->fd.pid, NULL, 0);
	waitpid(all->fd.pid2, NULL, 0);
	return (0);
}
