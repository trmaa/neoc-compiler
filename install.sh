#!/usr/bin/env bash

gcc *.c -o /usr/bin/ncc
cd syntax
./install.sh
cd ..
