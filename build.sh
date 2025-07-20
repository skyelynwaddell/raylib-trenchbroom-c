#!/bin/bash
cd build || exit 1
cmake .. && make && cd .. && ./bin/game