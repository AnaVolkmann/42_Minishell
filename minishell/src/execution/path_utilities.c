/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utilities.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:11:26 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/21 12:15:01 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates a file path by combining an environment variable
 *  with a file name.
 * @param env_var The environment variable containing the path.
 * @param file The file to append to the environment variable's value.
 * @param indx_s Array storing index positions to assist in path construction.
 * @return A newly created file path or NULL if an error occurs.
 */
char	*create_subpath_from_var(char *env_var, char *file, int *indx_s)
{
	char		*tmp_path;
	int			a;
	int			b;
	int			file_size;

	increment_path_index(env_var, indx_s, &a);
	file_size = sizeof_str(file, ' ');
	tmp_path = malloc((indx_s[1] - a) + file_size + 2);
	if (!tmp_path)
		return (NULL);
	b = 0;
	while (b < ((indx_s[1] - a) + file_size + 1))
	{
		if (indx_s[3] && (b < indx_s[1] - a))
			tmp_path[b] = env_var[a + b];
		else if (indx_s[3]
			&& env_var[indx_s[1] - 1] != '/'
			&& (b == indx_s[1] - a))
			tmp_path[b] = '/';
		else
			tmp_path[b] = file[b - (indx_s[1] - a) - indx_s[3]];
		b++;
	}
	tmp_path[b] = '\0';
	return (tmp_path);
}

/**
 * @brief Checks if a file exists in the system without using the 
 * environment variables.
 * @param file The file path to check.
 * @param mode The mode used to check access permissions.
 * @return The file path if it exists and is accessible, or NULL if not.
 */
char	*check_path_without_env(char *file, int mode)
{
	char				*tmp_path;
	int					b;

	b = sizeof_str(file, ' ');
	tmp_path = malloc(b + 1);
	if (!tmp_path)
		return (NULL);
	s_strcopy(tmp_path, file, 0, b);
	if (is_path_accessible(tmp_path, mode))
		return (tmp_path);
	free(tmp_path);
	return (NULL);
}

/**
 * @brief Resolves a file path using the environment's PATH 
 * variable or direct checking.
 * @param file The file or command to find.
 * @param envp The environment variable array containing PATH.
 * @param env_var The name of the environment variable (usually "PATH").
 * @param mode The mode used to check access permissions.
 * @return The resolved file path if found, or NULL if not.
 */
char	*get_file_path(char *file, char **envp, char *env_var, int mode)
{
	char				*tmp_path;
	int					indx_s[4];

	indx_s[3] = 0;
	indx_s[1] = sizeof_str(env_var, '\0');
	indx_s[0] = find_substr_index(envp, env_var, indx_s[1]);
	if (indx_s[0] < 0
		|| (file[0] && file[1] && file[0] == '.' && file[1] == '/'))
		return (check_path_without_env(file, mode));
	indx_s[2] = sizeof_str(envp[indx_s[0]], '\0');
	if (sizeof_str(file, ' ') != sizeof_str(file, '\0')
		&& !is_path_accessible(file, mode))
		return (NULL);
	while (envp[indx_s[0]][indx_s[1]])
	{
		tmp_path = create_subpath_from_var(envp[indx_s[0]], file, indx_s);
		if (!tmp_path)
			return (NULL);
		if (is_path_accessible(tmp_path, mode))
			return (tmp_path);
		free(tmp_path);
		if (!indx_s[3])
			indx_s[3] = 1;
	}
	return (NULL);
}

/**
 * @brief Extracts the next substring from a string, delimited
 *  by a specific character.
 * @param str The string to extract the substring from.
 * @param del The delimiter character.
 * @param index The current index for tracking substring extraction.
 * @return The extracted substring or NULL if an error occurs.
 */
char	*find_next_substring(char *str, char del, int *index)
{
	char		*sub;
	int			size;
	int			a;

	while (str[index[0]] && str[index[0]] == del)
		index[0] += 1;
	size = sizeof_str(str + index[0], del);
	sub = malloc(size + 1);
	if (!sub)
		return (NULL);
	a = 0;
	while (str[index[0]]
		&& str[index[0]] != del)
	{
		if (str[index[0]] != 34 && str[index[0]] != 39)
			sub[a++] = str[index[0]];
		index[0] += 1;
	}
	sub[a] = '\0';
	index[0] += 1;
	return (sub);
}

/**
 * @brief Prepares command arguments, resolving file paths using
 *  the PATH variable if needed.
 * @param cmd The command string to parse.
 * @param envp The environment variable array containing PATH.
 * @param c The current index for argument processing.
 * @return An array of command arguments, with resolved file paths,
 *  or NULL if an error occurs.
 */
char	**prepare_cmd_arguments(char *cmd, char **envp, int c)
{
	char		**cmd_arr;
	char		*cmd_holder;
	int			i[3];

	i[1] = count_substrings(cmd, ' ');
	cmd_arr = malloc((i[1] + 1) * sizeof(char *));
	i[0] = 0;
	while (c < i[1])
	{
		cmd_holder = find_next_substring(cmd, '\0', i);
		if (!c && !check_if_command_is_builtin(cmd_holder))
		{
			cmd_arr[c] = get_file_path(cmd_holder, envp, "PATH", X_OK);
			if (!cmd_arr[c])
				cmd_arr[c] = ft_strdup(cmd_holder);
			free(cmd_holder);
		}
		else
			cmd_arr[c] = cmd_holder;
		if (!cmd_arr[c])
			return (NULL);
		c += 1;
	}
	cmd_arr[c] = NULL;
	return (cmd_arr);
}
