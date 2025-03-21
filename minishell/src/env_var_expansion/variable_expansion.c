/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:23:33 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/21 18:25:24 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_variable_in_string(char *var, t_env *env, int a, int *f_hole)
{
	int							hole_size;
	int							c;
	int							b;
	char						*new_var;

	b = a + 1;
	while (var[a] && is_valid_variable_start(var, b, 0))
		b++;
	hole_size = b - a;
	new_var = malloc(hole_size + 1);
	s_strcopy(new_var, var, a + 1, b);
	if (str_cmp(new_var, "?", NULL) && g_signal)
		update_env_status(env, g_signal, "?=");
	g_signal = 0;
	c = find_env_var_index(env, new_var);
	free(new_var);
	*f_hole = 0;
	if (c >= 0)
	{
		new_var = replace_variable_with_value(var, env->parsed_env[c][1], a, b);
		*f_hole = sizeof_str(env->parsed_env[c][1], '\0') + a;
	}
	else
		new_var = replace_variable_with_value(var, "", a, b);
	return (new_var);
}

void	skip_single_quotes(char *var, int *f_arr)
{
	f_arr[0]++;
	f_arr[1]++;
	while (!(f_arr[2] % 2) && var[f_arr[0]] && var[f_arr[0]] != 39)
		f_arr[0]++;
}

char	*recursively_expand_variables(char *var,
			t_env *env, int __con, int *f_arr)
{
	char	*new_var;

	if (var == NULL || var[0] == '\0' || (var[0] == '$'
			&& (var[1] == '"' || var[1] == '\'')))
		return (ft_strdup(""));
	if (var[0] == '$' && var[1] == '$')
		return (ft_strdup(ft_itoa(getpid())));
	while (var[f_arr[0]])
	{
		if (var[f_arr[0]] == 39)
			skip_single_quotes(var, f_arr);
		if (var[f_arr[0]] == 34)
			f_arr[2]++;
		if (is_valid_variable_start(var, f_arr[0], 1)
			&& ((!(f_arr[2] % 2) && __con) || (f_arr[2] % 2 && !__con)))
		{
			new_var = expand_variable_in_string(var, env, f_arr[0], &f_arr[0]);
			if (new_var && new_var[0] == '\0')
				return (ft_strdup(""));
			return (recursively_expand_variables(new_var, env, __con, f_arr));
		}
		f_arr[0]++;
	}
	return (var);
}

char	**refactore_args_array(char **args, int *quick_norm_fix)
{
	int						a;
	int						b;
	int						c;
	char					**new_args;

	a = 0;
	b = 0;
	*quick_norm_fix = 0;
	c = count_strings_in_array(args);
	new_args = malloc((detected_errors(args) + c + 1) * sizeof(char **));
	if (!new_args)
		return (NULL);
	while (args[a])
	{
		c = is_flawed_str(args[a], 0, 0, 0);
		if (c)
			b = simplified_refactor(new_args, b, args[a], 0);
		else
			new_args[b++] = strcopy(args[a]);
		a++;
	}
	new_args[b] = 0;
	free_string_array(args);
	return (new_args);
}

void	expand_variables_in_ast(t_ast_node *head, t_env *env)
{
	int							a;
	int							f_arr[3];

	if (head->file_type != FILE_READY && head->args
		&& head->file_type != READ_FROM_APPEND)
	{
		a = -1;
		while (head->args[++a])
			((ft_memset(f_arr, 0, 3 * sizeof(int))),
				(head->args[a] = recursively_expand_variables(
						head->args[a], env, 1, f_arr)));
		head->args = refactore_args_array(head->args, &a);
		while (head->args[a])
		{
			ft_memset(f_arr, 0, 3 * sizeof(int));
			head->args[a] = recursively_expand_variables(
					head->args[a], env, 0, f_arr);
			head->args[a] = remove_quotes_from_str(head->args[a], 0, 0, 0);
			a++;
		}
	}
	if (head->left)
		expand_variables_in_ast(head->left, env);
	if (head->right)
		expand_variables_in_ast(head->right, env);
}
