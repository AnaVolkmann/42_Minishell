/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:03:51 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/21 12:10:45 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Updates the counts for single and double quotes.
 * @param c The character being evaluated.
 * @param s_q_count Pointer to the count of single quotes.
 * @param d_q_count Pointer to the count of double quotes.
 */
void	update_quote_counts(char c, int *s_q_count, int *d_q_count)
{
	if (c == 34)
		(*d_q_count)++;
	else if (c == 39)
		(*s_q_count)++;
}

/**
 * @brief Skips over leading spaces and tab characters in a string.
 * @param input The string to process.
 * @return The string after skipping spaces and tabs.
 */
const char	*skip_spaces(const char *input)
{
	while (*input && (*input == ' ' || *input == '\t'))
		input++;
	return (input);
}

/**
 * @brief Checks if the operator in the input is invalid.
 * @param input The input string to check for an invalid operator.
 * @return 1 if the operator is invalid, 0 otherwise.
 */
int	is_invalid_operator(const char **input)
{
	const char	*operator_start;

	operator_start = *input;
	(*input)++;
	if (*operator_start == **input)
		(*input)++;
	*input = skip_spaces(*input);
	if (**input == '\0' || **input == '>'
		|| **input == '<' || **input == '|')
		return (1);
	return (0);
}
