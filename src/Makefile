SHELL := /bin/bash

######################
###     PARAMS     ###
######################
# network
ifeq ($(strip ${NET}),)
net="ZteNet"
else
net="${NET}"
endif
# sim_time
ifeq ($(strip ${SIM_TIME}),)
sim_time_blk=
else
sim_time_blk=--sim-time-limit=${SIM_TIME}
endif

######################
###    COMMANDS    ###
######################
update-submodules:
	git submodule update --init --recursive

build:
	cd zte_qos; make MODE=release all

setup-dev: update-submodules build

run:
	cd zte_qos/simulations; ../src/zte_qos -m -u Cmdenv -c ${net} omnetpp.ini ${sim_time_blk}
