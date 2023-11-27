def f(base):
    l1 = []

    for i in base:
        for j in base:
            if i + j not in base:
                l1.append(i + j)
    for i in l1:
        base.append(i)

    s = []

    for i in base:
        if i not in s:
            s.append(i)

    base = sorted(s)
    return base


import random


def select_numbers():
    return random.sample(range(2, 16), 3)


# for i in range(20000):
#     print(i)
#     selected_numbers = select_five_numbers()
#     selected_numbers.append(16)
#     print(selected_numbers)
#     if f(selected_numbers) == 64:
#         print(selected_numbers)
#         exit()

# print(f([1, 3, 11, 15, 32]))

# src = [1, 4, 10, 11, 28, 33, 78]

# max = 0
# while 1:
#     src0 = [1] + select_numbers()

#     src = src0.copy()

#     for i in range(2):
#         src = f(src)

#     cnt = 1
#     while 1:
#         if cnt in src:
#             cnt += 1
#         else:
#             if cnt - 1 > max:
#                 print(max, src0)
#                 max = cnt - 1
#             break


src = [1, 4, 10, 11, 28, 33, 78]

for i in range(2):
    src = f(src)

cnt = 1
while 1:
    if cnt in src:
        cnt += 1
    else:
        print(cnt - 1)
        break
