/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:03:55 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/21 12:10:45 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks for common syntax errors in the input string.
 * @param input The input string to check for syntax errors.
 * @return 1 if there is a syntax error, 0 otherwise.
 */
int	check_syntax_errors(const char *input)
{
	if (has_unclosed_quotes(input))
	{
		ft_putstr_fd("Syntax error: unclosed quote\n", STDERR_FILENO);
		return (1);
	}
	if (has_invalid_redirections(input))
	{
		ft_putstr_fd("Syntax error: invalid redirection\n", STDERR_FILENO);
		return (1);
	}
	if (has_misplaced_operators(input))
	{
		ft_putstr_fd("Syntax error: misplaced operator\n", STDERR_FILENO);
		return (1);
	}
	if (has_logical_operators(input))
	{
		ft_putstr_fd("Error: Logical operators '&&' and '||' \
			are not supported.\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

/**
 * @brief Checks if there are unclosed quotes in the input string.
 * @param input The input string to check for unclosed quotes.
 * @return 1 if there are unclosed quotes, 0 otherwise.
 */
int	has_unclosed_quotes(const char *input)
{
	char	quote_type;

	quote_type = 0;
	while (*input)
	{
		if (*input == '\'' || *input == '\"')
		{
			if (quote_type == *input)
				quote_type = 0;
			else if (!quote_type)
				quote_type = *input;
		}
		input++;
	}
	return (quote_type != 0);
}

/**
 * @brief Checks for invalid redirections in the input string.
 * @param input The input string to check for invalid redirections.
 * @return 1 if there are invalid redirections, 0 otherwise.
 */
int	has_invalid_redirections(const char *input)
{
	int	s_q_count;
	int	d_q_count;

	s_q_count = 0;
	d_q_count = 0;
	while (*input)
	{
		update_quote_counts(*input, &s_q_count, &d_q_count);
		if ((!(s_q_count % 2) && !(d_q_count % 2))
			&& (*input == '>' || *input == '<'))
		{
			if (is_invalid_operator(&input))
				return (1);
		}
		else
			input++;
	}
	return (0);
}

/**
 * @brief Checks for misplaced operators in the input string.
 * @param input The input string to check for misplaced operators.
 * @return 1 if there are misplaced operators, 0 otherwise.
 */
int	has_misplaced_operators(const char *input)
{
	int	expect_command_next;
	int	s_q_count;
	int	d_q_count;

	s_q_count = 0;
	d_q_count = 0;
	expect_command_next = 0;
	if (*input == '|' || *input == '&')
		return (1);
	while (*input)
	{
		update_quote_counts(*input, &s_q_count, &d_q_count);
		if (*input == '|' && !(s_q_count % 2) && !(d_q_count % 2))
		{
			if (expect_command_next)
				return (1);
			expect_command_next = 1;
		}
		else if (!ft_isspace(*input))
			expect_command_next = 0;
		input++;
	}
	if (expect_command_next)
		return (1);
	return (0);
}

/**
 * @brief Checks for logical operators (&&, ||) in the input string.
 * @param input The input string to check for logical operators.
 * @return 1 if logical operators are found, 0 otherwise.
 */
int	has_logical_operators(const char *input)
{
	int							s_q_count;
	int							d_q_count;

	s_q_count = 0;
	d_q_count = 0;
	while (*input)
	{
		update_quote_counts(*input, &s_q_count, &d_q_count);
		if (!(d_q_count % 2) && !(s_q_count % 2)
			&& ((*input == '&' && *(input + 1) == '&')
				|| (*input == '|' && *(input + 1) == '|')))
			return (1);
		input++;
	}
	return (0);
}
