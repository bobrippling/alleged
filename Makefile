TMPDIR    = alleg

.PHONY: client server

all: client

client:
	$Qmake -C $@

server:
	$Qmake -C $@

clean:
	make -C client clean
	make -C server clean

dist: clean
	@mkdir -p ${TMPDIR}
	@cp -R \
			client \
			config.h \
			config.mk \
			Makefile \
			README \
			server \
			TODO \
			${TMPDIR}
	@rm -f ${TMPDIR}.tar.bz2
	@tar cf ${TMPDIR}.tar ${TMPDIR}
	@rm -r ${TMPDIR}
	@bzip2 ${TMPDIR}.tar
