import numpy as np

# numpy array with elements [1,2,3,4]
arr1 = np.array([1,2,3,4])
print(arr1)

# np.ones 3x4 array
arrOnes = np.ones((3,4))
print(arrOnes)

# np.zeros 4x3 array
arrZeros = np.zeros((4,3))
print(arrZeros)

# 2x3 Matrix A
A = np.array([[1, 2, 3], [4, 5, 6]])

# 3x4 Matrix B
B = np.array([[7, 8, 9, 10], [11, 12, 13, 14], [15, 16, 17, 18]])

# numpy matrix multiplication
result = np.dot(A, B)
print("Result of matrix multiplication A * B:\n", result)

C = np.array([[3,1],[1,2]])
vals = np.linalg.eigh(C)
print(vals)