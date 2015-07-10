CFLAGS = -Wall -Werror -g
SOURCES = $(wildcard *.c)
PROGRAMS = $(SOURCES:.c=.out)

all: $(PROGRAMS)

%.out: %.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	find . -name '*.out' -delete & \
	find . -name '*.a' -delete & \
	find . -name '*.o' -delete
