#include "arguments.h"

Arguments* initArguments(int argc, char **argv, char *optString)
{
	Arguments *arguments = NULL;
	arguments = malloc(sizeof(Arguments));
	
	arguments->argc = argc;
	arguments->argv = argv;	
	arguments->optString = malloc((strlen(optString) + 1) * sizeof(char));
	strcpy(arguments->optString, optString);
	
	return arguments;
}

void destroyArguments(Arguments *arguments)
{
	if(arguments->optString != NULL)
	{
		free(arguments->optString);
	}
	
	if(arguments->deviceName != NULL)
	{
		free(arguments->deviceName);
	}
	
	free(arguments);
}

void parseArgs(Arguments *arguments)
{
	int opt;
	char *endptr;
	
	opt = getopt(arguments->argc, arguments->argv, arguments->optString);
	
	while(opt != - 1)
	{
		switch(opt)
		{
			case 'd':
				
				arguments->deviceName = malloc ((strlen(optarg) + 1) * sizeof(char));
				strcpy(arguments->deviceName, optarg);
				break;
				
			case 'f':
				
				arguments->frequency = strtod(optarg, &endptr);
				break;
			
			case 'r':
			
				arguments->samplerate = (unsigned int) strtol(optarg, &endptr, 10);
				break;
			
			case 'w':
			
				arguments->waveSamplerate = strtod(optarg, &endptr);
				break;
			
			case 'c':
			
				arguments->channels = (unsigned short int) strtol(optarg, &endptr, 10);	
				break;
			
			case 's':
			
				arguments->sampleSize = (unsigned short int) strtol(optarg, &endptr, 10);
				break;
		}
		
		opt = getopt(arguments->argc, arguments->argv, arguments->optString);
		
	}
}
