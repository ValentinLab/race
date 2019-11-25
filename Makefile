CFLAGS = -Wall -g -Iinclude -std=c99

SHARED_OBJ = shared.o
TARGETS = cugnot

all: libshared.a $(TARGETS)

libshared.a : $(SHARED_OBJ)
	ar cr $@ $(SHARED_OBJ)

#tesla: tesla.o libshared.a
#	cc -o $@ tesla.o -L. -lshared -lpthread

#slow: slow.o libshared.a
#	cc -o $@ slow.o -L. -lshared -lpthread

cugnot: cugnot.o libshared.a
	cc -o $@ cugnot.o -L. -lshared -lpthread

clean:
	rm -f *.o

mrproper: clean
	rm -f libshared.a
	rm -f $(TARGETS)
