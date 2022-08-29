from ntpath import join
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits import mplot3d


x = []
y = []
z = []
colours = []

order = 3

file = open('cubedata.txt', 'r')

fill = 0


temp = ""

last_char = ''

index = 1

while 1:

    char = file.read(1)
    

    if(char == '('):
        fill = 1
        continue
    if(char == ')'):
        fill = 0
        continue
    
    # Values
    if(char == ','):
        if(index%3 == 1):
            x.append(float(temp))
        elif(index%3 == 2):
            y.append(float(temp))
        elif(index%3 == 0):
            z.append(float(temp))
        index += 1 
        temp = ""
        continue
    
    if(fill == 1):
        temp += char


    # colours
    if(char == 'W'):
        colours.append("White")
    elif(char == 'Y'):
        colours.append("Yellow")
    elif(char == 'O'):
        colours.append("Orange")
    elif(char == 'R'):
        colours.append("Red")
    elif(char == 'G'):
        colours.append("Green")
    elif(char == 'B'):
        colours.append("Blue")

    if not char:
        break
 
file.close()



plt.figure()

ax = plt.axes(projection='3d')

ax.scatter(x,y,z, s = 20000/order, marker = ".", c = colours)

ax.set_xlabel("x")
ax.set_ylabel("y")
ax.set_zlabel("z")

plt.show()