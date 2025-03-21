/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufiguei <lufiguei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:11:40 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/21 12:41:17 by lufiguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** 
 * @brief Sets the new "PWD" (current working directory)
 *  value in the environment.
 * 
 * @param new_ The new directory path.
 * @param env The environment to update.
 * @param c Unused parameter.
 */
void	set_new_pwd_in_env(char *new_, t_env *env, int c)
{
	char				**exp_pwd;
	int					a;
	int					b;

	a = 0;
	b = 4;
	(void)c;
	exp_pwd = malloc(3 * sizeof(char *));
	if (!exp_pwd)
		return ;
	exp_pwd[0] = strcopy("export");
	exp_pwd[1] = malloc(sizeof_str(new_, '\0') + 5);
	if (!exp_pwd[1])
		return ;
	s_strcopy(exp_pwd[1], "PWD=", 0, 4);
	while (new_[a])
		exp_pwd[1][b++] = new_[a++];
	exp_pwd[1][b] = '\0';
	exp_pwd[2] = 0;
	unset_or_export_cmd(exp_pwd, env, NULL, &a);
	free_string_array(exp_pwd);
}

/** 
 * @brief Changes the current working directory.
 * 
 * @param path The directory path to change to.
 * @param env The environment containing the "HOME" variable.
 * 
 * @return Status of the `chdir()` operation (0 if successful, -1 if failure).
 */
int	change_current_directory(char *path, t_env *env)
{
	int					status;
	int					a;

	if (!path || !sizeof_str(path, '\0'))
	{
		a = find_env_var_index(env, "HOME");
		if (a >= 0)
			status = chdir(env->parsed_env[a][1]);
		else
			status = -1;
	}
	else
		status = chdir(path);
	return (status);
}

/** 
 * @brief Retrieves the current working directory.
 * 
 * @param size The initial size for the buffer.
 * @param tries Number of retry attempts in case of failure.
 * @param fd File descriptor for error messages.
 * 
 * @return The current working directory as a string, or NULL on failure.
 */
char	*get_current_working_directory(int size, int tries, int fd)
{
	char				*buffer;

	buffer = malloc(size);
	if (!buffer)
		return (NULL);
	if (!getcwd(buffer, size))
	{
		free(buffer);
		if (tries < 10)
			return (get_current_working_directory(size + 50, tries + 1, fd));
		else
		{
			ft_putendl_fd("  err: cd() / pwd(): getcwd(): you are lost", fd);
			return (NULL);
		}
	}
	return (buffer);
}

/** 
 * @brief Sorts a 2D array of strings based on 
 * the first element of each sub-array.
 * 
 * @param _array The 2D array to be sorted.
 * @param _si The size of the array.
 * 
 * @return The sorted array.
 */
char	***sort_array(char ***_array, int _si)
{
	int					a;
	int					b;
	int					c;
	char				**temp_p;

	a = 0;
	while (a < _si - 1)
	{
		b = a + 1;
		c = string_weight_compare(_array[a][0], _array[b][0]);
		if (c)
		{
			temp_p = _array[a];
			_array[a] = _array[b];
			_array[b] = temp_p;
		}
		a++;
	}
	if (check_array_arrangment(_array, _si))
		return (_array);
	return (sort_array(_array, _si));
}

/** 
 * @brief Prints the environment variables in 
 * the export format to a file descriptor.
 * 
 * @param env The environment to print.
 * @param _out_fd The file descriptor to print to.
 */
void	print_export_declaration_to_fd(t_env *env, int *_out_fd)
{
	char				***new_array;
	int					a;

	a = 0;
	while (env->parsed_env[a] != 0)
		a++;
	if (!a)
		return ;
	new_array = duplicate_env_structure(env, a, 'F', -1);
	new_array[a] = 0;
	new_array = sort_array(new_array, a);
	print_export_vars(new_array, a, _out_fd[1]);
	free_environment_variables(new_array);
}
