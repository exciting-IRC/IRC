#! /bin/bash

source $(dirname -- "$0")/setup2cli.sh

sendlinefrom 2 '/join test'
sendlinefrom 3 '/join test'

sendlinefrom 2 'hi!'
sendlinefrom 3 'greetings!'

sendlinefrom 2 '/kill test2'
sendlinefrom 2 '/oper admin asdfdsf'
sendlinefrom 2 '/oper admin admin'
sendlinefrom 2 '/kill test2'

sendlinefrom 2 'C-n'
sendlinefrom 3 'C-n'

tmux attach -t test_session
