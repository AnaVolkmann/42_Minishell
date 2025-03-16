/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_managment2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:12:31 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 21:14:06 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** 
 * @brief Replaces or adds an environment variable in the environment structure.
 * 
 * Searches for an existing environment variable and replaces it with the new value if found. 
 * If the variable does not exist, it adds a new environment variable entry. 
 * The function handles different cases of variable formatting, such as with or without an equals sign (`=`).
 * 
 * @param var The environment variable to replace or add.
 * @param env The environment structure containing the environment variables.
 */
void	replace_env_var(char *var, t_env *env)
{
	int				c;
	int				o;
	char			*env_var;

	c = sizeof_str(var, '=');
	env_var = malloc(c + 1);
	if (!env_var)
		return ;
	s_strcopy(env_var, var, 0, c);
	o = find_env_var_index(env, env_var);
	if (o >= 0)
		remove_env_entry(env, o);
	free(env_var);
	if (c > 0 && c < sizeof_str(var, '\0') - 1)
		add_env_entry(env, var, c, 1);
	else if (var[c] == '=')
		add_env_entry(env, var, c, 0);
	else if (c == sizeof_str(var, '\0'))
		add_env_entry(env, var, c, -1);
}

/** 
 * @brief Updates the status variable in the environment structure.
 * 
 * Converts the given status value to a string and updates an environment variable with a given start string.
 * The updated variable will reflect the status in the environment.
 * 
 * @param env The environment structure.
 * @param status The status value to set.
 * @param start The string that starts the environment variable name.
 */
void	update_env_status(t_env *env, int status, char *start)
{
	char					*var;
	int						a;

	a = count_digits_in_int(status) + sizeof_str(start, '\0') + 1;
	var = malloc(a);
	if (!var)
		return ;
	s_strcopy(var, start, 0, sizeof_str(start, '\0'));
	var[--a] = '\0';
	if (!status)
		var[--a] = '0';
	while (status)
	{
		var[--a] = (status % 10) + 48;
		status /= 10;
	}
	replace_env_var(var, env);
	free(var);
}

/** 
 * @brief Checks if the line is empty or contains only whitespace characters.
 * 
 * This function checks if the given string consists solely of spaces, tabs, or newlines and returns true if the string is empty or consists only of whitespace characters.
 * 
 * @param line The input string.
 * 
 * @return 1 if the line is empty or contains only whitespace characters, 0 otherwise.
 */
int	is_space(char *line)
{
	int				a;

	a = 0;
	while (line[a] == ' ' || line[a] == '\t'
		|| line[a] == '\n')
		a += 1;
	if (line[a] == '\0')
		return (1);
	return (0);
}

/** 
 * @brief Checks if the provided line is empty, a newline, or consists of only whitespace characters.
 * 
 * This function checks if the line is either empty, a newline (`\n`), or just whitespace. It frees the line if it meets any of these conditions.
 * 
 * @param line The string to check.
 * 
 * @return 1 if the line is empty or only whitespace, 0 otherwise.
 */
int	check_line(char **line)
{
	if (*line[0] == '\0' || str_cmp(*line, "\n", NULL) || is_space(*line))
	{
		free(*line);
		return (1);
	}
	return (0);
}

/** 
 * @brief Removes a specified character from a string.
 * 
 * This function creates a new string that is a copy of the original string with all instances of the specified character (`del`) removed.
 * 
 * @param str The input string.
 * @param del The character to remove from the string.
 * 
 * @return A new string with the character removed, or NULL if memory allocation fails.
 */
char	*str_without_char(char *str, char del)
{
	int				a;
	int				b;
	char			*new_str;

	a = 0;
	b = 0;
	new_str = malloc(sizeof_str(str, '\0') + 1);
	if (!new_str)
		return (NULL);
	while (str[a])
	{
		if (str[a] != del)
			new_str[b++] = str[a];
		a++;
	}
	new_str[b] = '\0';
	return (new_str);
}
