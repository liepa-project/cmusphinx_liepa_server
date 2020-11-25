#!/bin/bash
# aplay -fS16_LE -r 16000 test/test1.raw
arecord  -c 1 -f S16_LE -r 16000 -t raw -q | nc -N localhost 5050
