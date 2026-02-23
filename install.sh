#!/usr/bin/env bash

gcc *.c /usr/bin/ncc
cd syntax
./install.sh
cd ..
