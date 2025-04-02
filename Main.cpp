#include "ImgFilter.h"
#include <stdio.h>
int main()
{
	TypeFloatStream StreamCluSup;
	TypeUint14Stream InImg;

	float Res[492][492];
	for(int i=0;i<512*512;i++)
	{
		InImg<<1;
	}
	Filter512(InImg, StreamCluSup);

	FILE *fp;
	fp=fopen("result.dat","wb+");

	for(int i=0;i<492;i++)
	{
		for(int j=0;j<492;j++)
		{
			StreamCluSup>>Res[i][j];
			fwrite(&Res[i][j],sizeof(float),1,fp);
		}
	}

	fclose(fp);

	return 0;
}

