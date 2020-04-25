PROG=proj2

FILES=proj2.c process_gen.c process_gen.h components.c components.h

CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic -pthread

all:$(PROG)

$(PROG): $(FILES)
	$(CC) $(CFLAGS) -g -o $(PROG) $(FILES)