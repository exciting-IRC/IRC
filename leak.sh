#!/bin/bash

./ircserv &
echo pid is $!
echo "leaks $!" | pbcopy
