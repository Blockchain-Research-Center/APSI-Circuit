import subprocess
from time import sleep


subprocess.run("sed -i 's/auto numOfslice = 6/auto numOfslice = 5/g' sender/apsi/sender_db.cpp", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
subprocess.run("./server.sh", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
subprocess.run("nohup ./build/bin/sender_cli -d db-1M.csv -p parameters/1M-5535-PoL.json -l debug -t 1 > server-1M-5535.log 2>&1 &", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
subprocess.run("./client1M.sh > client-1M-5535.log", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

sleep(5)

subprocess.run("./server.sh", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
subprocess.run("nohup ./build/bin/sender_cli -d db-16M.csv -p parameters/16M-5535-PoL.json -l debug -t 1 > server-16M-5535.log 2>&1 &", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
subprocess.run("./client16M.sh > client-16M-5535.log", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)


sleep(5)

subprocess.run("sed -i 's/auto numOfslice = 5/auto numOfslice = 6/g' sender/apsi/sender_db.cpp", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
subprocess.run("./server.sh", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
subprocess.run("nohup ./build/bin/sender_cli -d db-256M.csv -p parameters/256M-1024-PoL.json -l debug -t 32 > server-256M-1024.log 2>&1 &", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
subprocess.run("./client256M.sh > client-256M-1024.log", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)


sleep(5)

subprocess.run("python3 ./tools/scripts/test_data_creator.py 1048576 11041 20 0 64", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
subprocess.run("python3 ./tools/scripts/test_data_creator.py 16777216 11041 20 0 64", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
subprocess.run("python3 ./tools/scripts/test_data_creator.py 268435456 11041 20 0 64", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

subprocess.run("sed -i 's/auto numOfslice = 6/auto numOfslice = 3/g' sender/apsi/sender_db.cpp", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
subprocess.run("./server.sh", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
subprocess.run("nohup ./build/bin/sender_cli -d db-1M.csv -p parameters/1M-11041-PoL.json -l debug -t 1 > server-1M-11041.log 2>&1 &", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
subprocess.run("./client1M.sh > client-1M-11041.log", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

sleep(5)

subprocess.run("./server.sh", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
subprocess.run("nohup ./build/bin/sender_cli -d db-16M.csv -p parameters/16M-11041-PoL.json -l debug -t 1 > server-16M-11041.log 2>&1 &", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
subprocess.run("./client16M.sh > client-16M-11041.log", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)


sleep(5)

subprocess.run("./server.sh", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
subprocess.run("nohup ./build/bin/sender_cli -d db-256M.csv -p parameters/256M-2048-PoL.json -l debug -t 32 > server-256M-2048.log 2>&1 &", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
subprocess.run("./client256M.sh > client-256M-2048.log", shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)