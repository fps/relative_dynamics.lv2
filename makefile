.PHONY: clean install

PREFIX ?= /usr
INSTALL_DIR ?= ${PREFIX}/lib/lv2

all: lv2/relative_dynamics/dynamics.so

lv2/stereofication/stereo.so: stereo.cc
	g++ -shared -o lv2/stereofication/stereo.so stereo.cc `pkg-config --libs --cflags rubberband`

install: all
	install -d ${INSTALL_DIR}/stereofication
	install lv2/stereo.so ${INSTALL_DIR}/stereofication

