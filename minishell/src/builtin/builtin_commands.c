/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:11:51 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/23 16:42:41 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles the echo command, printing arguments 
 * with or without a newline.
 * @param cmd The command arguments.
 * @param _out_fd Output file descriptor for printing.
 * @return 0 on success.
 */
int	ft_echo(char **cmd, int *_out_fd)
{
	int		a;
	int		op_num;

	op_num = 0;
	if (cmd[0] && cmd[1] && is_valid_echo_param(cmd[1]))
		op_num = 1;
	a = op_num + 1;
	while (op_num && cmd[a] && is_valid_echo_param(cmd[a]))
		a++;
	if ((cmd[0] && cmd[a]) || sizeof_str(cmd[a], '\0'))
	{
		while (1)
		{
			ft_putstr_fd(cmd[a], _out_fd[1]);
			a++;
			if (cmd[a])
				write(_out_fd[1], " ", 1);
			else
				break ;
		}
	}
	if (!op_num)
		write(_out_fd[1], "\n", 1);
	return (0);
}

/**
 * @brief Handles the `env` or `pwd` command, printing 
 * environment variables or current directory.
 * @param cmd Command string (either "env" or "pwd").
 * @param env The environment variables.
 * @param con Condition to determine export output.
 * @param _out_fd Output file descriptor for printing.
 * @return 0 on success, 256 on failure.
 */
int	env_or_pwd_cmd(char **cmd, t_env *env, int con, int *_out_fd)
{
	int		a;
	char	*absolute_pwd;

	a = -1;
	if (cmd[1] && str_cmp(cmd[0], "pwd", NULL))
		return (ft_putendl_fd("pwd: too many arguments.", 2), 1);
	if (str_cmp(cmd[0], "env", NULL))
	{
		if (cmd[1])
			return (ft_putendl_fd("env: no such file or directory.",
					_out_fd[1]), 127);
		if (con)
			print_export_declaration_to_fd(env, _out_fd);
		else
		{
			while (env->parsed_env[++a])
				print_env_var_to_fd(
					env->parsed_env[a][0], env->parsed_env[a][1], _out_fd[1]);
		}
		return (0);
	}
	absolute_pwd = get_current_working_directory(100, 5, _out_fd[1]);
	if (absolute_pwd)
		return (ft_putendl_fd(absolute_pwd, _out_fd[1]), free(absolute_pwd), 0);
	return (256);
}

/**
 * @brief Handles the export command to set environment variables.
 * @param cmd Command arguments.
 * @param env The environment.
 * @param _out_fd Output file descriptor for printing.
 * @param s Status flag.
 * @return Updated command arguments.
 */
char	**ft_export(char **_cmd, t_env *env, int *_out_fd, int **s)
{
	int				a;
	int				b;

	a = 1;
	while (_cmd[a])
	{
		b = export_statment_check(_cmd[a]);
		if (b > 0)
		{
			if (b >= 1 && _cmd[a][b] == '+')
				append_env_var(_cmd[a], env);
			else
				replace_env_var(_cmd[a], env);
		}
		else
		{
			ft_putstr_fd("   err: export(\'", _out_fd[1]);
			ft_putstr_fd(_cmd[a], _out_fd[1]);
			ft_putendl_fd("\'): Invalid variable name or format for export",
				_out_fd[1]);
			**s = 256;
		}
		a++;
	}
	return (_cmd);
}

/**
 * @brief Handles both `unset` and `export` commands.
 * @param _cmd Command arguments.
 * @param env The environment.
 * @param _out_fd Output file descriptor for printing.
 * @param s Status flag.
 * @return Updated command arguments.
 */
char	**unset_or_export_cmd(char **cmd, t_env *env, int *_out_fd, int *s)
{
	char			**util;

	util = NULL;
	*s = 0;
	if (cmd[1] && str_cmp(cmd[0], "unset", NULL) && env->parsed_env[0])
		unset_helper(cmd, env, s);
	else if (str_cmp(cmd[0], "export", NULL))
	{
		if (export_print_or_export(cmd))
			cmd = ft_export(cmd, env, _out_fd, &s);
		else
		{
			util = malloc(sizeof(char *) * 2);
			util[0] = ft_strdup("env");
			util[1] = NULL;
			env_or_pwd_cmd(util, env, 1, _out_fd);
			free_string_array(util);
		}
	}
	return (cmd);
}

/**
 * @brief Handles the `cd` command to change the current directory.
 * @param _cmd Command arguments.
 * @param env The environment.
 * @param _out_fd Output file descriptor for printing.
 * @return 0 on success, 256 on failure.
 */
/* int	ft_cd(char **cmd, t_env *env, int *_out_fd)
{
	int					a;
	char				*new_path;
	char				*current_pwd;

	new_path = handle_edge(cmd, env, _out_fd);
	if (!new_path)
		return (256);
	current_pwd = get_current_working_directory(100, 5, _out_fd[1]);
	if (change_current_directory(new_path, env) < 0)
		return (free(current_pwd), free(new_path),
			ft_putendl_fd("  err: cd(): No such file or directory",
				_out_fd[1]), 256);
	free(new_path);
	set_old_pwd_in_env(current_pwd, env);
	a = find_env_var_index(env, "PWD");
	if (a >= 0)
		remove_env_entry(env, a);
	new_path = get_current_working_directory(100, 5, _out_fd[1]);
	if (new_path)
		return (set_new_pwd_in_env(new_path, env, a), free(new_path), 0);
	return (0);
} */
int	ft_cd(char **_cmd, t_env *env, int *_out_fd)
{
	int					a;
	char				*new_path;

	if (_cmd[1] && _cmd[2])
		ft_putendl_fd("  err: cd(): Too many arguments", _out_fd[1]);
	else
	{
		if (change_current_directory(_cmd[1], env) < 0)
			ft_putendl_fd(
				"  err: cd(): Only existing destinations", _out_fd[1]);
		else
		{
			a = find_env_var_index(env, "PWD");
			if (a >= 0)
				remove_env_entry(env, a);
			new_path = get_current_working_directory(100, 5, _out_fd[1]);
			if (new_path)
			{
				set_new_pwd_in_env(new_path, env, a);
				free(new_path);
			}
			return (0);
		}
	}
	return (256);
}
