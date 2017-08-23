import argparse
import csv
import matplotlib.pyplot as plt

def read_csvfile(csvfile):
	
	data = []
	with open(csvfile, 'r') as f:

		reader = csv.reader(f, delimiter = ';')

		for row in reader:
			data.append(int(row[1]))	
		
		f.close()

	return data

if __name__ == '__main__':
	
	parser = argparse.ArgumentParser(description='BRAMS PPS plotter')
	parser.add_argument('directory', type=str, help='directory location')
	parser.add_argument('datetime', type=str, help='datetime string with format : yyyymmddhhmm', nargs='+')
	args = vars(parser.parse_args())
	directory = args['directory']
	datetimes = args['datetime']

	all_data = {}
	all_data['pps'] = []
	all_data['ts'] = []
	
	for datetime in datetimes:

		pps_csv_file = directory + 'RAD_BEDOUR_' + datetime[0:8] + '_' + datetime[8:] + '_TEST00_SYS001.pps.csv'
		ts_csv_file = directory + 'RAD_BEDOUR_' + datetime[0:8] + '_' + datetime[8:] + '_TEST00_SYS001.ts.csv'
	
	
		all_data['pps'].extend(read_csvfile(pps_csv_file))
		all_data['ts'].extend(read_csvfile(ts_csv_file))

	plt.subplot(211)
	print(len(all_data['pps']))
	print(len(all_data['ts']))

	diff = [all_data['pps'][i] - all_data['ts'][i] for i in range(len(all_data['pps']) if len(all_data['pps']) < len(all_data['ts']) else len(all_data['ts']))]
	print float(sum(diff)) / len(diff)
	plt.plot(diff, '.')

	plt.subplot(212)

	plt.hist(diff, 50)

	plt.show()
