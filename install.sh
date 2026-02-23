#!/usr/bin/env bash

if [[ -f /usr/bin/ncc ]]; then
	mv /usr/bin/ncc /usr/bin/old_ncc
fi
./ncc *.nc *.nh -o /usr/bin/ncc
cd syntax
./install.sh
cd ..
