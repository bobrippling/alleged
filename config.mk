CC      = gcc
CFLAGS  = -g -Wall -Wextra -pedantic
LD      = gcc
LDFLAGS = -g

Q       = @

.c.o:
	@echo CC $@
	$Q${CC} ${CFLAGS} -c -o $@ $<

mostlyclean:
	$Qrm -f *.o
