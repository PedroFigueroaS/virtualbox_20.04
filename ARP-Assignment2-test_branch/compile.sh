#! /usr/bin/bash

#mkdir -p /bin
gcc src/master.c -Iinclude -lncurses -lbmp -lrt -lm -o bin/master -pthread
gcc src/processA.c -Iinclude -lncurses -lbmp -o bin/processA -lrt -pthread
gcc src/processB.c -Iinclude -lncurses -lbmp -o bin/processB -lrt -pthread