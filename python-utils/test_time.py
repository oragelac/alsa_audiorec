import argparse
import csv
import matplotlib.pyplot as plt


parser = argparse.ArgumentParser(description='BRAMS time test plotter')
parser.add_argument('csvfile', type=str, help='csv file')
args = vars(parser.parse_args())
csvfile = args['csvfile']

data = []
with open(csvfile, 'r') as f:

	reader = csv.reader(f, delimiter = ';')

	for row in reader:

		data.append(float(row[1]))	
		
	f.close()

plt.plot(data, '.')
plt.show()


