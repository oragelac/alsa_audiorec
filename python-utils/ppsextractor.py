import wave
import argparse
import struct
import csv
import re
from subprocess import Popen, PIPE

def parse_wavefile(wavefile):

	wav = wave.open(wavefile, 'rb')

	(nchannels, sampwidth, framerate, nframes, comptype, compname)  = wav.getparams()

	pps = []
	average = 29000
	threshold = 25000
	frames = []
	ppslength = framerate / 10
	n = nframes

	for i in range(nframes / n):

		if i == (nframes / n) - 1:

			n += (nframes % n)

		frame = wav.readframes(n)
		v = (struct.unpack("<%dh" % n, frame))
		frames.extend(v)

	wav.close()		

	i = 0

	while i < nframes - 2:

		diff = frames[i + 2] - frames[i]

		if diff > threshold:
			
			pps.append(i + 1)
			i += 2 + ppslength
		else:

			i += 1		

	return pps

def parse_bramspps_output(filename):

	args = ['brams', 'pps', filename]

	proc = Popen(args, stdout = PIPE, stderr  = PIPE)
	out, err = proc.communicate()
	
	pps = re.findall(r"\[([^\]]*),", out)
	pps  = [ int(r) for r in pps]
	
	return pps


def write_csvfile(csvfile, data):

	with open(csvfile, 'w') as f:

		writer = csv.writer(f, delimiter = ';')

		for i in range(len(data)):
			
			writer.writerow((i + 1, data[i]))
		
		f.close()
	

if __name__ == '__main__':
	
	parser = argparse.ArgumentParser(description='BRAMS wave file PPS extractor')
	parser.add_argument('wavefile', type=str, help='wave files to analyze' , nargs="+")
	args = vars(parser.parse_args())
	wavefiles = args['wavefile']

	for wavefile in wavefiles:
		
		pps = parse_wavefile(wavefile)
		write_csvfile(wavefile.rsplit('.', 1)[0] + '.pps.csv', pps)
		ts = parse_bramspps_output(wavefile)
		write_csvfile(wavefile.rsplit('.', 1)[0] + '.ts.csv', ts)
		print(str(len(pps)) + "\tPPS in\t\t" + wavefile)
		print(str(len(ts)) + "\ttimestamps in\t" + wavefile + '\n')


