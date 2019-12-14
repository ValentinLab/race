CFLAGS = -Wall -g -Iinclude -std=c99

LIB_DIR = lib

SHARED_OBJ = shared.o
TARGETS = cugnot multipla trottinette

all: libshared.a $(TARGETS)

libshared.a : $(LIB_DIR)/$(SHARED_OBJ)
	ar cr $@ $(LIB_DIR)/$(SHARED_OBJ)

#tesla: $(LIB_DIR)/tesla.o libshared.a
#	cc -o $@ tesla.o -L. -lshared -lpthread

cugnot: $(LIB_DIR)/cugnot.o libshared.a
	cc -o $@ $(LIB_DIR)/cugnot.o -L. -lshared -lpthread

multipla: $(LIB_DIR)/multipla.o libshared.a
	cc -o $@ $(LIB_DIR)/multipla.o -L. -lshared -lpthread

trottinette: $(LIB_DIR)/trottinette.o libshared.a
	cc -o $@ $(LIB_DIR)/trottinette.o -L. -lshared -lpthread

clean:
	rm -f $(LIB_DIR)/*.o
	rm -f *.o
	rm -f *.log*

mrproper: clean
	rm -f libshared.a
	rm -f $(TARGETS)
