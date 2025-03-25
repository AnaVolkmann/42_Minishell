/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands_utils4.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufiguei <lufiguei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:45:59 by lufiguei          #+#    #+#             */
/*   Updated: 2025/03/25 11:55:55 by lufiguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset_helper(char **cmd, t_env *env, int *s)
{
	int			a;
	int			c;

	a = 0;
	if (cmd[1][0] - '-' == 0)
		return ;
	else if (str_cmp(cmd[1], "ALL", NULL) == 1)
	{
		while (env->parsed_env[a] != 0)
			remove_env_entry(env, a);
		a++;
	}
	while (cmd[a])
	{
		c = find_env_var_index(env, cmd[a]);
		if (c >= 0)
			remove_env_entry(env, c);
		else
			*s = 256;
		a++;
	}
}

char	*handle_edge(char **cmd, t_env *env, int *_out_fd)
{
	char	*new_path;

	new_path = NULL;
	if (!cmd[1] || !ft_strncmp(cmd[1], "~", 2))
	{
		new_path = get_env_value(env, "HOME");
		if (!new_path)
			return (ft_putendl_fd("  err: cd(): HOME is not set.",
					_out_fd[1]), NULL);
	}
	else if (!ft_strncmp(cmd[1], "-", 2))
	{
		new_path = get_env_value(env, "OLDPWD");
		if (!new_path)
			return (ft_putendl_fd("  err: cd(): OLDPWD is not set.",
					_out_fd[1]), NULL);
		printf("%s\n", new_path);
	}
	else if (cmd[1] && cmd[2])
		return (ft_putendl_fd("  err: cd(): Too many arguments",
				_out_fd[1]), NULL);
	else
		new_path = ft_strdup(cmd[1]);
	return (new_path);
}

void	free_ast_child(t_ast_node *node)
{
	int	i;

	i = 0;
	if (!node)
		return ;
	while (node->parent != NULL)
		node = node->parent;
	if (node->type == T_WORD && node->args)
	{
		while (node->args && node->args[i])
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}
	free_ast(node->left);
	free_ast(node->right);
	free(node);
}

void	set_parent(t_ast_node *head)
{
	if (!head)
		return ;
	if (head->left)
		head->left->parent = head;
	if (head->right)
		head->right->parent = head;
}
