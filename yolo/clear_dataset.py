import os


CONE = 0
PARE = 1
SIGA = 2
images_path = "./dataset"
files = [images_path + file for file in os.listdir(images_path)]
count = 0
for file in files:
    if(file[-3:] == "txt"):
        with open(file, "r") as f:
            lines = f.readlines()
        with open(file, "w") as f:
            have_sign = False
            for line in lines:
                if(line[0] == PARE or line[0] == SIGA):
                    have_sign = True
                    f.write(line)
            if(not have_sign):
                img_name = file[:-3] + "JPG"
                os.remove(file)
                os.remove(img_name)
                count += 1

print("deleted files {}".format(count))
