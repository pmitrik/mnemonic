CC = gcc
INCLUDES = -I.
CFLAGS = -Wall -g
DEPS = 
OBJS = example.o
MAIN = ex
LFLAGS =
LIBS =

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

all:	$(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *~ core $(MAIN)

