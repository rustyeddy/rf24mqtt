#############################################################################
#
# Makefile for Raspberry Pi NRF24L01/NRF24L01+ receiver
#
# Run:
#     make clean; make
#     sudo ./recv
#
#############################################################################
prefix := /usr/local

PROG		= recv
SRCS		= recv.cc mqtt.cc
OBJS		= recv.o mqtt.o
HDRS		= mqtt.h
INCLUDES	= -I/usr/local/include
LIBDIR		= -L/usr/local/lib
LIBS		= -lmosquitto -lrf24-bcm

# The recommended compiler flags for the Raspberry Pi
# CCFLAGS		= -Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s -Wall
CCFLAGS		= -g -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s -Wall


all: ${PROG}

${PROG}: ${OBJS}
	g++ ${CCFLAGS} ${INCLUDES} ${LIBS} ${OBJS} -o $@

%.o : %.cc
	g++ -c ${CCFLAGS} ${INCLUDES} ${LIBS} $< 

clean:
	rm -rf recv *.o *~

install: all
	test -d $(prefix) || mkdir $(prefix)
	test -d $(prefix)/bin || mkdir $(prefix)/bin
	for prog in $(PROGRAMS); do \
		install -m 0755 $$prog $(prefix)/bin; \
	done

.PHONY: install
