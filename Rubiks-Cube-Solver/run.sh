#!/bin/bash

cd ../Rubiks-Cube-Solver

/usr/bin/time --output=time_solver.txt ./solver ${1} > ../build-cube-Desktop-Debug/result.txt

exit