import argparse
import csv
import matplotlib.pyplot as plt

def read_csvfile(csvfile):
	
	data = []
	with open(csvfile, 'r') as f:

		reader = csv.reader(f, delimiter = ';')

		for row in reader:
			value = int(row[1]);
			data.append(value)
		
		f.close()

	return data

if __name__ == '__main__':
	
	parser = argparse.ArgumentParser(description='BRAMS PPS plotter')
	parser.add_argument('csvfile', type=str, help='csv file')
	args = vars(parser.parse_args())
	csvfile = args['csvfile']
	
	data = read_csvfile(csvfile)
	diff = []
	for i in range (1, len(data)):
		
		diff.append(data[i] - data[i-1])
	
	plt.subplot(211)
	plt.plot(diff, '.')
	plt.subplot(212)
	plt.hist(diff, 20)
	plt.show()
