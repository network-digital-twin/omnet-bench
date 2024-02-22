SHELL := /bin/bash

# parameters
net=${NET:-"ZteNet"}
[ -z ${SIM_TIME} ] && sim_time_blk="" || sim_time_blk="--sim-time-limit=${SIM_TIME}"

update-submodules:
	git submodule update --init --recursive

build:
	cd zte_qos; make MODE=release all

setup-dev: update-submodules build

run:
	cd zte_qos/simulations; ../src/zte_qos -m -u Cmdenv -c ${net} omnetpp.ini ${sim_time_blk}
