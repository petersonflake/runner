#ifndef MAIN_H
#define MAIN_H
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <getopt.h>
#include <ftw.h>
#include "scripts.tab.h"
#include "command.h"
#define FDS 15
void print_commands(command_stack *stack, int allflag);

void print_str_stack(string_stack *stack);

void print_arg_stack(argstack *args);

void exec_all_cmds(command_stack *stack);

void mark_cmd(command_stack *stack, char *id);

void show_help();

void dry_run(command_stack *stack);

void print_completions(command_stack *stack);

int set_data_dir(char *buffer);

int is_valid_datadir(char *path);

struct FTW;

int parse_file(const char *filepath, const struct stat *info,
        int typeflag, struct FTW *pathinfo);

#endif
