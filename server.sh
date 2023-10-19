cd build
make -j32
cd ..
# ./build/bin/sender_cli -d senderdb-16M-1024 -p parameters/16M-1024.json -l debug
./build/bin/sender_cli -d db.csv -p parameters/16M-1024.json -l debug