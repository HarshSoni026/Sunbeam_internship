#!/bin/bash
# test_clients.sh

for i in {1..3}
do
    (
        echo "Hello from client $i"
        sleep 1
        echo "Another message from client $i"
        sleep 1
    ) | nc 127.0.0.1 8081 &
done

wait
