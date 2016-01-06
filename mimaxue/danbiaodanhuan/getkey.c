#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{	
	if(argc < 3)
	{
		printf("run like this: ./getkey cipher's freq.txt  exfreq.txt\n");
		exit(-1);
	}
	char a[3],b[3];
	int x, y;
	FILE *ffreq = fopen(argv[1], "r");
	FILE *fexfreq = fopen(argv[2], "r");
	FILE *fkey = fopen("mykey.txt", "w");
	if(ffreq == NULL || fexfreq == NULL || fkey == NULL)
	{
		printf(" %s or %s or mykey.txt not exists.\n");
		return -1;
	}
	char key[27];
	int i;
	for(i = 0; i < 26; i++)
	{
		fscanf(ffreq, "%3s%d", a, &x);
		fscanf(fexfreq, "%3s%d", b, &y);
		key[a[0]-'a'] = b[0];	
	}
	key[26] = '\0';
	fprintf(fkey, "abcdefghijklmnopqrstuvwxyz\n%s\n",key);
	fclose(ffreq);
	fclose(fexfreq);
	fclose(fkey);
}
