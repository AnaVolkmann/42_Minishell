/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:06:03 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/21 12:13:54 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Creates a new token with a specified type and value.
 * @param type The type of the token (e.g., T_WORD, T_OPERATOR).
 * @param value The string value associated with the token.
 * @return A pointer to the newly created token, or NULL if memory
 *  allocation fails.*/
t_token	*create_new_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->next = NULL;
	return (token);
}

/** @brief Adds a new token to the end of the token list.
 * @param tokens A pointer to the head of the token list.
 * @param new_token The new token to be added.
 * @details Appends the new token to the end of the list if
 *  it exists, otherwise sets the new token as the head.*/
void	add_token_to_list(t_token **tokens, t_token *new_token)
{
	t_token	*last;

	if (!*tokens)
		*tokens = new_token;
	else
	{
		last = *tokens;
		while (last->next)
			last = last->next;
		last->next = new_token;
	}
}

/** @brief Frees all tokens in the linked list.
 * @param tokens The head of the token list.
 * @details Iterates through the token list and frees 
 * each token's value and the token itself.*/
void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

/** @brief Updates the status of quote handling during tokenization.
 * @param c The current character being processed.
 * @param in_quote A flag indicating if a quote is currently open
 *  (1 if open, 0 if closed).
 * @param quote_char The character used for quoting (either single 
 * or double quote).
 * @details If a quote is encountered, it either opens or closes the
 *  quote status.*/
void	update_quote_status(char c, int *in_quote, char *quote_char)
{
	if (!*in_quote && (c == '\'' || c == '\"'))
	{
		*in_quote = 1;
		*quote_char = c;
	}
	else if (*in_quote && c == *quote_char)
		*in_quote = 0;
}

/** @brief Adds a word token to the token list if a valid word is found.
 * @param start A pointer to the start of the word.
 * @param input A pointer to the current position in the input string.
 * @param tokens A pointer to the head of the token list.
 * @details If a word is detected between `start` and `input`, a new word 
 * token is created and added to the list.*/
void	add_word_token_if_valid(char **start, char **input, t_token **tokens)
{
	char	*word;

	if (*input > *start)
	{
		word = ft_strndup(*start, *input - *start);
		if (word)
		{
			add_token_to_list(tokens, create_new_token(T_WORD, word));
			free(word);
		}
		else
			ft_putstr_fd("Error: Malloc failed in handle_word.\n", 2);
	}
}
