cd build
make -j32
cd ..
# ./build/bin/sender_cli -d senderdb-16M-1024 -p parameters/16M-1024.json -l debug
# ./build/bin/sender_cli -d db.csv -p parameters/16M-1024.json -l debug
kill -9 $(netstat -antp | grep :1212 | awk '{print $7}' | awk -F'/' '{ print $1 }')
./build/bin/sender_cli -d db.csv -p parameters/1M-5535-PoL.json -l debug -t 1