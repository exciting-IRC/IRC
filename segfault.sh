trap "kill 0" SIGINT
trap "kill 0" EXIT
trap "exit" INT TERM

while true; do
  python3 connect.py || exit
  sleep 0.1
done
