#include<stdio.h>

int main()
{
	char key[128];
	char c;
	FILE *fkey = fopen("key.txt","r");
	FILE *fplain = fopen("plain.txt", "r");
	FILE *fcipher = fopen("cipher.txt", "w");
	if(fkey == NULL)
	{
		printf("no key file, please generate key file\n");
		return -1;
	}
	if(fplain == NULL)
	{
		printf("no plain.txt\n");
		return -1;
	}
	fscanf(fkey,"%26s", key);
	fclose(fkey);
	//printf("key: %s\n", key);
	while((c=fgetc(fplain)) != EOF)
	{
		if(c>='A' && c<='Z')
			c += 32;
		if(c >= 'a' && c <= 'z')
			c = key[c-'a'];
		fputc(c, fcipher);
	}
	fclose(fplain);
	fclose(fcipher);
	return 0;
}
