#! /bin/bash

source $(dirname -- "$0")/setup2cli.sh

sendlinefrom 2 '/join test'
sendlinefrom 3 '/join test'

sendlinefrom 2 'hi!'
sendlinefrom 3 'greetings!'

sendlinefrom 2 '/quit goodbye'
sleep 0.1
sendlinefrom 2 'irssi -w password -c 127.0.0.1 -n test1'
sendlinefrom 2 '/join test'
sendlinefrom 2 '/quit goodbye'
sleep 0.1
sendlinefrom 2 'irssi -w password -c 127.0.0.1 -n test1'
sendlinefrom 2 '/join test'

tmux attach -t test_session
