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
