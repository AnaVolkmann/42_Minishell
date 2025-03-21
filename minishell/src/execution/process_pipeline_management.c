/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pipeline_management.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:11:33 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/21 12:19:35 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Manages file descriptors for child processes, 
 * handling input/output redirection.
 * @param pipe_data Array of pipe states and flags.
 * @param _fd File descriptors for input/output redirection.
 * @param fd_ Temporary file descriptors for the child process.*/
void	child_fds_managment(int *pipe_data, int *_fd, int *fd_)
{
	if (pipe_data[8] && pipe_data[6])
	{
		dup2(pipe_data[1], 0);
		safe_close(pipe_data[1]);
	}
	if (pipe_data[8] && pipe_data[7])
	{
		dup2(pipe_data[2], 1);
		safe_close(pipe_data[2]);
	}
	if (pipe_data[0] && pipe_data[0] <= pipe_data[5]
		&& (!pipe_data[8] || !pipe_data[6]))
		dup2(_fd[0], 0);
	if (pipe_data[0] > 1 && (!pipe_data[8] || !pipe_data[7]))
		dup2(fd_[1], 1);
	else
		safe_close(fd_[0]);
	safe_close(fd_[0]);
	safe_close(fd_[1]);
}

/** @brief Manages file descriptors for parent processes after
 *  child execution.
 * @param pipe_data Array of pipe states and flags.
 * @param _fd File descriptors for input/output redirection.
 * @param fd_ Temporary file descriptors for the parent process.*/
void	parent_fds_managment(int *pipe_data, int *_fd, int *fd_)
{
	if (pipe_data[8] && pipe_data[6])
	{
		safe_close(pipe_data[1]);
		pipe_data[6] = 0;
	}
	if (pipe_data[8] && pipe_data[7])
	{
		safe_close(pipe_data[2]);
		pipe_data[7] = 0;
	}
	if (!pipe_data[7] && !pipe_data[6])
		pipe_data[8] = 0;
	safe_close(fd_[1]);
	safe_close(_fd[0]);
	if (pipe_data[0] > 1)
		_fd[0] = fd_[0];
	else
		safe_close(fd_[0]);
}
