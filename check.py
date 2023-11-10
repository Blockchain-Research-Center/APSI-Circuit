f1 = open("input0.txt", "r").readlines()

f2 = open("input1.txt", "r").readlines()


cnt = 0
for i in range(40960):
    if int(f1[i] == f2[i]):
        cnt += 1
print(cnt)
