aiSat
=====

The directories 

.
├── LICENSE
├── Makefile
├── README.md
├── aisat_release
├── cavexample2.txt
├── depend.mk
├── example.txt
├── include
├── lib
├── logic
├── poly
├── pro.txt
├── psd
├── result.txt
├── sdp
├── src
└── util

sdp: semidefinite programming lib
psd: convert polynomils constraints to semidefinit programmig lib
poly: polynomial algebra system
logic: formal logical forumation system
util: common lib
==============

mathemtics optmization and control theorem lib

like sostool we want present a tool to integrate sdp and semidefinite algebra

system: linux  system


need libs:   lapack blas 

complie:

mkdir lib

make

make

usage:
./aisat_release filename

example:
./aisat_release example.txt


CAV13 eaxmple:
./aisat_release example.txt

