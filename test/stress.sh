#!/bin/bash

set -u

if ! tmux -V; then
  echo "install tmux to run this script."
  exit 1
fi

if ! irssi -v; then
  echo "install irssi to run this script."
  exit 1
fi

cd $(dirname -- "$0")/..

echo Make server
if ! make all; then
  echo "failed to make server"
  exit 1
fi

echo Make bot
if ! make -C bot all; then
  echo "failed to make bot"
  exit 1
fi

function sendlinefrom {
  from=$1
  line=$2
  
  tmux selectp -t $from
  tmux send-keys "$line" ENTER
}

tmux new-session -d -s test_session 'zsh'
tmux split-window -h
tmux split-window -h
tmux selectp -t 0
tmux split-window -v

# run server
sendlinefrom 0 './ircserv'
sleep 0.2

sendlinefrom 1 'cd bot && ./msbot' 
sleep 0.2

# connect client 1
sendlinefrom 2 'irssi -w password -c 127.0.0.1 -n test1'

# connect clinet 2
sendlinefrom 3 './segfault.sh'

tmux attach -t test_session
