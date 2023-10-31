from decimal import Decimal


m = 2**20
D = 3 * 2**15

m = Decimal("%d" % m)
d = Decimal("%d" % D)
one = Decimal("1.0")


def prob(b):
    global d
    B = Decimal("%d" % b)
    p = d / B
    for i in range(1, b):
        p *= (d - Decimal("%d" % i)) / (B - Decimal("%d" % i))

    res = (one - one / m) ** (d - b) * (one / m) ** b

    r = Decimal("0.0")
    for i in range(b, D + 1):
        res = res / m * (m - one) / m
        p = p * (d - Decimal("%d" % (i))) / Decimal("%d" % (i + 1))
        r += m * res * p

    return r


s = 2**40
threshold = one / Decimal("%d" % s)


flag = 1
lower = 1
upper = 2
while flag:
    if not (prob(upper) < threshold):
        upper = upper * 2
    if prob(upper) < threshold:
        flag = 0

print(upper)

while upper - lower > 1:
    mid = (upper + lower) // 2
    if prob(mid) < threshold:
        upper = mid
    else:
        lower = mid
    print(lower, upper)

print(upper)
