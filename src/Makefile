SHELL := /bin/bash

setup-dev:
	make update-submodules
	cd inet; chmod 754 setenv && source setenv && git checkout setenv && make makefiles && make

update-submodules:
	git submodule update --init --recursive

build:
	cd zte_qos; make MODE=release all

run-ztenet:
	cd zte_qos/simulations; ../src/zte_qos -m -u Cmdenv -c ZteNet omnetpp.ini
