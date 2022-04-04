# README

## Objective

The objective of this project is to transmit lines of text in a text file via shared memory from the server to the client and use threads in the client to parse the text.

## Included Files

The project includes the following files:

- makefile
- text-server.c
- text-client.c
- text-memory.c
- README.md

## Makefile

When `make` is executed in the terminal. The make file executes two commands.

`gcc -o text-client text-client.c`

`gcc -o text-server text-server.c`

When `make clean` is executed in the terminal. The make file removes any executable or intermediary build files created by `make`. It is done using the following command in bash.

`rm -rf text-client text-server *.o`

## Server

To run the server a command in the following format must be entered.

`./text-server`

The server will start and display that it started.

Everytime a client connects:

- The server will log the client's request, memory segment opening, filename, file closing, and memory closing.
- If the server is successfully able to open the file, the it will send the text in the file to the client via shared memory.

## Client

A client can be started by entering a command in the following format.

`./text-client dat/dante.txt "the journey"`

The client will start, parse text using four threads, and display the output of the threads.

Everytime a client connects:

- The client will send a filename to the server.
- After the server is able to open the file, the contents of the file are then copied to the shared memory.
- The client uses threads to parse the text.

## Header Files

For this project, `text-memory.h` is created primarily to contain functions to help create and destroy shared memory. It also contains a struct that is used by the client to assist in threading.

- `bool file_checker(char filename[])` checks if the file sent by the client is valid.
- `key_t create_key(char filename[])` creates a key using the filename.
- `int create_segment(char filename[])` creates a meemory segment.
- `char *attach_segment(char filename[])` attaches the memory segment to the client on server.
- `bool detach_segment(char filename[])` detached the memory segment from the process.
- `bool destroy_segment(char filename[])` destroys the memory segment.
- `bool word_checker(char line[], char word[])` takes in a line and a specific string to check if it exists in the line.

Many of the functions listed above are helper functions. Although they are not used explicitly by the client or server, they are used by other functions to assist in shared memory.
