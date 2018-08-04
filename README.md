# Artemis Core

## Introduction
Artemis is a system that can find components in compiled binary executable. We are using it to find vulnerable components and give user feedback on it.

I created this program while I was having a internship in Tsinghua University Software Security Lab, one of the top University in China.


This is artemis's core algorithm implmentation.
Core algorithm is purely C++, enjoy the speed boost.

## Requirements:
 1. A C++ compiler supporting C++11 (either clang, gcc or Visual Studio)
 2. cmake

## Usage:

- First build cxx:
 ```
 $ cd cxx
 $ mkdir build
 $ cd build
 $ cmake ..
 ```
- Copy the executable to project root directory
- If you do not have a database dump, run `python dump_mysql.py`
- Scan a directory and generate a file list by `python filefinder.py DIRECTORY_NAME`
- RUN the program by `./artemis-core`
- Enter any key to start

## TODO

- [X] Create Common Interfaces
- [X] Print in JSON
- [X] Select core components
- [X] Speedup
