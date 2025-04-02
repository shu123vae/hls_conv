
#include "ImgFilter.h"


/**/

void FilterEXP(INPUT TypeUint14Stream &InImg,
	OUTPUT TypeUint14Stream &OutImg)
{
	uint14 Temp;
	int i, j;
	for (i = 0; i < SIZE_IMG_EXP; i++)
	{ 
		for (j = 0; j < SIZE_IMG_EXP; j++)
		{
			if (
					i >= (SIZE_IMG_EXP - SIZE_IMG) / 2 && i <= (SIZE_IMG_EXP + SIZE_IMG) / 2 - 1
					&& j >= (SIZE_IMG_EXP - SIZE_IMG) / 2 && j <= (SIZE_IMG_EXP + SIZE_IMG) / 2 - 1
				)
			{
				InImg >> Temp;
				OutImg << Temp;
			}
			else
			{
				OutImg << (uint14)0;
			}
		}
	}
	
}


void CacA(
	INPUT uint14 BufBlock[SIZE_EXT][SIZE_EXT],

	OUTPUT float &RawImg,
	OUTPUT float &DeltAvg,
	OUTPUT float &DeltRms)
{
	int i, j;
	uint20 AvgIntTemp = 0;
	uint21 AvgGuardTemp = 0;
	uint21 AvgExtTemp = 0;
	float AvgInt;
	float AvgGuard;
	float AvgExt;
	float RmsInt = 0;
	float RmsExt = 0;
	float AvgSqrtInt;

CacA_label3:
	for (i = (SIZE_EXT - SIZE_INT) / 2; i < (SIZE_EXT + SIZE_INT) / 2; i++)
	{
	CacA_label4:
		for (j = (SIZE_EXT - SIZE_INT) / 2; j < (SIZE_EXT + SIZE_INT) / 2; j++)
		{
			AvgIntTemp = AvgIntTemp + BufBlock[i][j];
		}
	}
	AvgInt = (float)AvgIntTemp / (float)(SIZE_INT * SIZE_INT); //内框平均
	AvgSqrtInt = AvgInt * AvgInt;							   //内框平均平方

CacA_label5:
	for (i = (SIZE_EXT - SIZE_INT) / 2; i < (SIZE_EXT + SIZE_INT) / 2; i++) //内框平方和计算
	{
	CacA_label6:
		for (j = (SIZE_EXT - SIZE_INT) / 2; j < (SIZE_EXT + SIZE_INT) / 2; j++)
		{
			RmsInt = RmsInt + (float)BufBlock[i][j] * (float)BufBlock[i][j];
		}
	}
	RmsInt = RmsInt / (float)(SIZE_INT * SIZE_INT) - AvgSqrtInt; //内框平方和的平均-平均的平方（内框方差）

CacA_label7:
	for (i = (SIZE_EXT - SIZE_GUARD) / 2; i < (SIZE_EXT + SIZE_GUARD) / 2; i++)
	{
	CacA_label8:
		for (j = (SIZE_EXT - SIZE_GUARD) / 2; j < (SIZE_EXT + SIZE_GUARD) / 2; j++)
		{
			if (
				i < (SIZE_EXT - SIZE_INT) / 2 ||
				i >= (SIZE_EXT + SIZE_INT) / 2 ||

				(i >= (SIZE_EXT - SIZE_INT) / 2 &&
				 i < (SIZE_EXT + SIZE_INT) / 2 &&
				 (j < (SIZE_EXT - SIZE_INT) / 2 || j >= (SIZE_EXT + SIZE_INT) / 2)))
			{
				AvgGuardTemp = AvgGuardTemp + BufBlock[i][j];
			}
		}
	}
	AvgGuard = AvgGuardTemp / (float)(SIZE_GUARD * SIZE_GUARD - SIZE_INT * SIZE_INT); //中框求平均

CacA_label9:
	for (i = 0; i < SIZE_EXT; i++)
	{
	CacA_label10:
		for (j = 0; j < SIZE_EXT; j++)
		{
			if (
				i < (SIZE_EXT - SIZE_GUARD) / 2 ||
				i >= (SIZE_EXT + SIZE_GUARD) / 2 ||

				(i >= (SIZE_EXT - SIZE_GUARD) / 2 &&
				 i < (SIZE_EXT + SIZE_GUARD) / 2 &&
				 (j < (SIZE_EXT - SIZE_GUARD) / 2 || j >= (SIZE_EXT + SIZE_GUARD) / 2)))
			{
				AvgExtTemp = AvgExtTemp + BufBlock[i][j];
			}
		}
	}

	AvgExt = (float)AvgExtTemp / (float)(SIZE_EXT * SIZE_EXT - SIZE_GUARD * SIZE_GUARD); //外框求平均
	AvgSqrtInt = AvgExt * AvgExt;														 //外框求平均的平方

CacA_label11:
	for (i = 0; i < SIZE_EXT; i++)
	{
	CacA_label12:
		for (j = 0; j < SIZE_EXT; j++)
		{
			if (
				i < (SIZE_EXT - SIZE_GUARD) / 2 ||
				i >= (SIZE_EXT + SIZE_GUARD) / 2 ||

				(i >= (SIZE_EXT - SIZE_GUARD) / 2 &&
				 i < (SIZE_EXT + SIZE_GUARD) / 2 &&
				 (j < (SIZE_EXT - SIZE_GUARD) / 2 || j >= (SIZE_EXT + SIZE_GUARD) / 2)))
			{
				RmsExt = RmsExt + (float)BufBlock[i][j] * (float)BufBlock[i][j];
			}
		}
	}
	RmsExt = RmsExt / (float)(SIZE_EXT * SIZE_EXT - SIZE_GUARD * SIZE_GUARD) - AvgSqrtInt; //外框方差

	RawImg = (float)BufBlock[(SIZE_EXT - 1) / 2][(SIZE_EXT - 1) / 2]; //中心点灰度
	DeltAvg = (float)(AvgInt - AvgGuard);							  //内框平均-中框平均
	DeltRms = (RmsInt - RmsExt);									  //内框方差-外框方差
}


void FilterLayerA(
	INPUT TypeUint14Stream &InImg,

	OUTPUT TypeFloatStream &StreamRawImg,
	OUTPUT TypeFloatStream &StreamDeltAvg,
	OUTPUT TypeFloatStream &StreamDeltRms)
{
	int i, j;
	int BufLineI = 0;
	int BufBlockJ = 0;
	int k, l;
	int m;

	uint14 BufLine[SIZE_EXT][SIZE_IMG];
	uint14 BufBlock[SIZE_EXT][SIZE_EXT];
	uint14 Temp;

	float RawImg;
	float DeltAvg;
	float DeltRms;

	for (i = 0; i < SIZE_IMG_EXP; i++)
	{
	FilterLayerA_label100:
		for (j = 0; j < SIZE_IMG_EXP; j++)
		{
			InImg >> Temp; 
			BufLine[BufLineI][j] = Temp;

			if (i >= SIZE_EXT - 1)
			{
			FilterLayerA_label1:
				for (k = 0; k < SIZE_EXT; k++)
				{
				FilterLayerA_label2:
					for (l = SIZE_EXT - 1; l >= 1; l--)
					{
						BufBlock[k][l] = BufBlock[k][l - 1];
					}
				}

				if (BufLineI == SIZE_EXT - 1)
				{
					m = 0;
				}
				else
				{
					m = BufLineI + 1;
				}

			FilterLayerA_label0:
				for (k = 0; k < SIZE_EXT - 1; k++)
				{
					BufBlock[k][0] = BufLine[m][j];

					if (m == SIZE_EXT - 1)
					{
						m = 0;
					}
					else
					{
						m++;
					}
				}
				BufBlock[SIZE_EXT - 1][0] = Temp;

				if (j >= SIZE_EXT - 1)
				{
					CacA(BufBlock, RawImg, DeltAvg, DeltRms);
					StreamDeltRms << DeltRms;
				}
			}

			if (
				i >= SIZE_EXT - 1 + (SIZE_INT - 1) / 2 &&
				i <= SIZE_IMG_EXP - 1 - (SIZE_INT - 1) / 2 &&
				j >= SIZE_EXT - 1 + (SIZE_INT - 1) / 2 &&
				j <= SIZE_IMG_EXP - 1 - (SIZE_INT - 1) / 2)
			{
				StreamRawImg << RawImg;
				StreamDeltAvg << DeltAvg;
			}

			if (j == SIZE_IMG_EXP - 1)
			{
				if (BufLineI == SIZE_EXT - 1)
				{
					BufLineI = 0;
				}
				else
				{
					BufLineI++;
				}
			}
		}
	}
}

void CacB(
	INPUT float &RawImg,
	INPUT float &DeltAvg,
	INPUT float BlockDeltRms[SIZE_INT][SIZE_INT],
	//INPUT	TypeD09MN &D09MN,

	//OUTPUT TypeD09MNStream &StreamD09MN,

	OUTPUT float &Res)
{
	int i, j;
	float MVARD = 0;

CacB_label13:
	for (i = 0; i < SIZE_INT; i++)
	{
	CacB_label14:
		for (j = 0; j < SIZE_INT; j++)
		{
			if (i != (SIZE_INT - 1) / 2 && j != (SIZE_INT - 1) / 2)
				MVARD = MVARD + BlockDeltRms[i][j];
		}
	}

	MVARD = MVARD / (float)(SIZE_INT * SIZE_INT - 1);

	if (MVARD > 0 && DeltAvg > 0)

		Res = RawImg * MVARD * MVARD * DeltAvg * DeltAvg;

	else
		Res = 0.0;

	//if (DeltAvg > 0)

	//	Res = DeltAvg;

	//else
	//	Res = 0.0;

	//Res = DeltAvg;

	//if (Tem>TH_ALPHA)
	//	StreamD09MN<<D09MN;
}

void FilterLayerB(
	INPUT TypeFloatStream &StreamRawImg,
	INPUT TypeFloatStream &StreamDeltAvg,
	INPUT TypeFloatStream &StreamDeltRms,

	//OUTPUT TypeD09MNStream &StreamD09MN,
	OUTPUT TypeFloatStream &StreamCluSup

)
{
	int i, j;
	int BufLineI = 0;
	int BufBlockJ = 0;
	int k, l;
	int m;
	float BufLine[SIZE_INT][SIZE_IMG_EXP - SIZE_EXT + 1];
	float BufBlock[SIZE_INT][SIZE_INT];
	float StreamRawImgTemp;
	float StreamDeltAvgTemp;
	float StreamDeltRmsTemp;
//	TypeD09MN D09MN;

	float Res;

	for (i = 0; i < SIZE_IMG_EXP - SIZE_EXT + 1; i++)
	{

	FilterLayerB_label1:
		for (j = 0; j < SIZE_IMG_EXP - SIZE_EXT + 1; j++)
		{

			StreamDeltRms >> StreamDeltRmsTemp;
			BufLine[BufLineI][j] = StreamDeltRmsTemp;
//			D09MN.M = i;
//			D09MN.N = j;

			if (i >= SIZE_INT - 1)
			{

			FilterLayerB_label16:
				for (k = 0; k < SIZE_INT; k++)
				{
				FilterLayerB_label17:
					for (l = SIZE_INT - 1; l >= 1; l--)
					{
						BufBlock[k][l] = BufBlock[k][l - 1];
					}
				}

				if (BufLineI == SIZE_INT - 1)
				{
					m = 0;
				}
				else
				{
					m = BufLineI + 1;
				}

			FilterLayerB_label15:
				for (k = 0; k < SIZE_INT - 1; k++)
				{
					BufBlock[k][0] = BufLine[m][j];

					if (m == SIZE_INT - 1)
					{
						m = 0;
					}
					else
					{
						m++;
					}
				}
				BufBlock[SIZE_INT - 1][0] = StreamDeltRmsTemp;
				if (j >= SIZE_INT - 1)
				{
					StreamRawImg >> StreamRawImgTemp;
					StreamDeltAvg >> StreamDeltAvgTemp;
					CacB(StreamRawImgTemp, StreamDeltAvgTemp, BufBlock, /*D09MN, StreamD09MN, */ Res);
					StreamCluSup << Res;
				}
			}

			if (j == SIZE_IMG_EXP - SIZE_EXT)
			{
				if (BufLineI == SIZE_INT - 1)
				{
					BufLineI = 0;
				}
				else
				{
					BufLineI++;
				}
			}
		}
	}
}

void Filter512(
	INPUT TypeUint14Stream &InImg,
	//OUTPUT TypeD09MNStream &StreamD09MN,
	OUTPUT TypeFloatStream &StreamCluSup)
{

	static TypeFloatStream StreamRawImg;
	static TypeFloatStream StreamDeltAvg;
	static TypeFloatStream StreamDeltRms;
	static TypeUint14Stream ImgExp;

	FilterEXP(INPUT InImg, OUTPUT ImgExp);

	FilterLayerA(
		INPUT ImgExp,

		OUTPUT StreamRawImg,
		OUTPUT StreamDeltAvg,
		OUTPUT StreamDeltRms);
	//RawImgBufSize=SIZE_IMG*((SIZE_INT-1)/2)+SIZE_INT
	FilterLayerB(
		INPUT StreamRawImg,
		INPUT StreamDeltAvg,
		INPUT StreamDeltRms,

		//OUTPUT StreamD09MN,
		OUTPUT StreamCluSup);
}


void myTest(INPUT TypeFloatStream& InImg, OUTPUT TypeFloatStream& OutImg)
{
	int i, j, k;
	int BufLineI = MYTEST_SIZE_EDGRFILL;
	float BufLine[MYTEST_SIZE][SIZE_IMG];
	float temp;

	for ( i = 0; i < SIZE_IMG + MYTEST_SIZE_EDGRFILL; i++)
	{
		for ( j = 0; j < SIZE_IMG + MYTEST_SIZE_EDGRFILL; j++)
		{
			if (i<SIZE_IMG && j<SIZE_IMG)
			{
				InImg >> temp;
			}
			else
			{
				temp = 0;
			}

			if (i < MYTEST_SIZE_EDGRFILL)
			{
				if (j < MYTEST_SIZE_EDGRFILL)
				{
					BufLine[BufLineI][j] = temp;
				}
			}
			else
			{
				if (i<SIZE_IMG && j<SIZE_IMG)
				{
					BufLine[BufLineI][j] = temp;
				}
			}
			if (j == SIZE_IMG + MYTEST_SIZE_EDGRFILL - 1)
			{
				if (i < SIZE_IMG)
				{
					if (BufLineI == MYTEST_SIZE - 1)
					{
						BufLineI = 0;
					}
					else
					{
						BufLineI++;
					}
				}
			}

		}
	}
}
