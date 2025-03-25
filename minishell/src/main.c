/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufiguei <lufiguei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:51:55 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/25 12:19:04 by lufiguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Processes the input string and tokenizes it.
 * @param input The raw input string.
 * @return A list of tokens generated from the input,
 * or NULL if there was a syntax error.
 * @details Trims whitespace characters from the input,
 *checks for syntax errors, 
 *and then tokenizes the cleaned input. Frees the original input string 
 *after processing.*/
t_token	*process_and_tokenize_input(char *input)
{
	char		*trimmed_input;
	t_token		*tokens;

	trimmed_input = ft_strtrim(input, " \t\n\v\f\r");
	free(input);
	if (!trimmed_input)
		return (NULL);
	if (check_syntax_errors(trimmed_input))
	{
		free(trimmed_input);
		return (NULL);
	}
	tokens = tokenize_input(trimmed_input);
	free(trimmed_input);
	return (tokens);
}

/** @brief Main loop for running the minishell program.
 * @param env The environment variables for the shell.
 * @details Prompts the user for input, tokenizes the
 *input, parses it into an AST,
 *executes the command, and updates the environment status.
  Repeats until the user exits the shell.*/
void	run_minishell(t_env *env)
{
	char		*line;
	int			status;
	t_token		*tokens;
	t_ast_node	*ast;

	while (1)
	{
		status = 0;
		line = readline("minishell: ");
		if (!line)
			break ;
		if (check_line(&line))
			continue ;
		add_history(line);
		tokens = process_and_tokenize_input(line);
		if (!tokens)
			status = 2;
		if (!status)
		{
			ast = parse_tokens(&tokens);
			command_executer(ast, env, &status);
			free_ast_child(ast);
		}
		update_env_status(env, status, "?=");
	}
}

/** @brief The entry point for the minishell program.
 * @param argc The number of arguments passed to the program.
 * @param argv The list of arguments passed to the program.
 * @param original_env The original environment variables.
 * @return 0 if the shell started successfully, or an error code.
 * @details Initializes the shell environment, sets up signal
 *handlers, and starts the main minishell loop if the shell
 * is running interactively.*/
int	main(int argc, char **argv, char **original_env)
{
	t_env	*env;

	(void)argv;
	setup_signal_handlers();
	env = malloc(sizeof(t_env));
	if (!isatty(1) || !isatty(0))
		return (free(env), 0);
	if (argc == 1 && initialize_shell_with_environment(env, original_env))
	{
		run_minishell(env);
		cleanup_and_exit_shell(env, 0);
	}
	free(env);
	return (0);
}
