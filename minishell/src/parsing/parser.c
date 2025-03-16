/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ana-lda- <ana-lda-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:06:18 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/16 21:54:41 by ana-lda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Parses a command from the token list and creates an AST node for it.
 * @param tokens The token list to parse the command from.
 * @return A pointer to the newly created command AST node, or NULL if memory allocation fails.
 */
t_ast_node	*parse_command(t_token **tokens)
{
	t_ast_node		*command_node;
	int				arg_count;

	command_node = new_ast_node(T_WORD);
	arg_count = count_command_arguments(*tokens);
	command_node->args = malloc(sizeof(char *) * (arg_count + 1));
	if (!command_node->args)
		return (NULL);
	fill_command_arguments(command_node, tokens, arg_count);
	return (command_node);
}

/**
 * @brief Creates a file AST node from a given token.
 * @param token The token representing the file.
 * @return A pointer to the newly created file AST node, or NULL if memory allocation fails.
 */
t_ast_node	*create_file_node(t_token *token)
{
	t_ast_node			*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = token->type;
	node->args = malloc(sizeof(char *) * 2);
	if (!node->args)
	{
		free(node);
		return (NULL);
	}
	node->args[0] = token->value;
	node->args[1] = NULL;
	node->left = NULL;
	node->right = NULL;
	free(token);
	return (node);
}

/**
 * @brief Recursively parses redirections from the token list and creates corresponding AST nodes.
 * @param tokens The token list to parse redirections from.
 * @return A pointer to the redirection AST node, or NULL if no valid redirection is found.
 */
t_ast_node	*parse_redirection(t_token **tokens)
{
	t_token		*tmp;
	t_ast_node	*redirect_node;
	t_token		*next_token;

	if (!*tokens)
		return (NULL);
	tmp = *tokens;
	if ((*tokens)->type >= T_REDIR_IN
		&& (*tokens)->type <= T_REDIR_HEREDOC)
		return (create_and_link_redirection(tokens, tmp));
	while (*tokens && (*tokens)->next)
	{
		next_token = (*tokens)->next;
		if ((*tokens)->next->type >= T_REDIR_IN
			&& (*tokens)->next->type <= T_REDIR_HEREDOC)
		{
			redirect_node = new_ast_node((*tokens)->next->type);
			(*tokens)->next = next_token->next->next;
			redirect_node->left = parse_redirection(&tmp);
			redirect_node->right = create_file_node((next_token->next));
			return (free(next_token->value), free(next_token), redirect_node);
		}
		*tokens = next_token;
	}
	return (parse_command(&tmp));
}

/**
 * @brief Recursively parses a pipeline of commands from the token list and creates corresponding AST nodes.
 * @param tokens The token list to parse the pipeline from.
 * @return A pointer to the pipeline AST node, or NULL if no valid pipeline is found.
 */
t_ast_node	*parse_pipeline(t_token **tokens)
{
	t_token		*tmp;
	t_token		*next_token;
	t_ast_node	*pipe_node;

	tmp = *tokens;
	while (*tokens && (*tokens)->next)
	{
		next_token = (*tokens)->next;
		if ((*tokens)->next->type == T_PIPE)
		{
			pipe_node = new_ast_node((*tokens)->next->type);
			(*tokens)->next = NULL;
			pipe_node->left = parse_redirection(&tmp);
			pipe_node->right = parse_pipeline(&(next_token->next));
			free(next_token->value);
			free(next_token);
			return (pipe_node);
		}
		*tokens = next_token;
	}
	return (parse_redirection(&tmp));
}


/**
 * @brief Parses the entire token list and creates the corresponding abstract syntax tree (AST).
 * @param tokens The token list to parse.
 * @return A pointer to the root AST node, or NULL if no valid AST could be created.
 */
t_ast_node	*parse_tokens(t_token **tokens)
{
	if (!tokens || !*tokens)
		return (NULL);
	return (parse_pipeline(tokens));
}
