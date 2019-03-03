from random import uniform
import os, sys

if not os.path.isdir("./data"):
	os.system("mkdir data")

if len(sys.argv) == 1:
	file_name = "data/test_data.txt"
	numbers =  1_000_000
elif len(sys.argv) == 2:
	file_name = "data/" + sys.argv[1]
	numbers =  1_000_000
elif len(sys.argv) == 3:
	file_name = "data/" + sys.argv[1]
	numbers = sys.argv[2]
else:
	print('Incorrect input. Read README.md')
	sys.exit(0)
with open(file_name, "w") as test_data:
	for _ in range(int(numbers)):
		test_data.write(str(uniform(-1_000_000_000, 1_000_000_000)) + '\n')
print("Your file is stored in 'data/'\nYour file name is: '" + file_name + "'\n use it when run lab1.exe(./lab1)")
