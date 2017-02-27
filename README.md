# CS130 Webserver

This repository contains the Code Monkeys' code for the CS130 webserver
project.

Repository layout:

* build - compiler objects and libraries (.a files)
* bin - compiled binaries
* lib - external libraries
* src - project modules, grouped by folder
* test - unit test home

Project modules contain header files, source code and unit tests, along with
the Makefiles required to build those modules. Unit tests do not nee

The /test directory is intended to provide a place for unit tests to be linked
together and run as a single binary. It is assumed that each module will place
its unit tests' object files under /build.

## Server configuration

The server may be configured through a file based on nginx format. It must
follow this form:

server {
  listen: 8080;
  echo {
    echo_uri_1;
    echo_uri_2;
    ...
  }
  serve {
    file_uri_1 dir_1;
    file_uri_2 dir_2;
    ...
  }
}

The order of configuration parameters does not matter, and echo/serve blocks
may be split as follows:

server {
  echo {
    echo_uri_1;
  }
  ...
  echo {
    echo_uri_2;
  }
}

However, URI's must not be repeated in the configuration.

## Webserver API
All relevant .h and .cc files for the [CS130 API](https://github.com/UCLA-CS130/webserver-api) are stored in /src/api.

### Request Handler
The RequestHandler abstract base class can be found in /src/api/request_handler.h and can be included in other source files using `#include "api/request_handler.h"`. RequestHandler::Init initializes a handler with the longest prefix matching URI and the NginxConfig substatement that contains the relevant config block. Some handler implementations may not need to use the config or URI, so you can void cast them in the functions as a no-op that passes the W-unused-variable compile flag. 

### Request Handler Registerer
Also in request_handler.h is the REGISTER_REQUEST_HANDLER(ClassName) macro. This code is taken from [the registerer](https://github.com/jfarrell468/registerer/blob/master/request_handler.h) that the professor wrote for us. Suppose I create `class ProxyHandler : public RequestHandler` and implement the pure virtual functions. After the class definition, put `REGISTER_REQUEST_HANDLER(ProxyHandler)` and you will be able to create an instance of that RequestHandler by its string name. For example, I can use the static factory method as follows: `auto handler_ = RequestHandler::CreateByName("ProxyHandler");`. Note that RequestHandler::CreateByName takes the string name of the class. 

# Contributing
Contributing to this project is pretty straightforward. Source files go in /src, and test files go in /test. The project is set up with recursive Makefiles, so the top level Makefile will actually call Makefiles in subdirectories.

## Adding Files/Directories
Files added to /src and /test will automatically be built and linked by a wildcard expression in the respective Makefiles. Any file ending in .cc will be compiled and linked to the main executable. Note that by default the Makefile does not recursively check subdirectiories. If you choose to add a new directory in /src for organization, you will have to reflect those changes in the Makefile by adding another wildcard expression in [CC_SOURCES](https://github.com/UCLA-CS130/Code-Monkies/blob/master/src/Makefile#L4).

## Running and Adding Tests
Unit test files go in the /test directory. Like /src, all .cc files in /test will be compiled and linked into the test executable, which is automatically run by `make test`. Currently `make int-test` runs the integration test in /scripts/integration.sh as well as having a dependency on `make test`. To add your own integration test, add the script to /scripts and either have `make int-test` call the script or make a new task with appropriate dependencies.

## Submodules
Git Submodules are stored in /lib to avoid top level clutter. It is your choice to add a new submodule, but if it makes sense to use then by all means add it. The worst we can do is say no in the pull request.

## Questions
If you have any questions, you can email us at ahesselgrave@ucla.edu
