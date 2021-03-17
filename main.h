#ifndef MAIN_H
#define MAIN_H
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <getopt.h>
#include <ftw.h>
#include "scripts.tab.h"
#include "command.h"
#define FDS 15 /* Number of file descriptors for nftw */

/*
 * Print details about commands.  If allflag is 0, only marked
 * commands are displayed.  Otherwise, show all of them.
 */
void print_commands(command_stack *stack, int allflag);

/*
 * Print numbered list of strings on string stack.
 */
void print_str_stack(string_stack *stack);

/*
 * Print all elements of the argstack, which holds
 * an array of string stacks.
 */
void print_arg_stack(argstack *args);

/*
 * Execute all marked commands.
 */
void exec_all_cmds(command_stack *stack);

/*
 * Mark command for show or execution.
 */
void mark_cmd(command_stack *stack, char *id);

void show_help();

/*
 * Show the commands that would be executed with there arguments.
 */
void dry_run(command_stack *stack);

/*
 * Print all ids, for bash tab completion.
 */
void print_completions(command_stack *stack);

/*
 * Set the data directory, where we look for files and the scripts directory.
 */
int set_data_dir(char *buffer);

/*
 * Check if path is a directory and is accessible for reading.
 */
int is_valid_datadir(char *path);

struct FTW;

/* This function is passed to nftw.  It checks the file, and tries to read it. */
int parse_file(const char *filepath, const struct stat *info,
        int typeflag, struct FTW *pathinfo);

#endif
