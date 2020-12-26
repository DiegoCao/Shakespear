import numpy as np
learning_rate = 0.3
x = []
with open("train.txt", 'r') as f:
    for line in f:
        x.append(map(float, line.strip().split()))
    xs, ys = zip(*x)
    xs, ys = np.asarray(xs), np.asarray(ys)
print(xs)   
print(ys)

