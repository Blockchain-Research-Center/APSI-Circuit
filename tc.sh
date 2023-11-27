sudo tc qdisc add dev lo root handle 1: netem delay 40ms
sudo tc qdisc add dev lo parent 1:1 handle 10: tbf rate 100mbit burst 32kbit latency 400ms
# sudo tc qdisc del dev lo root
