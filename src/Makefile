SHELL := /bin/bash

setup-dev:
	make update-submodules
	cd inet; chmod 754 setenv && source setenv && git checkout setenv && make makefiles && make

update-submodules:
	git submodule update --init --recursive
