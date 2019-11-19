CFLAGS = -Wall -g -std=c99

TARGETS = tesla

all: $(TARGETS)

ferrari: tesla.o

clean:
	rm -f *.o

mrproper: clean
	rm $(TARGETS)
