# Runner
> A simple C program to run scripts or programs with predefined arguments as
> defined in files in a specific directory.  The program reads a file passed
> on the command line, or else files in a default location.  Each command is
> identified with an ID, which can be passed on the command line to run that
> command with its set of arguments.

## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Setup](#setup)
* [Features](#features)
* [Status](#status)
* [Contact](#contact)

## General info
The program tries to read files in "$XDG\_CONFIG\_HOME/runner,
"$HOME/.config/runner", and "$HOME/.runner/" in that order.  If none of those
directories are available and a file is not passed on the command line, the
program exits.  Otherwise, it builds an array of commands which are displayed
or executed based on the flags passed.  Each command is identified by an id in
one of the config files, which when passed on the command line marks that
command for either being shown or executed.

## Technologies
* tech 1 - version 

## Setup
Set up

## Code Examples
Config example:
`"Optional documentation string"
echo {
    "hello", "world";
    "how", "are", "you";
} greet;
`

The above defines a command identified as "greet".  If you call `runner -e
greet`, runner will execute `echo hello world` then `echo how are you`.  If
you pass -s, the command and the arguments will be shown, with the docstring,
if present, and the arguments listed.  If you call `runner -d greet`, `echo
hello world` and `echo how are you` will be printed, to show what would be run
if called with the e flag.  If there are multiple commands with the same
identifier, they are executed in the order they are defined, when that
identifier is passed.

All files in the data directory are read, so you can have multiple files there
instead of a single large one, though if there are commands where the order is
important, they should be in the same file.

In addition, if you create a directory labeled scripts in the data directory,
then the program will check there for programs before relying on path.  This
allows you to have a script of the same name as a program, and have it called
when you call runner, and also to use scripts without having to add them to
your PATH.

If there is a syntax error in any of the files, the program will print "Error:
syntax error".

## Features
List of features ready and TODOs for future development
* Call scripts or programs with predefined arguments.

To-do list:
* Variables
* Wow improvement to be done 2

## Status
Project is: _in progress_

## Contact
[mbfmitchell@gmail.com](mbfmitchell@gmail.com)
