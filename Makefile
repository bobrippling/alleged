CXXFLAGS += -g -Wall
LDFLAGS  += -g $(shell allegro-config --libs) -lm
Q         = @

BIN       = test
TMPDIR    = alleg

.PHONY: clean mostlyclean dist


${BIN}: main.o phyobj.o ball.o
	$Qecho LD $@
	$Q${CXX} ${LDFLAGS} -o $@ $^

.cpp.o:
	$Qecho CPP $@
	$Q${CXX} ${CXXFLAGS} -c -o $@ $<

clean: mostlyclean
	$Qrm -f ${BIN}

mostlyclean:
	$Qrm -f *.o

dist:
	@mkdir -p ${TMPDIR}
	@cp ball_24.bmp ball.cpp ball.h main.cpp \
		Makefile TODO ${TMPDIR}
	@rm -f ${TMPDIR}.tar.bz2
	@tar cf ${TMPDIR}.tar ${TMPDIR}
	@rm -r ${TMPDIR}
	@bzip2 ${TMPDIR}.tar
