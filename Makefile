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
	rm -f scripts_sources/*.sh.x.c

mrproper: clean
	rm -f libshared.a
	rm -f $(TARGETS)
	rm -f $(SCRIPTS)

ARCHIVE_DIR = nhoun_vperignon

archive: mrproper
	test "$(LIB_DIR)" = "lib"
	rm -rf $(ARCHIVE_DIR)
	mkdir $(ARCHIVE_DIR)
	cp -r lib Makefile race-server race-viewer $(ARCHIVE_DIR)
	cp rapport/*.png ./
	pandoc -f markdown -S -s -N -Vsecnumdepth=4 -t latex -V geometry:margin=3.5cm -V lang=fr rapport/rapport.md -o rapport.pdf
	rm -f *.png
	mv rapport.pdf $(ARCHIVE_DIR) 
	tar zcvf $(ARCHIVE_DIR).tar.gz $(ARCHIVE_DIR)
	rm -rf $(ARCHIVE_DIR)

