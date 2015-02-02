CC=gcc
CFLAGS= -std=c99 -Wall
LIBS= -lm -lreadline
OBJECTS= main.o brainfuck.o repl.o
DIST_SOURCES= *.c *.h Makefile README.md *.man
TARGET=bfng
PNAME=brainfuck-ng
VERSION=`cat main.h|grep VERSION |cut -d '"' -f2`
BINDIR=/usr/local/bin
MANDIR=/usr/local/share/man/man1
DIST_DIR=$(PNAME)
MANSRC=${PNAME}.man
MANTARGET1=${PNAME}.1
MANTARGET2=${TARGET}.1
SHELL=/bin/bash


all: $(TARGET)

.PHONY: clean install uninstall dist

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $(TARGET)

%.o : %.c
	$(CC) $(CFLAGS) -c $<


clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -rf ./${DIST_DIR}/*
	if [ -d ${DIST_DIR} ]; then rmdir ${DIST_DIR}; fi

install:
		mkdir -p ${DESTDIR}${BINDIR}
		cp -p ${TARGET} ${DESTDIR}${BINDIR}/${TARGET}
		chmod 555 ${DESTDIR}${BINDIR}/${TARGET}
		mkdir -p ${DESTDIR}${MANDIR}
		cp -p ${MANSRC} ${DESTDIR}${MANDIR}/${MANTARGET1}
		cp -p ${MANSRC} ${DESTDIR}${MANDIR}/${MANTARGET2}
		chmod 644 ${DESTDIR}${MANDIR}/${MANTARGET1}
		chmod 644 ${DESTDIR}${MANDIR}/${MANTARGET2}

uninstall:
		rm -f ${DESTDIR}${BINDIR}/${TARGET}
		rm -f ${DESTDIR}${MANDIR}/${MANTARGET1}
		rm -f ${DESTDIR}${MANDIR}/${MANTARGET2}

dist: #TODO:mod
		mkdir ${DIST_DIR}
		for i in $(DIST_SOURCES); do cp $$i ${DIST_DIR}/; done;
		COPYFILE_DISABLE=1 tar -cvzf ${PNAME}-${VERSION}.tar.gz ${DIST_DIR}/
		rm -rf ./${DIST_DIR}/*
		rmdir ${DIST_DIR}

help:
		@ echo "The following targets are available"
		@ echo "help      - print this message"
		@ echo "install   - install everything"
		@ echo "uninstall - uninstall everything"
		@ echo "clean     - remove any temporary files"
		@ echo "dist      - make a dist .tar.gz tarball package"
