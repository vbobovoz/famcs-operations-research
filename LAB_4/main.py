import numpy as np


def knapsack(v, w, W):
    size = len(v)
    f = np.zeros((size + 1, W + 1))
    p = np.zeros((size + 1, W + 1))
    
    for i in range(1, size + 1):
        for j in range(1, W + 1):
            if w[i - 1] <= j:
                f[i,j] = max(f[i - 1, j], f[i, j - w[i - 1]] + v[i - 1])
            else:
                f[i,j] = f[i - 1, j]
            
            if f[i,j] == f[i - 1, j]:
                p[i,j] = 0
            else: 
                p[i,j] = 1
                
    # Обратный ход
    selected = np.zeros_like(w)
    i, j = size, W
    while i != 0 and j != 0:
        if p[i,j] == 1:
            selected[i - 1] += 1
            j -= w[i - 1]
        else:
            i -= 1

    return f[size, W], selected

if __name__ == '__main__':
    v = np.array([3, 8, 12])
    w = np.array([2, 3, 3])
    W = 8
    
    max_value, selected = knapsack(v, w, W)
    print(f"Оптимальное решение задачи: {max_value}")
    print(f"Выбранные предметы        : x1={selected[0]}, x2={selected[1]}, x3={selected[2]}")