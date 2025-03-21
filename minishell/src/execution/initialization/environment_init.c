/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:23:49 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/21 18:25:24 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Duplicates the environment variables array.
 * @param env The original environment variables array.
 * @return A new array of environment variables, or NULL on failure.
 */
char	**duplicate_environment_variables(char **env)
{
	int				a;
	int				b;
	char			**new_old;

	a = 0;
	b = 0;
	while (env[a])
		a++;
	new_old = malloc((a + 1) * sizeof(char **));
	if (!new_old)
		return (NULL);
	while (b < a)
	{
		new_old[b] = strcopy(env[b]);
		b++;
	}
	new_old[b] = 0;
	return (new_old);
}

/**
 * @brief Initializes the shell's environment structure
 *  by parsing the environment variables.
 * @param env The environment structure to initialize.
 * @param original_env The original environment variables array.
 * @param a Index to start parsing from.
 * @return 1 on success, 0 on failure.
 */
int	initialize_shell_environment_structure(
		t_env *env, char **original_env, int a)
{
	int				b;
	int				c;

	env->original_env = duplicate_environment_variables(original_env);
	while (original_env[a])
		a++;
	env->parsed_env = malloc((a + 1) * sizeof(char ***));
	if (!env->parsed_env)
		return (0);
	b = -1;
	while (++b < a)
	{
		c = sizeof_str(original_env[b], '=');
		env->parsed_env[b] = malloc(2 * sizeof(char **));
		env->parsed_env[b][0] = malloc(c * sizeof(char *));
		env->parsed_env[b][1] = malloc(
				(sizeof_str(original_env[b], '\0') - c) * sizeof(char *));
		if (!env->parsed_env[b] || !env->parsed_env[b][0]
			|| !env->parsed_env[b][1])
			return (0);
		s_strcopy(env->parsed_env[b][0], original_env[b], 0, c);
		s_strcopy(env->parsed_env[b][1], original_env[b],
			c + 1, sizeof_str(original_env[b], '\0'));
	}
	return (env->parsed_env[b] = 0, 1);
}

/**
 * @brief Initializes default shell variables like `SHELL`, `?`, and `PWD`.
 * @param env The environment structure to modify.
 * @param a Index for variable lookup (not used).
 */
void	initialize_default_variables(t_env *env, int a)
{
	char					*new_pwd;

	a = find_env_var_index(env, "SHELL");
	if (a >= 0)
		remove_env_entry(env, a);
	replace_env_var("SHELL=minishell", env);
	replace_env_var("?=0", env);
	a = find_env_var_index(env, "PWD");
	new_pwd = get_current_working_directory(100, 5, 2);
	if (new_pwd)
	{
		if (a >= 0)
			remove_env_entry(env, a);
		set_new_pwd_in_env(new_pwd, env, a);
		free(new_pwd);
	}
}

/**
 * @brief Initializes the shell environment with the 
 * provided original environment.
 * @param env The environment structure to initialize.
 * @param original_env The original environment variables.
 * @return 1 on success, 0 on failure.
 */
int	initialize_shell_with_environment(t_env *env, char **original_env)
{
	int					index;
	int					status;
	int					a;

	if (!env)
		return (0);
	status = initialize_shell_environment_structure(env, original_env, 0);
	a = find_env_var_index(env, "SHLVL");
	index = 0;
	if (a >= 0)
		index = string_to_int(env->parsed_env[a][1]);
	update_env_status(env, index + 1, "SHLVL=");
	initialize_default_variables(env, 0);
	return (status);
}
