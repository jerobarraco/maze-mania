#!/bin/bash
rm bin/run/mazemania
ln -s "$(pwd)/bin/$1/mazemania" ./bin/run/mazemania 
exit 0