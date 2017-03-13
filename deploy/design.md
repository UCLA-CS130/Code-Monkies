# Overview
Code Monkies presents an extension of the static handler that automatically converts Markdown files (*.md) to HTML
using embedded Python2.7 in C++

# Implementation
To begin, we had to install Python2.7 and the appropriate development tools to embed it in C++. We also used
a virtual environment to install the markdown library using `pip install markdown`. We knew what Python needed to be executed,
so we converted the Python code into C++ using [the Python C API](https://docs.python.org/2/extending/embedding.html).

# Challenges Faced

## Compiling
There were a considerable number of issues trying to get the executable to compile and link with python in general, and it was
an issue of the location of the "Python.h" header being nested in a subdirectory in /usr/include/.

## Developing
Although we were effectively writing Python, the C++ API is incredibly low level and every function returns a PyObject pointer which could potentially be null.
It was exacerbated by the fact that the markdown library expects a unicode input which was challenging to work with given that we created strings from char*.

# Demo URL
This file was actually rendered from Markdown! But to see the larger example, visit [this link](http://35.167.71.217/static/example.md)


