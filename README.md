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
