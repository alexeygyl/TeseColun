#include "main.h"


void help(void);

void readAttr(int _argc, char ** _argv){
	int rez;
	args.port = 9999;	//Default
	args.mode = 0;		//Default is not deamon
	args.channel = 0;   //Default playing sin Wave;
	strcpy(args.ip,"192.168.1.200");
	while((rez=getopt(_argc,_argv,ARG))!=-1){
		switch(rez){
			case 'p':	
				sscanf(optarg,"%" SCNd16,&args.port);
			break;
			case 'd':	
				sscanf(optarg,"%s",args.ip);
			break;
			case 'D':
				args.mode = 1;
			break;
			case 'L':
				args.channel = 1;
			break;
			case 'R':
				args.channel = 2 ;
			break;
			case 'S':
				args.channel = 4;
			break;
			case 'h':
				help();
				exit(0);
			break;
			default:
			break;

		}

	}

}

void help(void){
	printf("Usage:\n");
	printf("  binary [OPTIONS]\n");
	printf("\n\n");
	printf("[OPTIONS]\n");
	printf("  -D <mode>\t\texecute program in deamon mode, no Deamon is default\n");
	printf("  -p <port>\t\tport of snmp agent, 9999 is default\n");
	printf("  \n");
	printf("Exemple: binary -D -p 6666\n");

}
