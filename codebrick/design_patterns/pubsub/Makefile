CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = pubsub_example
SRCS = pubsub_example.c pubsub.c pubsub_platform.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean 
