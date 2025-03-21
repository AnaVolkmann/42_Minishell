/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:22:35 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/21 18:22:37 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Sets up signal handlers for the program.
 * @details Configures signal handlers for SIGINT (Ctrl+C) 
 * and SIGQUIT (ignores Quit signal).*/
void	setup_signal_handlers(void)
{
	signal(SIGINT, handle_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
}

/** @brief Handles the SIGINT signal in the child process.
 * @param sig_num The signal number (e.g., SIGINT = 2).
 * @details Sets the global signal variable `g_signal` to 
 * 130 for SIGINT, or 131 for SIGQUIT.*/
void	child_ctrl_c(int sig_num)
{
	if (sig_num == 2)
		g_signal = 130;
	else
	{
		ft_putstr_fd("Quit", 1);
		g_signal = 131;
	}
	write(1, "\n", 1);
}

/** @brief Handles the SIGINT signal in the parent process.
 * @param a The signal number (unused).
 * @details Sets the global signal variable `g_signal`
 *  to 1, clears the current line, and triggers a new line display.*/
void	handle_ctrl_c(int a)
{
	(void)a;
	g_signal = 1;
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}
