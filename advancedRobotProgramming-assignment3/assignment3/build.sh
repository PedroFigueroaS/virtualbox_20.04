#!/bin/bash
mkdir -p ./bin
gcc ./src/master.c -lncurses -o ./bin/master -lrt -lpthread
gcc ./src/processA_server.c -lbmp -lm -lncurses -o ./bin/processA_server -lrt -lpthread
gcc ./src/processA_client.c -lbmp -lm -lncurses -o ./bin/processA_client -lrt -lpthread
gcc ./src/processB.c -lbmp -lncurses -lm -o ./bin/processB -lrt -lpthread
