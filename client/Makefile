CXXFLAGS += -g -Wall
LDFLAGS  += -g $(shell allegro-config --libs) -lm
Q         = @

BIN       = test
TMPDIR    = alleg

.PHONY: clean mostlyclean dist


${BIN}: main.o phyobj.o netobj.o jet.o
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
	@cp -R \
			jet_24.bmp \
			jet.cpp \
			jet.h \
			main.cpp \
			Makefile \
			netobj.cpp \
			netobj.h \
			phyobj.cpp \
			phyobj.h \
			svr.c \
			${TMPDIR}
	@rm -f ${TMPDIR}.tar.bz2
	@tar cf ${TMPDIR}.tar ${TMPDIR}
	@rm -r ${TMPDIR}
	@bzip2 ${TMPDIR}.tar

jet.o: jet.cpp phyobj.h netobj.h jet.h
main.o: main.cpp netobj.h phyobj.h jet.h
netobj.o: netobj.cpp netobj.h
phyobj.o: phyobj.cpp phyobj.h
