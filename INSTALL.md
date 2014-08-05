Installation instructions for ClassCreator
==========================================

* To build and install the RELEASE version use the following commands on a linux
  terminal in the source folder (Where the CMakeLists.txt file is):

1. make build && cd build
2. cmake -DCMAKE_BUILD_TYPE=RELEASE ..
3. make
4. make install (As superuser. it will install with /usr prefix)

* To uninstall use 'make uninstall' (without the quotes) as superuser.

* To build the DEBUG version use:

1. make build && cd build
2. cmake ..
3. make
