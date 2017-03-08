# This is a top-level Makefile. It contains project-wide compiler settings and
# variable definitions. The recipes it provides are:
# 	compile (default) -> build source code only
# 	lib -> build third party libraries
# 	test -> build unit tests
# 	clean

# Project-wide compiler settings.
export CXX=g++
export CXXFLAGS=-c -std=c++11 -Wall -Wextra -Werror
export LDFLAGS=-lgcov --coverage -lboost_system -lboost_thread -lboost_regex -lpthread -lpython2.7
export DEBUG_FLAGS=-DDEBUG -g

ifdef VERBOSE
	export DEBUG_FLAGS+=-DDEBUG_VERBOSE
endif

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

.PHONY: debug
debug: lib
	$(MAKE) -C src debug

.PHONY: lib
lib:
	$(MAKE) -C lib

.PHONY: test
test: lib
	$(MAKE) -C src test
	$(MAKE) -C test
	cd bin && ./webserver_tests

.PHONY: proxy-test
proxy-test: lib
	$(MAKE) -C src test
	$(MAKE) -C test
	cd bin && ./proxy_webserver_tests

.PHONY: int-test
int-test: test
	scripts/integration.sh && scripts/multithread_test.sh && scripts/proxy_integration_test.sh

.PHONY: proxy-int-test
proxy-int-test: lib		
	scripts/proxy_integration_test.sh

.PHONY: proxy-302-test
proxy-302-test: lib
	scripts/proxy_302_test.sh

.PHONY: gcov
gcov: test
	gcov -o $(OBJ_DIR) -r src/*

.PHONY: clean
clean:
	$(MAKE) -C src clean
	$(MAKE) -C test clean
	$(MAKE) -C lib clean
	rm -rf $(OBJ_DIR)/*.gcda $(OBJ_DIR)/*.gcno
