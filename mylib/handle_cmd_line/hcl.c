#include <stdio.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
	int ch;
	opterr=0;
	while((ch = getopt(argc, argv,"h:f::cde"))!=-1)
	{
		printf("optind: %d\n", optind);
		printf("optarg:	 %s\n", optarg);
		printf("ch:%c\n", ch);
		switch(ch)
		{
			case 'h':
				printf("option h:'%s'\n", optarg);
				break;
			case 'f':
				printf("option f:'%s'\n", optarg);
				break;
			case 'c':
				printf("option c\n");
				break;
			case 'd':
				printf("option d\n");
				break;
			case 'e':
				printf("option e\n");
				break;
			default:
				printf("other option:%c\n", ch);	
		}
		printf("optopt+%c\n",optopt);
	}
	
	printf("handle command line end!\n");
	return 0;
}
