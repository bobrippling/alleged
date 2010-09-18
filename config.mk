CC      += gcc
CFLAGS  += -g -Wall -Wextra -pedantic
LD      += gcc
LDFLAGS += -g

Q       = @

.PHONY: mostlyclean

.c.o:
	@echo CC $@
	$Q${CC} ${CFLAGS} -c -o $@ $<

.cpp.o:
	$Qecho CPP $@
	$Q${CXX} ${CXXFLAGS} -c -o $@ $<

mostlyclean:
	$Qrm -f *.o
