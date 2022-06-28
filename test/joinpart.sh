#! /bin/bash

source $(dirname -- "$0")/setup2cli.sh

# join channel
sendlinefrom 2 '/join test'
sendlinefrom 3 '/join test'

# simple chat
sendlinefrom 2 'hello!!'
sendlinefrom 3 'hi'

sendlinefrom 2 '/part #test'

sendlinefrom 3 'hello...?'

sendlinefrom 2 '/join test'
sendlinefrom 2 'greetings!'

tmux attach -t test_session
