#include "shell.h"

/**
* control_c - ignore Ctrl-C and prompts the user again
* @number: inputs integer from signal
*/
void control_c(int number)
{
(void)number;
write(STDOUT_FILENO, "\n$ ", 3);
}

/**
* built_in - handles builtins (exit, env, cd)
* @env: environmental variable 
* @num: nth user command to handle error messages
* @commandtoken: user's typed command
* @command: free the command
* Return: 1 if builtin executed, 0 if builtin not executed
* See previous projects for string compare functions i.e _strcmp()
*/
int built_in(char **commandtoken, list_t *env, int num, char **command)
{
int i = 0;

/* if input entered is "exit", free the command tokens or user command, and exit/quit shell */
/* See previous projects for string compare functions i.e _strcmp() */
if (_strcmp(commandtoken[0], "exit") == 0)
{
i = __exit(commandtoken, env, num, command);
}
/* if input entered is "env", print then free the command tokens or user command, and reprompt for input again */
/* See previous projects for string compare functions i.e _strcmp() */
else if (_strcmp(commandtoken[0], "env") == 0)
{
_env(commandtoken, env);
i = 1;
}
/* if input entered is "cd" , shell change directory */
/* See previous projects for string compare functions i.e _strcmp() */
/* See man getcwd() for expansion on working directories */
else if (_strcmp(commandtoken[0], "cd") == 0)
{
i = _cd(commandtoken, env, num);
}
/* if input entered is "setenv", create or modify linked list node */
/* See previous projects for string compare functions i.e _strcmp() */
/* See previous projects for linked list nodes & and setting enviromental variables*/
else if (_strcmp(commandtoken[0], "setenv") == 0)
{
_setenv(&env, commandtoken);
i = 1;
}
/* if input entered is "setenv", create or modify linked list node */
/* See previous projects for string compare functions i.e _strcmp() */
/* See previous projects for linked list nodes & and setting enviromental variables*/
else if (_strcmp(commandtoken[0], "unsetenv") == 0)
{
_unsetenv(&env, commandtoken);
i = 1;
}
return (i);
}

/**
* ignore_space - function to return string without spaces. i.e whole input as one string
* @str: string
* Return: new string
*/
char *ignore_space(char *str)
{
while (*str == ' ')
str++;
return (str);
}

/**
* control_D - exits program if Ctrl-D is entered
* @input: characters read @ get_line
* @command: user's entered command
* @env: environmental variable linked list
* see env in notes to expand on environmental variables
*/
void control_D(int input, char *command, list_t *env)
{
if (input == 0) /* for handling Ctrl+D keyboard shortcut*/
{
free(command); /* quit/exit shell with new line*/
free_linked_list(env);
if (isatty(STDIN_FILENO))/* ctrl+d prints newline */
write(STDOUT_FILENO, "\n", 1);
exit(0);
}
}

/**
* prompt - continously prompts user and executes user's commands set
* @envv: envrionmental variables
* Return: 0 on success
*/
int prompt(char **envv)
{
list_t *env;
size_t i = 0, n = 0;
int command_line_no = 0, exit_stat = 0;
char *command, *n_command, **token;

env = env_linked_list(envv);
do {
command_line_no++;
if (isatty(STDIN_FILENO)) /* reprompt if shell not interactive */
write(STDOUT_FILENO, "$ ", 2);
else
non_interactive(env);
signal(SIGINT, control_c); /* makes ctrl+c not work */
command = NULL; i = 0; /* reset variables every single time the propmtloop runs */
i = get_line(&command); /* read user's command entered in stdin */
control_D(i, command, env); /* wquits/exits shell when user enters ctrl-D */
n_command = command;
command = ignore_space(command);
n = 0;
while (command[n] != '\n') /* replace get_line's \n with \0 */
n++;
command[n] = '\0';
if (command[0] == '\0') /* reprompt if user enters empty input or hits enter only */
{
free(n_command);
continue;
}
token = NULL; token = _str_tok(command, " ");
if (n_command != NULL)
free(n_command);
exit_stat = built_in(token, env, command_line_no, NULL);
if (exit_stat)
continue;
exit_stat = _execve(token, env, command_line_no);
} while (1); /* repeats forver until user exits shell */
return (exit_stat);
}
