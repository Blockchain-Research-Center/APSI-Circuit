cd build
make -j32
cd ..
# ./build/bin/sender_cli -d senderdb-16M-1024 -p parameters/16M-1024.json -l debug
# ./build/bin/sender_cli -d db.csv -p parameters/16M-1024.json -l debug
kill -9 $(netstat -antp | grep :1212 | awk '{print $7}' | awk -F'/' '{ print $1 }')
# ./build/bin/sender_cli -d db-1M.csv -p parameters/1M-5535-PoL.json -l debug -t 32
# ./build/bin/sender_cli -d db-1M.csv -p parameters/1M-11041-PoL.json -l debug -t 32
./build/bin/sender_cli -d db-16M.csv -p parameters/16M-5535-PoL.json -l debug -t 32
# ./build/bin/sender_cli -d db-16M.csv -p parameters/16M-11041-PoL.json -l debug -t 32
# valgrind --tool=massif --time-unit=ms ./build/bin/sender_cli -d db-256M.csv -p parameters/256M-1024-PoL.json -l debug -t 32
# ./build/bin/sender_cli -d db-256M.csv -p parameters/256M-1024-PoL.json -l debug -t 32