#!/usr/bin/env bash

mkdir -p ~/.vim/syntax/
mkdir -p ~/.vim/ftdetect/

cp -r syntax/* ~/.vim/syntax/
cp -r ftdetect/* ~/.vim/ftdetect/
