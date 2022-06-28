#! /bin/bash

source $(dirname -- "$0")/setup2cli.sh

sendlinefrom 2 '/join test'
sendlinefrom 3 '/join test'

sendlinefrom 2 'hi!'
sendlinefrom 2 '/nick test3'
sendlinefrom 2 'my name is test3 now'

sendlinefrom 3 'wow.'

tmux attach -t test_session
