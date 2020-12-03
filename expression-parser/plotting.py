import matplotlib.pyplot as plt

import sys

# line 1 points
x1 = sys.argv[1].split(',')
y1 = sys.argv[2].split(',')

x1 = [float(i) for i in x1]
y1 = [float(i) for i in y1]

print(x1)
print(y1)

# setting the axes at the centre
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
ax.spines['left'].set_position('center')
ax.spines['bottom'].set_position('center')
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
ax.xaxis.set_ticks_position('bottom')
ax.yaxis.set_ticks_position('left')

# plotting the line 1 points
plt.plot(x1, y1, label="line 1")

# function to show the plot
plt.show()
