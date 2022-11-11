.PHONY: clean install

PREFIX ?= /usr
INSTALL_DIR ?= ${PREFIX}/lib/lv2

all: lv2/relative_dynamics/dynamics.so

lv2/relative_dynamics/dynamics.so: dynamics.cc
	g++ -shared -o lv2/relative_dynamics/dynamics.so dynamics.cc

install: all
	install -d ${INSTALL_DIR}/relative_dynamics.lv2
	install lv2/relative_dynamics/dynamics.so ${INSTALL_DIR}/relative_dynamics.lv2
	install lv2/relative_dynamics/*.ttl ${INSTALL_DIR}/relative_dynamics.lv2

