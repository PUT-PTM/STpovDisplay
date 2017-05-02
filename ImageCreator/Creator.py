import cv2
import numpy as np

pixels = 3

def rotateImage(image, angle):
    center=tuple(np.array(image.shape[0:2])/2)
    rot_mat = cv2.getRotationMatrix2D(center,angle,1.0)
    return cv2.warpAffine(image, rot_mat, image.shape[0:2],flags=cv2.INTER_LINEAR)


img = cv2.imread('input.png', 1)
img = cv2.resize(img, (pixels * 40, pixels * 40))

colors = []

for i in range(180):
    rotated = rotateImage(img, i * (-2))
    for j in range(4, 20):
        r, g, b = rotated[int((pixels * 40)/2), int(((pixels * 40)/2) + (pixels * j))]
        colors.append(b)
        colors.append(g)
        colors.append(r)


plik = open('output.txt', 'w')
for i in range(180 * 16 * 3):
    plik.write(str(colors[i]))
    plik.write("\n")
plik.close()