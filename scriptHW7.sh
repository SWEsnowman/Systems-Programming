gcc client.c -o client
gcc server.c -o server
./server 1050 &
./client 1050

rm client
rm server