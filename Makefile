CFLAGS = -Wall -Werror -g
SRC_DIR = ./src

all: tcp-client.out tcp-server.out getip.out

getip.out: $(SRC_DIR)/getip.c
	$(CC) $(CFLAGS) -o $@ $^

tcp-client.out: $(SRC_DIR)/tcp-client.c
	$(CC) $(CFLAGS) -o $@ $^

tcp-server.out: $(SRC_DIR)/tcp-server.c
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $@ $^

.PHONY: clean

clean:
	find . -name '*.out' -delete & \
	find . -name '*.a' -delete & \
	find . -name '*.o' -delete
