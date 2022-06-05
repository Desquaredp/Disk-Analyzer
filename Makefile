# Output binary name
bin=da
lib=libda.so
list_lib=libelist.so

# Set the following to '0' to disable log messages:
LOGGER ?=0

# Compiler/linker flags
CFLAGS += -g -Wall -fPIC -DLOGGER=$(LOGGER)
LDLIBS += -lm
LDFLAGS += -L. -Wl,-rpath='$$ORIGIN'

all: $(bin) $(lib) $(list_lib)

$(bin): da.o elist.o util.o
	$(CC) $(CFLAGS) $(LDLIBS) $(LDFLAGS) $^ -o $@

$(lib): da.o elist.o util.o
	$(CC) $(CFLAGS) $(LDLIBS) $(LDFLAGS) $^ -shared -o $@

$(list_lib): elist.o
	$(CC) $(CFLAGS) $(LDLIBS) $(LDFLAGS) $^ -shared -o $@

docs: Doxyfile
	doxygen

clean:
	rm -f $(bin) $(lib) $(list_lib) elist.o da.o util.o
	rm -rf docs

# Individual dependencies --
da.o: da.c logger.h util.h elist.h
elist.o: elist.c elist.h logger.h
util.o: util.c util.h logger.h


