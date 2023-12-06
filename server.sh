cd build
make -j
cd ..

kill -9 $(netstat -antp | grep :1212 | awk '{print $7}' | awk -F'/' '{ print $1 }')
