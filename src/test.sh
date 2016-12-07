#!/bin/bash
g++ -Wall -D__MACOSX_CORE__ -I/usr/local/Cellar/stk/4.5.0/include/stk -o asd main.cpp -lstk && \
	./asd && afplay sine.wav

