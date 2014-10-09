#include <stdio.h>
#include <stdlib.h>

int randLimited(int limit)
{
	return rand() % limit;
}

int main()
{
	int i = 0;
	int numOfRandomNumbers = 1000000;
	int limit = 1000;

	FILE* pFile;
  pFile = fopen("/tmp/randomNumbers.txt","w");

	for (i=0; i<numOfRandomNumbers; i++)
	{
  	if (pFile!=NULL)
  	{
    	fprintf(pFile, "%d\n", randLimited(limit));
  	}
	}
  fclose(pFile);

	return 0;
}
