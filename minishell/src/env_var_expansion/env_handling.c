/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alawrence <alawrence@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:12:21 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/21 16:41:19 by alawrence        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Checks if a variable in the string is valid to start.
 * 
 * Validates if the character at the specified index in the 
 * string is the start of a valid variable, based on whether
 *  the condition (`con`) is met.
 * If `con` is true, checks for variables that start with a dollar
 *  sign `$` followed by valid characters. 
 * If `con` is false, checks for variables that do not start with 
 * `$` and consist of alphanumeric characters or `_` or `?`.
 * @param str The input string.
 * @param index The index in the string to check.
 * @param con Condition that determines which validation to apply.
 * @return 1 if the variable start is valid, 0 otherwise.*/
int	is_valid_variable_start(char *str, int index, int con)
{
	if ((con && str[index] == '$'
			&& str[index + 1]
			&& str[index + 1] != '$'
			&& !ft_isspace(str[index + 1])
			&& (ft_isalnum(str[index + 1])
				|| str[index + 1] == '_'
				|| str[index + 1] == '?'))
		|| (!con && str[index]
			&& str[index] != '$'
			&& !ft_isspace(str[index])
			&& (ft_isalnum(str[index])
				|| str[index] == '_'
				|| str[index] == '?')))
		return (1);
	return (0);
}

/** @brief Appends a new value to an existing environment variable
 *  or creates a new one.
 * If the environment variable already exists, the function appends
 *  the new value to it (handling the case where `+` is present in 
 * the input).
 * If the variable does not exist, it creates a new entry with the
 *  given value.
 * @param var The environment variable to append or create.
 * @param env The environment structure containing the environment 
 * variables.*/
void	append_env_var(char *var, t_env *env)
{
	int				b;
	int				c;
	int				d;
	int				o;
	char			*env_var;

	c = sizeof_str(var, '+');
	d = sizeof_str(var, '\0') - c - 1;
	env_var = malloc(c + 1);
	s_strcopy(env_var, var, 0, c);
	o = find_env_var_index(env, env_var);
	free(env_var);
	if (o >= 0)
	{
		b = sizeof_str(env->original_env[o], '\0');
		env_var = malloc(b + d + 1);
		if (!env_var)
			return ;
		s_strcopy(env_var, env->original_env[o], 0, b);
		s_strcopy(env_var + b, var, c + 2, sizeof_str(var, '\0'));
	}
	else
		env_var = str_without_char(var, '+');
	replace_env_var(env_var, env);
	free(env_var);
}

/** @brief Finds the index of an environment variable in the parsed environment.
 * Searches for a matching environment variable name and returns
 *  its index in the parsed environment.
 * If the variable is not found, it returns -1.
 * @param env The environment structure.
 * @param name The name of the environment variable to search for.
 * @return The index of the environment variable, or -1 if not found.*/
int	find_env_var_index(t_env *env, char *name)
{
	int				a;

	a = 0;
	while (env->parsed_env[a] != 0)
	{
		if (str_cmp(env->parsed_env[a][0], name, NULL))
			return (a);
		a++;
	}
	return (-1);
}
