CC=gcc
CFLAGS=-g -std=c11 -Wall -Wextra

NAME=ndtm

$(NAME): $(NAME).c
	$(CC) $(CFLAGS) -o $@ $<
