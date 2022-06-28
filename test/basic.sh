#! /bin/bash

trap 'cleanup' INT

SERV_PID = -1
BOT_PID = -1

function cleanup () {
  echo cleanup
  kill -TERM SERV_PID
  kill -TERM BOT_PID
  exit 0
}

cd ..

make all
make -C bot all

echo Launch server...
./ircserv
sleep 0.5

echo Launch Bot...
./bot/msbot

echo 123
