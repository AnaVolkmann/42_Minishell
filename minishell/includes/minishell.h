/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufiguei <lufiguei@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:03:36 by ana-lda-          #+#    #+#             */
/*   Updated: 2025/03/26 11:24:38 by lufiguei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define READ_FILE 10
# define READ_FROM_APPEND 15
# define WRITE_FILE 20
# define WRITE_FILE_APPEND 30
# define EXECUTE_FILE 40
# define FILE_READY 50

/*******************INCLUDES****************/
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <errno.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

/*****************GLOBAL VARIABLE*****************/
extern int	g_signal;

/********************TOKEN TYPE*******************/
typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_REDIR_HEREDOC,
	T_ENV_VAR,
}	t_token_type;

/***********************TOKEN STRUCT*****************/
typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
}	t_token;

/***********************AST STRUCT*****************/
typedef struct s_ast_node
{
	t_token_type		type;
	int					file_type;
	char				**args;
	char				**cmd;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	struct s_ast_node	*root;
}	t_ast_node;

/**************************envp*********************/
typedef struct s_env
{
	char				**original_env;
	char				***parsed_env;
}	t_env;

void		ft_exit(char **_cmd_, t_env *env, t_ast_node *head);
int			export_print_or_export(char **_cmd_);

/* ------------------ Main Shell ------------------ */

void		run_minishell(t_env *env);
void		setup_signal_handlers(void);
void		handle_ctrl_c(int sig_num);
void		child_ctrl_c(int sig_num);

/* ------------------ Input Processing ------------------ */

t_token		*tokenize_input(char *input);

/* ------------------ Syntax Handling ------------------ */

int			check_syntax_errors(const char *input);
int			has_unclosed_quotes(const char *input);
int			has_invalid_redirections(const char *input);
int			has_misplaced_operators(const char *input);
int			has_logical_operators(const char *input);
void		update_quote_counts(char c, int *s_q_count, int *d_q_count);
const char	*skip_spaces(const char *input);
int			is_invalid_operator(const char **input);
void		update_quote_status(char c, int *in_quote, char *quote_char);
void		add_word_token_if_valid(char **start, char **input,
				t_token **tokens);

/* ------------------ Token Management ------------------ */

t_token		*create_new_token(t_token_type type, char *value);
void		add_token_to_list(t_token **tokens, t_token *new_token);
void		free_tokens(t_token *tokens);
void		handle_quotes(char **input, t_token **tokens);
void		handle_special_chars(char **input, t_token **tokens);
void		handle_word(char **input, t_token **tokens);
t_token		*tokenize_input(char *input);

/* --------------- AST Construction and Management ----------------- */

t_ast_node	*new_ast_node(t_token_type type);
void		free_ast(t_ast_node *node);
t_ast_node	*parse_tokens(t_token **tokens);
t_ast_node	*parse_command(t_token **tokens);
int			count_command_arguments(t_token *current);
void		fill_command_arguments(t_ast_node *command_node,
				t_token **tokens, int arg_count);
t_ast_node	*parse_pipeline(t_token **tokens);
t_ast_node	*parse_redirection(t_token **tokens);
t_ast_node	*create_file_node(t_token *token);
t_ast_node	*create_and_link_redirection(t_token **tokens, t_token *tmp);

/* ------------------ Built-in Command Execution ------------------ */

int			ft_cd(char **cmd, t_env *env, int *out_fd);
int			ft_echo(char **cmd, int *out_fd);
int			env_or_pwd_cmd(char **cmd, t_env *env, int condition, int *out_fd);
char		**ft_export(char **cmd, t_env *env, int *out_fd, int **status);
char		**unset_or_export_cmd(char **cmd, t_env *env,
				int *out_fd, int *status);

/* --------------------- Built-in Utilities ----------------------- */

void		set_new_pwd_in_env(char *new_path, t_env *env, int unused);
int			change_current_directory(char *path, t_env *env);
char		*get_current_working_directory(int size, int tries, int fd);
int			export_statment_check(char *_cmd_);
void		exec_builtin_and_exit(t_ast_node *head,
				t_env *env, int *_out_fd, int *pipe_data);

/* ------------------ Environment Init ---------------------------- */

int			check_line(char **line);
int			find_env_var_index(t_env *env, char *name);
void		free_environment_variables(char ***array);
void		replace_env_var(char *var, t_env *env);
void		append_env_var(char *var, t_env *env);
int			is_string_numeric(char *s_1);

/* -------------- Printing and Utility Functions ------------------ */

void		print_env_var_to_fd(char *key, char *value, int fd);
void		print_export_vars(char ***arr, int a, int fd);
void		print_export_declaration_to_fd(t_env *env, int *_out_fd);

/* ------------------ Command Execution Management ------------------ */

void		adjust_ast_nodes_for_execution(t_ast_node *head);
void		command_executer(t_ast_node *head,
				t_env *env, int *status);
int			execute_ast_node(t_ast_node *head, int *piped, t_env *env);
int			prepare_and_execute_command(t_ast_node *head, int *fd,
				int *piped, t_env *env);
int			handle_command_redirection(t_ast_node *head, int *piped,
				t_env *env, int *fd);
int			handle_piped_command_execution(t_ast_node *head,
				int *piped, t_env *env, int *fd);
int			exec_here_doc(t_ast_node *limiter, int *piped, t_env *env);
void		initialize_or_reset_pipe_state(int *piped, int flag);
int			open_file_for_redirection(
				t_ast_node *head, int *piped, t_env *env, int status);
int			check_if_command_is_builtin(char *cmd);
int			manage_builtin_execution(t_ast_node *head, int *fd,
				t_env *env, int *piped);
int			manage_single_builtin_execution(t_ast_node *head, int *fd,
				t_env *env, int *piped);
int			execute_builtin_command_in_child(t_ast_node *head,
				t_env *env, int *out_fd, int *pipe_data);

/* ----------- Path and Environment Variable Handling ---------------- */

void		increment_path_index(char *env_var, int *index_start,
				int *index_end);
void		add_env_entry(t_env *env, char *cmd,
				int bool_condition, int condition);
void		remove_env_entry(t_env *env, int index);
void		update_env_status(t_env *env, int status, char *start);
int			is_path_accessible(char *path, int mode);
int			env_just_print(t_env *env, int con, int *_out_fd);

/* ------------------ String Manipulation and Checking --------------- */

void		s_strcopy(char *dest, char *src, int start, int end);
char		*strcopy(char *src);
char		*str_without_char(char *str, char delimiter);
char		*remove_quotes_from_str(char *str, int a, int b, int c);
int			str_cmp(char *str1, char *str2, char *str3);
int			is_space(char *line);
int			sizeof_str(char *str, char delimiter);
int			count_strings_in_array(char **arr);

/* ------------------ Process and Pipeline Management ----------------- */

int			wait_for_children(int status, int *piped);
void		count_redirects_and_pipes(t_ast_node *head, int *piped);
void		close_pipe_ends(int fd_1, int fd_2);
void		child_fds_managment(int *pipe_data, int *_fd, int *fd_);
void		parent_fds_managment(int *pipe_data, int *_fd, int *fd_);

/* ------------------ Variable Expansion and Replacement ------------------ */

void		expand_variables_in_ast(t_ast_node *head, t_env *env);
char		*recursively_expand_variables(
				char *var, t_env *env, int __con, int *f_arr);
char		*replace_variable_with_value(char *old_var, char *new_value,
				int start, int end);
int			simplified_refactor(char **array,
				int index, char *str, int in);
int			is_flawed_str(char *str, int a, int b, int res);
int			detected_errors(char **array);

/* ------------------String and Array Operations ------------------ */

int			string_weight_compare(char *s_1, char *s_2);
int			check_array_arrangment(char ***array, int _si);
char		**merge_command_args(char **first_args, char **additional_args);
char		***duplicate_env_structure(t_env *env, int a, int a_2, int d);
int			count_substrings(char *str, char delimiter);
int			find_substr_index(char **haystack, char *needle, int needle_length);
char		*find_next_substring(char *str, char delimiter, int *index);
int			string_to_int(char *str);
int			is_valid_variable_start(char *str, int index, int check_dollar);
void		free_string_array(char **arr);
char		*get_file_path(char *file, char **envp, char *env_var, int mode);
char		*create_subpath_from_var(char *env_var, char *file,
				int *index_start);
int			is_valid_echo_param(char *s);

/* ----------------------------- Extra ------------------------------ */

int			initialize_shell_with_environment(t_env *env, char **original_env);
void		cleanup_and_exit_shell(t_env *env, int status);
int			get_shell_exit_status(int error_code);
int			count_digits_in_int(int num);
char		**prepare_cmd_arguments(char *cmd, char **envp, int condition);
int			check_command_file_permissions(t_ast_node *head, char **env);
int			handle_file_error_message(char *file, int _status);
void		safe_close(int fd);
void		set_old_pwd_in_env(char *old, t_env *env);
char		*get_env_value(t_env *env, char *key);
char		*handle_edge(char **cmd, t_env *env, int *_out_fd);
void		unset_helper(char **cmd, t_env *env, int *s);
void		exec_error(t_ast_node *head, t_env *env, int flag);
void		set_root(t_ast_node *head);

#endif
