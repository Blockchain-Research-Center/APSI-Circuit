from random import randint
import numpy as np
from sympy import randprime


b = 10
p = randint(2**b, 2 ** (b + 1))
d = 100
coeff = [randint(0, p - 1) for _ in range(d)]

x = np.array([randint(0, p - 1) for _ in range(d)], int)


def eval(X, coeff):
    r = np.zeros(len(X), int)
    base = np.array([1] * len(X), int)
    for i in range(len(coeff)):
        r += coeff[i] * base
        r = r % p
        base = base * X
        base = base % p
    return r


y = eval(x, coeff)


new_x = np.array(range(p), int)
cnt = 0
for i in range(100):
    cnt += np.count_nonzero((eval(new_x, coeff) == randint(0, p - 1)))
print(cnt)
