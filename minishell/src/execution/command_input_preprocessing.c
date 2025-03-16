/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_input_preprocessing.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:06:54 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 21:46:11 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Merges two arrays of command arguments into one.
 *        It concatenates the arguments from the `f_args` array with those from the `_cmd_` array.
 * @param f_args The first array of command arguments.
 * @param cmd The second array of command arguments to append to `f_args`.
 * @return A new array containing all the arguments from both `f_args` and `_cmd_`, 
 *         or NULL if memory allocation fails.
 */
char	**merge_command_args(char **f_args, char **cmd)
{
	int				a;
	char			**new_args;

	if (!f_args)
		return (NULL);
	a = 1;
	while (cmd[0] && cmd[a])
		a++;
	new_args = malloc((a + 1) * sizeof(char *));
	if (!new_args)
		return (NULL);
	a = 0;
	if (f_args[a])
		new_args[a] = strcopy(f_args[a]);
	else
		new_args[a] = strcopy("");
	if (cmd && cmd[0])
	{
		while (cmd[++a])
			new_args[a] = strcopy(cmd[a]);
	}
	new_args[a] = 0;
	free_string_array(f_args);
	return (new_args);
}
