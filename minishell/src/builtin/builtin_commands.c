/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufiguei <lufiguei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:11:51 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/18 14:08:52 by lufiguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles the echo command, printing arguments with or without a newline.
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
 * @brief Handles the `env` or `pwd` command, printing environment variables or current directory.
 * @param cmd Command string (either "env" or "pwd").
 * @param env The environment variables.
 * @param con Condition to determine export output.
 * @param _out_fd Output file descriptor for printing.
 * @return 0 on success, 256 on failure.
 */
int	env_or_pwd_cmd(char *cmd, t_env *env, int con, int *_out_fd)
{
	int		a;
	char	*absolute_pwd;

	a = -1;
	if (str_cmp(cmd, "env", NULL))
	{
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
	{
		ft_putendl_fd(absolute_pwd, _out_fd[1]);
		return (free(absolute_pwd), 0);
	}
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
			ft_putendl_fd("\'): Invalid variable name or format for export",_out_fd[1]);
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
	int				a;
	int				c;

	a = 1;
	*s = 0;
	if (cmd[a] && str_cmp(cmd[0], "unset", NULL) && env->parsed_env[0])
	{
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
	else if (str_cmp(cmd[0], "export", NULL))
	{
		if (export_print_or_export(cmd))
			cmd = ft_export(cmd, env, _out_fd, &s);
		else
			env_or_pwd_cmd("env", env, 1, _out_fd);
	}
	return (cmd);
}

static char	*handle_edge(char **cmd, t_env *env, int *_out_fd)
{
	char	*new_path;

	new_path = NULL;
	if (!cmd[1] || !ft_strncmp(cmd[1], "~", 2))
	{
		new_path = get_env_value(env, "HOME");
		if (!new_path)
			return (ft_putendl_fd("  err: cd(): HOME is not set.", _out_fd[1]), NULL);
	}
	else if (!ft_strncmp(cmd[1], "-", 2))
	{
		new_path = get_env_value(env, "OLDPWD");
		if (!new_path)
			return (ft_putendl_fd("  err: cd(): OLDPWD is not set.", _out_fd[1]), NULL);
		printf("%s\n", new_path);
	}
	else if (cmd[1] && cmd[2])
		return (ft_putendl_fd("  err: cd(): Too many arguments", _out_fd[1]), NULL);
	else
		new_path = ft_strdup(cmd[1]);
	return (new_path);
}

/**
 * @brief Handles the `cd` command to change the current directory.
 * @param _cmd Command arguments.
 * @param env The environment.
 * @param _out_fd Output file descriptor for printing.
 * @return 0 on success, 256 on failure.
 */
int	ft_cd(char **cmd, t_env *env, int *_out_fd)
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
				ft_putendl_fd("  err: cd(): Only existing destinations are allowed", _out_fd[1]), 256);
	free(new_path);
	set_old_pwd_in_env(current_pwd, env);
	a = find_env_var_index(env, "PWD");
	if (a >= 0)
		remove_env_entry(env, a);
	new_path = get_current_working_directory(100, 5, _out_fd[1]);
	if (new_path)
		return (set_new_pwd_in_env(new_path, env, a), free(new_path), 0);
	return (0);
}
