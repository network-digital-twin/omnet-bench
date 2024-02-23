SHELL := /bin/bash

######################
###     PARAMS     ###
######################
# network (e.g., TestNet)
ifeq ($(strip ${NET}),)
net=TestNet
else
net=${NET}
endif

# sim_time (e.g., 60s)
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

build-inet: update-submodules
	cd inet; source setenv; make makefiles; make MODE=release all

# Generate Makefile(s) for OMNeT++ project building
gen-makefiles:
	cd zte_qos; make makefiles || (make build-inet && make makefiles)

# Build the OMNeT++ project using the generated Makefile(s)
build:
	cd zte_qos; make MODE=release all || (make gen-makefiles && make MODE=release all)

# Run the built bench
run: build
	cd zte_qos/simulations; ../src/zte_qos -m -u Cmdenv -c ${net} omnetpp.ini ${sim_time_blk}
