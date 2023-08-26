import numpy as np
import os
import matplotlib.pyplot as plt

filename = input("请输入保存中轴线的文件的名称: ")
k = input("输入函数的的宽度k: ")
k = float(k)
Maxy = 20
# Maxy 限制最大的y的取值

part1_3_y = np.arange(1.0, Maxy, (Maxy-1)/500)
part1_x = []
part3_x = []
for i in part1_3_y:
    part1_x.append(k / 2 - (i ** (1/30)) * k / 2)
    part3_x.append(k / 2 + (i ** (1/30)) * k / 2)

part2_x = np.arange(0.0, k, k/2000)
part2_y = []
for i in part2_x:
    part2_y.append(((2/k) * (i-(k/2))) ** 30)

np.savetxt(filename, list(zip(part1_x,part1_3_y)))
np.savetxt(filename, list(zip(part2_x,part2_y)))
np.savetxt(filename, list(zip(part3_x,part1_3_y)))
# if not os.path.isfile(filename):
#     open(filename, 'x')
# with open(filename, 'w') as f:
#     f.write(list(zip(part1_x,part1_3_y)))
#     f.write(list(zip(part2_x,part2_y)))
#     f.write(list(zip(part3_x,part1_3_y)))
def getthecenterline(x , k = 40):
    return ((2/k) * (x - (k/2))) ** 30