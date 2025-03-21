/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:06:07 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/21 12:13:15 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Handles special characters (e.g., redirection, pipe) in the input
 *  string and creates corresponding tokens.
 * @param input A pointer to the input string to be processed.
 * @param tokens A pointer to the head of the token list.
 * @details Detects special characters like '>', '<', and '|' and creates 
 * corresponding redirection or pipe tokens.
 *  Updates the `input` pointer to skip over the processed characters.*/
void	handle_special_chars(char **input, t_token **tokens)
{
	if (**input == '>')
	{
		if (*(*input + 1) == '>')
		{
			add_token_to_list(tokens, create_new_token(T_REDIR_APPEND, ">>"));
			(*input)++;
		}
		else
			add_token_to_list(tokens, create_new_token(T_REDIR_OUT, ">"));
	}
	else if (**input == '<')
	{
		if (*(*input + 1) == '<')
		{
			add_token_to_list(tokens, create_new_token(T_REDIR_HEREDOC, "<<"));
			(*input)++;
		}
		else
			add_token_to_list(tokens, create_new_token(T_REDIR_IN, "<"));
	}
	else if (**input == '|')
		add_token_to_list(tokens, create_new_token(T_PIPE, "|"));
	(*input)++;
}

/** @brief Processes a word from the input string and adds it as a token 
 * to the token list.
 * @param input A pointer to the input string to be processed.
 * @param tokens A pointer to the head of the token list.
 * @details Extracts a word (continuous sequence of non-space, 
 * non-special characters) from the input and creates
 *  a T_WORD token. Updates the `input` pointer to skip over
 *  the processed word.*/
void	handle_word(char **input, t_token **tokens)
{
	char	*start;
	int		in_quote;
	char	quote_char;

	start = *input;
	in_quote = 0;
	quote_char = '\0';
	while (**input)
	{
		update_quote_status(**input, &in_quote, &quote_char);
		if (!in_quote && ft_strchr(" \t\n><|", **input))
			break ;
		(*input)++;
	}
	add_word_token_if_valid(&start, input, tokens);
}

/** @brief Tokenizes an input string by splitting it into tokens 
 * based on spaces, special characters, and quotes.
 * @param input The input string to be tokenized.
 * @return A pointer to the head of the list of tokens.
 * @details Iterates through the input string, handling special
 *  characters and words, and creates corresponding tokens
 * for each part of the input. The function returns the tokenized list.*/
t_token	*tokenize_input(char *input)
{
	t_token	*tokens;

	tokens = NULL;
	while (*input)
	{
		while (*input && ft_strchr(" \t\n", *input))
			input++;
		if (ft_strchr("><|", *input))
			handle_special_chars(&input, &tokens);
		else
			handle_word(&input, &tokens);
	}
	return (tokens);
}
