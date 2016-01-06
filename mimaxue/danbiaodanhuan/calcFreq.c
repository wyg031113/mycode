#include<stdio.h>
#include<stdlib.h>

char buf[1024*1024];
int freq[27];
int len = 0;
char *outfile = NULL;
void calcFreq()
{
	int i, j;
	char tc[] = "abcdefghijklmnopqrstuvwxyz";
	FILE *ffreq = fopen(outfile, "w");
	if(ffreq == NULL)
	{
		printf("build %s failed\n", outfile);
		exit(-1);
	}
	for(i = 0; i < len; i++)
		if(buf[i] >= 'a' && buf[i] <= 'z')
			freq[buf[i] - 'a']++;
		
	for(i = 0; i < 26; i++)
		for(j = i + 1; j < 26; j++)
			if(freq[i] < freq[j])
			{
				int t = freq[i]; freq[i] = freq[j];	freq[j] = t;
				
				char c = tc[i];	tc[i] = tc[j]; tc[j] = c;
			}
	printf("alph	freq\n");
	for(i = 0; i < 26; i++)
		fprintf(ffreq,"%c   	%d\n", tc[i], freq[i]);
	fclose(ffreq);

}

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		printf("run like this: ./caclFreq inputfile.txt outputfile.txt\n"
				"we will calc every alph's freq from inputfile, and write freq to outputfile\n");
		exit(-1);
	}
	FILE *fp = fopen(argv[1], "r");
	outfile = argv[2];
	//FILE *fw = fopen("exfreq.txt", "r");
	if(fp == NULL)
	{
		printf("Please give me a example.txt, it must be plain txt\n");
		exit(-1);
	}
	
	char c;
	int freq[27];
	while(len <1024*1024 && (buf[len++] = fgetc(fp)) != EOF)
		if(buf[len - 1] >= 'A' && buf[len - 1] <= 'Z')
			buf[len - 1] += 32;
	calcFreq();
}
