#!/bin/bash
# send_signal.sh

echo "Sending SIGTSTP to child process (PID: $(pgrep -x child)) every 3 seconds..."
for i in {1..5}
do
    pid=$(pgrep -x child)
    if [ -n "$pid" ]; then
        echo "Sending SIGTSTP to $pid"
        kill -SIGTSTP "$pid"
    else
        echo "Child process not found."
        exit 1
    fi
    sleep 3
done

pid=$(pgrep -x child)
if [ -n "$pid" ]; then
    echo "Done sending signals. Now sending SIGTERM to child."
    kill -SIGTERM "$pid"
else
    echo "Child process not found."
    exit 1
fi
