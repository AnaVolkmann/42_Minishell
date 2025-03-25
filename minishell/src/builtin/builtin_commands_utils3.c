/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands_utils3.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufiguei <lufiguei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:11:48 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/25 11:41:05 by lufiguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Exits the program with a status code determined by the input command.
 * If more than one argument is provided, exits with status 1. 
 * If the first argument is not a numeric string, exits with status 255. 
 * Otherwise, converts the first argument to an integer and exits 
 * with that status.
 * @param _cmd_ The command array.*/
void	ft_exit(char **_cmd_, t_env *env, t_ast_node *head)
{
	int				status;

	status = 0;
	if ((_cmd_[1] && !is_string_numeric(_cmd_[1])))
	{
		ft_putendl_fd("Minishell: exit: numeric argument required.", 2);
		status = 2;
		return ;
	}
	else if (_cmd_[1] && _cmd_[2])
	{
		ft_putendl_fd("Minishell: exit: too many arguments.", 2);
		status = 1;
	}
	else if (_cmd_[1])
		status = (ft_atoi(_cmd_[1]) % 256);
	free_ast_child(head);
	cleanup_and_exit_shell(env, status);
}

/** @brief Checks whether the export command should print or 
 * execute the export operation.
 * Loops through the command arguments and returns 1 if 
 * at least one argument exists.
 * @param _cmd_ The command array.
 * @return 1 if export operation is valid, 0 otherwise.*/
int	export_print_or_export(char **_cmd_)
{
	int	i;

	i = 1;
	while (_cmd_[0] && _cmd_[i])
	{
		if (_cmd_[i])
			return (1);
		i++;
	}
	return (0);
}

/**  @brief Checks if the export statement has a valid format. 
 * Checks that the string before the `=` in the export 
 * command is a valid identifier, and that the syntax is correct.
 * @param _cmd_ The command string.
 * @return The position of the `=` if the format is valid, -1 if invalid.*/
int	export_statment_check(char *_cmd_)
{
	int							a;
	int							b;

	a = sizeof_str(_cmd_, '=');
	if (a > 1 && _cmd_[a - 1] == '+')
		a -= 1;
	if (a)
	{
		b = 0;
		while (b < a)
		{
			if (!b && !ft_isalpha(_cmd_[b]) && _cmd_[b] != '_')
				return (-1);
			else if (b && !ft_isalnum(_cmd_[b]) && _cmd_[b] != '_')
				return (-1);
			b++;
		}
	}
	return (a);
}

char	*get_env_value(t_env *env, char *key)
{
	int		index;
	char	*value;

	value = NULL;
	index = find_env_var_index(env, key);
	if (index >= 0)
		value = ft_strdup(env->parsed_env[index][1]);
	return (value);
}

void	set_old_pwd_in_env(char *old, t_env *env)
{
	char				**exp_pwd;
	int					a;
	int					b;
	int					i;

	a = 0;
	b = 7;
	i = find_env_var_index(env, "OLDPWD");
	if (i >= 0)
		remove_env_entry(env, i);
	exp_pwd = malloc(3 * sizeof(char *));
	if (!exp_pwd)
		return ;
	exp_pwd[0] = strcopy("export");
	exp_pwd[1] = malloc(sizeof_str(old, '\0') + 8);
	if (!exp_pwd[1])
		return ;
	s_strcopy(exp_pwd[1], "OLDPWD=", 0, 7);
	while (old[a])
		exp_pwd[1][b++] = old[a++];
	exp_pwd[1][b] = '\0';
	exp_pwd[2] = 0;
	unset_or_export_cmd(exp_pwd, env, NULL, &a);
	free_string_array(exp_pwd);
	free(old);
}
