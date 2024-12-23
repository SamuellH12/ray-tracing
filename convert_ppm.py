from PIL import Image
import random

nm = str(random.randint(1, 99999999))
nm = '0'*(8-len(nm)) + nm

im = Image.open("out.ppm")
im.save(f"./renders/{nm}.jpg")
