CFLAGS = -Wall -Werror 
SRC_DIR = ./src

main: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -o $@ $^ 
ifneq (,$(findstring r, $(MAKEFLAGS)))
	./$@
endif

%.o: %.c
	$(CC) $(CFLAGS) -c $@ $^

.PHONY: clean

clean:
	find . -name 'main' -delete & \
	find . -name '*.a' -delete & \
	find . -name '*.o' -delete
