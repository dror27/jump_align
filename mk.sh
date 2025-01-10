#!/bin/bash -xv

g++ \
 -I ./manta-1.6.0/src/c++/lib/alignment/ \
 -I ./manta-1.6.0/src/c++/lib \
 \
 ./manta-1.6.0/src/c++/lib/blt_util/blt_exception.cpp \
 ./manta-1.6.0/src/c++/lib/blt_util/align_path.cpp \
 ./manta-1.6.0/src/c++/lib/blt_util/parse_util.cpp \
 jump_align.cpp \
 -o jump_align

