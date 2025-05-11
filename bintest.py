f = open("server.log", "r").readlines()

f = f[22:-159]


table = [[] for i in range(8192)]
for line in f:
    line = line.strip("\n").split(", ")
    idx = int(line[-1])
    v = 0
    for i in range(4):
        v = v * 2**20 + int(line[i])
    table[idx].append(v)

print(len(f))


f = open("client.log", "r").readlines()

cuckoo_table = [0] * 8192
cnt = 0
for idx in range(len(f)):
    line = f[idx]
    line = line.strip("\n").split(", ")
    v = 0
    for i in range(4):
        v = v * 2**20 + int(line[i])
    if v == 0:
        cnt += 1
    cuckoo_table[idx] = v

print(cnt)

for idx in range(len(cuckoo_table)):
    if cuckoo_table[idx] in table[idx]:
        print("find")
