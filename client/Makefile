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

bullet.o: bullet.cpp phyobj.h bullet.h
jet.o: jet.cpp phyobj.h netobj.h time.h vector.h bullet.h jet.h
main.o: main.cpp netobj.h phyobj.h bullet.h jet.h
netobj.o: netobj.cpp netobj.h
phyobj.o: phyobj.cpp phyobj.h vector.h
vector.o: vector.cpp vector.h
