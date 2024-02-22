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

# Generate Makefile(s) for OMNeT++ project building
gen-makefiles: update-submodules
	cd zte_qos; make makefiles

# Build the OMNeT++ project using the generated Makefile(s)
build: update-submodules
	cd zte_qos; make MODE=release all

# Set up development environment
setup-dev: update-submodules
	make build || (make gen-makefiles && make build)

# Run the built bench
run: update-submodules
	cd zte_qos/simulations; ../src/zte_qos -m -u Cmdenv -c ${net} omnetpp.ini ${sim_time_blk}
