signed short int setFCDFrequency(double frequency)
{
	int stat;
	int corr = 0;
    double freqf = frequency;
	int freq = (int)(freqf * 1.0e6f);

	/* calculate frequency */
	freq *= 1.0 + corr / 1000000.0;

	/* set it */
	stat = fcdAppSetFreq(freq);	
	
	if (stat == FCD_MODE_NONE)
	{
		printf("No FCD Detected.\n");
		return -1;
	}
	else if (stat == FCD_MODE_BL)
	{
		printf("FCD in bootloader mode.\n");
		return -1;
	}
	else	
	{
		printf("Freq set to %.6f MHz.\n", freq/1e6);
		return 0;
	}
}
