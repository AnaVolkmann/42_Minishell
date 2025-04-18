<div align=center>
	<h1>
		<i>Minishell</i>
	</h1>
	<img src=https://github.com/AnaVolkmann/42_project_badges/blob/main/badges/minishelle.png alt=minishell_badge/>
	<p align=center>
        <p align="center">
	<b><i>As beautifull as a shell</i></b><br>
  
---

<h2>
		Final score
	</h2>
	<img src=https://github.com/AnaVolkmann/AnaVolkmann/blob/main/minishell_score.png alt=minishell_grade/>

---

</div>

### Team Members

- [Ana Laura Volkmann](https://github.com/AnaVolkmann)
- [Lucas Graça](https://github.com/gracaflucas)

## Overview

Minishell is a simple shell program built in C, inspired by [Bash](https://en.wikipedia.org/wiki/Bash_(Unix_shell)). It was developed as part of the **42 School** curriculum to give us hands-on experience in **Unix process management**, **file handling**, and **command execution**. This project enabled us to delve deeply into the inner workings of a Unix shell, learning how to manage processes, environment variables, and handle command execution.

Minishell pushes you to think critically and creatively, offering practical challenges like building parsers, managing redirection, and implementing signals, while also helping you to develop strong teamwork skills.

> [!NOTE]  
> In accordance with the 42 School coding norms:  
> * Each function must not exceed 25 lines of code.  
> * Every variable should be declared at the top of the function and initialized on a separate line.  
> * Only allowed functions may be used—using others is considered cheating.


## Project Challenges

We faced many challenges while building Minishell, including:

- **Parsing User Input**: Handling complex input with spaces, quotes, and special characters while distinguishing between commands, arguments, and options.
- **Command Execution**: Correctly finding and executing the right command, whether built-in or external, using the `PATH` or direct file paths.
- **Signal Handling**: Ensuring proper behavior for signals like `SIGINT` (`Ctrl+C`), `SIGQUIT` (`Ctrl+\`), and `EOF` (`Ctrl+D`).
- **Redirection and Pipes**: Supporting input/output redirection (`<`, `>`, `>>`, `<<`) and pipes (`|`), enabling command chaining.
- **Environment Variables**: Managing environment variables and expanding them within commands.
- **Memory Management**: Avoiding memory leaks by properly managing dynamically allocated memory.
- **Built-in Commands**: Implementing internal shell commands (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`).
- **Process Management**: Using system calls like `fork`, `execve`, `wait`, and `pipe` to manage processes.
- **Error Handling**: Gracefully managing errors like invalid commands, permission issues, or missing files.

## External Functions

### Readline Functions

| Function            | Description                                                   |
| ------------------- | ------------------------------------------------------------- |
| `readline`          | Reads a line from standard input.                             |
| `rl_clear_history`  | Clears the readline history list.                            |
| `rl_on_new_line`    | Prepares readline for reading input on a new line.           |
| `rl_replace_line`   | Replaces the content of the readline current line buffer.    |
| `rl_redisplay`      | Updates the display to reflect changes to the input line.    |
| `add_history`       | Adds the most recent input to the readline history list.     |

### Standard I/O Functions

| Function   | Description                                             |
| ---------- | ------------------------------------------------------- |
| `printf`   | Outputs formatted data to stdout.                       |

### Memory Allocation Functions

| Function   | Description                                             |
| ---------- | ------------------------------------------------------- |
| `malloc`   | Allocates specified bytes of heap memory.               |
| `free`     | Deallocates previously allocated memory.                |

### File I/O Functions

| Function   | Description                                                   |
| ---------- | ------------------------------------------------------------- |
| `write`    | Writes data to a file descriptor.                             |
| `access`   | Checks calling process's permissions for a file or directory. |
| `open`     | Opens a file or device, returning a file descriptor.         |
| `read`     | Reads data from a file descriptor into a buffer.              |
| `close`    | Closes a previously opened file descriptor.                   |

### Process Control Functions

| Function      | Description                                                        |
| ------------- | ------------------------------------------------------------------ |
| `fork`        | Creates a new process by duplicating the calling process.         |
| `wait`        | Suspends execution until one of its children terminates.          |
| `waitpid`     | Waits for a specific child process to change state.               |
| `signal`      | Handles or ignores signals sent to the process.                   |
| `kill`        | Sends a signal to a process or group of processes.                |
| `exit`        | Terminates the calling process.                                   |

### Directory Functions

| Function   | Description                                             |
| ---------- | ------------------------------------------------------- |
| `getcwd`   | Gets the current working directory.                     |
| `chdir`    | Changes the current working directory.                  |
| `execve`   | Replaces the current process image with a new one.      |

### Error Handling Functions

| Function   | Description                                             |
| ---------- | ------------------------------------------------------- |
| `strerror` | Returns a pointer to the textual representation of an error code. |
| `perror`   | Returns a pointer to the textual representation of an error code. |

## Parsing and Input Management

We implemented a custom parser using the `readline` library to read user input, tokenize the commands, and check for syntax errors. The input was broken down into commands, arguments, and special operators like redirection and pipes. Each token was analyzed and processed to build an **Abstract Syntax Tree (AST)** representing the user's command structure.

### Tokenization and AST Construction

The process of tokenization involves breaking the input into meaningful units (tokens), such as:

- **Words**: Represent commands or arguments.
- **Redirection**: Input (`<`), output (`>`), append (`>>`), here-doc (`<<`).
- **Pipe**: (`|`).
- **Environment Variables**: `$` followed by variable names.

After tokenization, the input is parsed into an AST that reflects the execution flow of the commands, handling redirection, pipes, and environment variable expansions.

### Error Checking

Our syntax error checker identifies unclosed quotes, misplaced operators (like consecutive pipes), and invalid redirections, ensuring the shell correctly handles invalid input.

## Command Execution

We implemented logic to execute both **built-in commands** (like `echo`, `cd`, `pwd`, etc.) and **external commands**. When a built-in command is executed, we directly invoke the relevant function, while for external commands, we search for the executable using the `PATH` environment variable and execute it with system calls like `fork`, `execve`, and `wait`.

### Built-in Commands

- **echo**: Prints the arguments passed to it.
- **cd**: Changes the current working directory.
- **pwd**: Prints the current working directory.
- **export**: Sets environment variables.
- **unset**: Unsets environment variables.
- **env**: Prints the environment variables.
- **exit**: Exits the shell.

### External Commands

For external commands, we used `execve` to execute the command, managing redirections and pipes as needed.

### Redirection and Pipes

We implemented support for input/output redirection (`<`, `>`, `>>`, `<<`) and pipes (`|`), allowing for seamless chaining of commands and data flow.

## Signal Handling

Minishell supports signal handling for commonly used signals such as:

- **SIGINT**: Triggered by `Ctrl+C`.
- **SIGQUIT**: Triggered by `Ctrl+\`.
- **EOF**: Triggered by `Ctrl+D` to exit the shell.

By handling these signals properly, the shell behaves similarly to Bash, allowing for better user interaction and control.
