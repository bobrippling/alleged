CXXFLAGS += ${CFLAGS}
LDFLAGS  += $(shell allegro-config --libs) -lm

BIN       = test

${BIN}: main.o phyobj.o netobj.o jet.o vector.o
	$Qecho LD $@
	$Q${CXX} ${LDFLAGS} -o $@ $^

clean: mostlyclean
	$Qrm -f ${BIN}

.PHONY: clean

include ../config.mk
