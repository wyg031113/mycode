#include<stdio.h>
#include<stdlib.h>

char key[27] ="abcdefghijklmnopqrstuvwxyz";
void setKey()
{
	FILE *fkey = fopen("mykey.txt", "r");
	if(fkey == NULL)
	{
		fkey = fopen("mykey.txt", "w");	
		fprintf(fkey,"abcdefghijklmnopqrstuvwxyz\n");
		fprintf(fkey,"abcdefghijklmnopqrstuvwxyz\n");
		fclose(fkey);
		fkey = fopen("mykey.txt", "r");
		
	}
	fscanf(fkey, "%s", key);
	fscanf(fkey, "%s", key);
	fclose(fkey);
}
char buf[1024*1024];
int freq[27];
int len = 0;

void calcFreq()
{
	int i, j;
	char tc[] = "abcdefghijklmnopqrstuvwxyz";
	FILE *ffreq = fopen("freq.txt", "w");
	if(ffreq == NULL)
	{
		printf("build freq.txt failed\n");
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
void decrypt(FILE *f)
{	
	int i;
	for(i = 0; i < len; i++)
	{
		char c = buf[i];
		if(c >= 'a' && c <= 'z')
			c = key[c - 'a'];
		fputc(c, f);
		putchar(c);
	}	
	putchar('\n');
}
int main()
{
	FILE *fcipher = fopen("cipher.txt", "r");
	FILE *fdecrypt = fopen("fdecrypt.txt", "w");
	if(fcipher == NULL)
	{
		printf("No file cipher.txt\n");
		return -1;
	}
	setKey();

	char c;
	while((buf[len++] = fgetc(fcipher)) != EOF)
		;
	//calcFreq();
	decrypt(fdecrypt);
	fclose(fcipher);
	fclose(fdecrypt);
	return 0;
}
