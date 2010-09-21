CXXFLAGS += ${CFLAGS}
LDFLAGS  += $(shell allegro-config --libs) -lm

BIN       = test

${BIN}: main.o phyobj.o netobj.o vector.o  jet.o bullet.o
	$Qecho LD $@
	$Q${CXX} ${LDFLAGS} -o $@ $^

clean: mostlyclean
	$Qrm -f ${BIN}

.PHONY: clean

include ../config.mk

jet.o: jet.cpp phyobj.h netobj.h vector.h jet.h
main.o: main.cpp netobj.h phyobj.h jet.h
netobj.o: netobj.cpp netobj.h
phyobj.o: phyobj.cpp phyobj.h vector.h
vector.o: vector.cpp vector.h
