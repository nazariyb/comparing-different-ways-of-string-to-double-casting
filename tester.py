# coding=utf-8
import os, sys
import csv

arguments_number = len(sys.argv)
assert arguments_number != 4, 'Incorrect input. Not enough arguments'

types = int(sys.argv[1])
print(types < 0)
assert types > 0, 'number of types should be positive integer'

runs = int(sys.argv[2])
input_file = sys.argv[3]
data_fold = '../data/'

print('Running...')

src_fold = ''
for root, dirs, files in os.walk('.'):
	for dir in dirs:
		os.chdir(dir)
		for root1, dirs1, files1 in os.walk('.'):
			for file in files1:
				if file == 'lab1.exe' or file == 'lab1':
					file_name = file if file == 'lab1.exe' else './' + file
					src_fold = dir
					break
			if src_fold:
				break
		if src_fold:
			break
		os.chdir('../')
	if src_fold:
		break

results = []
times = []
for t in range(1, types + 1):

	correct_input = True
	times.append(list())

	print('Testing type {}...'.format(t))

	for _ in range(runs):
		# print("{3} {0} {2}{1} {2}type_{0}_result.txt > {2}temp_time_result.txt".format(t, input_file, data_fold, file_name))
		os.system("{3} {0} {2}{1} {2}type_{0}_result.txt > {2}temp_time_result.txt".format(t, input_file, data_fold, file_name))
		with open("{}temp_time_result.txt".format(data_fold)) as time:
			line = time.readline()
			while not line.startswith("Total time:"):
				if line.startswith("Error."):
					if line.startswith("Error. Incorrect input."):
						correct_input = False
						types = len(results)
					break
				line = time.readline()
			else:
				times[t - 1].append(int(line.split()[-1]))

	print('Storing results.')

	if correct_input:
		with open("{}type_{}_result.txt".format(data_fold, t)) as result:
			results.append(tuple(map(float, result.readlines())))
	else:
		break
	
	print('Type {} is tested.'.format(t))

print("Results don't match each other" if len(set(results)) > 1 else "Results are OK")

for t in range(1, types + 1):
	print("Type {} minimal execution time: {}μs".format(t, min(times[t - 1])))

type_names = ["using stream", "using stod() func", "using sscanf() func", "using boost lib"]

with open(data_fold + sys.argv[4], 'w') as t_r:
	writer = csv.writer(t_r)
	writer.writerow(["Type #", "Type description", "Time μs"])
	for t in range(1, types + 1):
		writer.writerow([t, type_names[t - 1], min(times[t - 1])])

print("Time results are saved to '" + data_fold + "time_results.csv'.")
