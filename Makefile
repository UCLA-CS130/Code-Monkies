# This is a top-level Makefile. It contains project-wide compiler settings and
# variable definitions. The recipes it provides are:
# 	compile (default) -> build source code only
# 	lib -> build third party libraries
# 	test -> build unit tests
# 	clean

# Project-wide compiler settings.
export CXX=g++
export CXXFLAGS=-c --std=c++11 -Wall -Wextra -Werror
export LDFLAGS=-lboost_system

# Get directory of this Makefile, AKA the top-level directory of the project.
# Adapted from this stackoverflow post: http://stackoverflow.com/a/18137056
MAKEFILE_PATH=$(abspath $(lastword $(MAKEFILE_LIST)))
export PROJ_ROOT=$(patsubst %/,%,$(dir $(MAKEFILE_PATH)))

export OBJ_DIR=$(PROJ_ROOT)/build
export BIN_DIR=$(PROJ_ROOT)/bin

export GTEST_DIR=$(PROJ_ROOT)/lib/googletest/googletest
export PARSER_DIR=$(PROJ_ROOT)/lib/nginx-configparser

.PHONY: compile
compile: lib
	$(MAKE) -C src

.PHONY: lib
lib:
	$(MAKE) -C lib

.PHONY: test
test: compile lib
	$(MAKE) -C test

.PHONY: clean
clean:
	$(MAKE) -C src clean
	$(MAKE) -C test clean
	$(MAKE) -C lib clean
