# **Code Loom**
My CS degree capstone project

## Description (What)
Compile, run, and compare the output of Java programs agaisnt a suite of user provided test cases and report back to the user which passed and which failed.

## Quick Start (how)
`dependencies`
- GNU Make
- gcc
- jdk
- less

To get started download the repository
```
curl -L -o code_loom.zip  https://github.com/yisroelshulman/code_loom/zipball/master
```

then unzip
```
unzip code_loom.zip
```

cd into the unzipped folder
run  make
```
make
```

This will create a new executable a.out and is now ready.

## Usage
Read the [SUL Manual][].\
Read the [Code Loom Manual][].

[SUL Manual]: https://docs.google.com/document/d/1Orgrxw0ZMv2xZduPHcYj2XTZWCJz-M6pQKo-W9VO2o0/edit?usp=sharing
[Code Loom Manual]: https://docs.google.com/document/d/1LsW59idS7LsewdEUGofjYueIyXoyFe6hmMkGVkKrmfE/edit?usp=sharing

To use the tool create a input.sul file with the test cases.\
Write a java source code file.\
run
```
./a.out default
```
And follow the propmpt to run with check or test cases to see the results.