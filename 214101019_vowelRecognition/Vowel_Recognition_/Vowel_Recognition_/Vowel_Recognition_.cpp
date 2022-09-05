// Voice_Recognition.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "iostream"
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <iomanip>


#define p 12
#define N 320

using namespace std;

long double rough[1600]={0};






/**********************Normalization*************************************/



int normalize_max(const char *filename,int num)
{
	//Welcome to normalize_max
	//This function calculates the max value of sample i.e. max amplitude of the waveform 

		FILE *fpointer = fopen(filename,"r");
		long double  temp=0,max=INT_MIN;
		char line[30] ="";
		int i=0;
		for(i=1;i<7;i++);
			fgets(line,30,fpointer);									//discard initial frames (as it is header)
		while(num!=0)
			{
				fgets(line,30,fpointer);
				sscanf(line,"%lf",&temp);
				if(abs(temp)>max)										//comparing scanned value with max value
					{
						max = abs(temp);								//updating max value
					}
				num = num -1;
			 }
		fclose(fpointer);
	
		return max;														//returns max value
}



int Calculate_dc()
{

	FILE *fpointer = fopen("ambient.txt","r");							 //ambient.txt is the ambience of the room when recording was done
	char line[30]=" ",ch=' ';
	int temp = 0,dc =0,num=0,i=0;
	while((ch=fgetc(fpointer))!=EOF)									 //calculating number of samples
		{
			if(ch=='\n')
				num++;
		}
	num = num - 5;
	fseek(fpointer,0,SEEK_SET);
	for(i=1;i<=5;i++)
		fgets(line,30,fpointer);										 //scanning first sample 
	sscanf(line, "%d", &temp);											 //storing first sample value in temp0
	dc=temp;
	for(i=2;i<=num;i++)
		{

			fgets(line,30,fpointer);
			sscanf(line, "%d", &temp);							
			dc = dc + temp;												//adding subsequent sample values to dc
																					
		}
		dc = (dc*320)/num;												//obtaining dc value per frame i.e.per 320 samples
		fclose(fpointer);	
		return dc;								



}

void Calculate_normalize(char const *filename)
{

	//This function normalizes the sample values and stores 1600 (320 * 5) samples in array rough[1600] 
	
	FILE *fpointer = fopen(filename,"r");
	int num=0,i=0;
	double max_nf;
	char line[30] = " ",ch =' ';
	int dc = Calculate_dc();
    fseek(fpointer,0,SEEK_SET);											  //moving back to start

	 while((ch=fgetc(fpointer))!=EOF)									  //calculating number of samples in the file
		{
			if(ch=='\n')
				num++;
		}
	num = num - 5;

	 max_nf = normalize_max(filename,num);
	 fseek(fpointer,0,SEEK_SET);
	 for(i=1;i<=5350;i++)
		fgets(line,30,fpointer);										   //discarding some initial samples

	 double nf = 5000/max_nf;
	  
	 int temp0=0;
	 for(i=0;i<1600;i++)
		{
			fgets(line,30,fpointer);
			sscanf(line, "%d", &temp0);
			temp0 = (temp0-dc)*nf;											//normalizing data
			rough[i] = temp0;												//storing normalised sample values in array rough[1600]
		 
		 }
	 fclose(fpointer);

		
}



/******************************Calculating Ci's*****************************************/



void Calculate_Ri(long double *R, const char *filename)	
{	

	//This function calculates Ri's for the frame stored in file filename and stores the corresponding Ri's in array R

	FILE *fptr = fopen(filename,"r");										//fptr is file pointer of file containing 1 frame i.e.320 normalised sample values
	long double sample[320]={0.0},temp=0;
	int i=0,j=0;
	char line[30] = "";
	for(i=0;i<N;i++)														//storing 320 normalised sample values in array sample
		{
			fgets(line,30,fptr);
			sscanf_s(line, "%lf",&temp);								
			sample[i] = temp;												

		}
	for(i=0;i<=p;i++)
		{
			for(j=0;j<=N-1-i;j++)
			{
				R[i] = R[i] + sample[j]*sample[j+i];						//Calculating R using correlation formula
			}
			
		}
	fclose(fptr);
}

void Calculate_Ai(long double *A,long double *R)
{

	//This function calulates the value of Ai coefficients and stores corresponding Ai's in array A
	
	int i=0,j=0;
	
	long double E=R[0],k=0;													//k is the reflection coefficient, E is initialised with R[0]= energy
	long double B[13];														//to store the value of A in (i-1)th iteration 
	
	for(i=0;i<=12;i++)
	 {
		B[i] = 0;
	 }
	long double sum=0;
	
	for(i=1;i<=p;i++)
	 {
		sum = 0;
		for(j=1;j<=i-1;j++)
		  {
			 sum = sum + B[j]*R[i-j];
		  }
		 k = (R[i] - sum)/E;
		 A[i] = k;
		 for(j=1;j<=i-1;j++)
		  {	
			  A[j] = B[j] - k*B[i-j];									     //updating Ai's using Bi's 
		  }
		 for(j=1;j<=i;j++)
		  {
			  B[j] = A[j] ;
		  }
		 E = (1-k*k)*E;
	 }	
}
void Calculate_Ci(const char *filename,double *rough_c)
{
  //file name contains the name of the sample file whose Ci values needs to be calculated

	long double Ri[13],Ai[13],Ci[13],Lifter[13];						//Array Ri to store Ri's, Ai to store Ai's, Ci to store Ci's,Lifter to store tampered window weights
	int i=0,k=0;
	long double sum=0;
	for(i=0;i<=12;i++)
	 {
		Ri[i] = 0.00000;
		Ai[i] = 0.00000;
		Ci[i] = 0;
	 }

	Calculate_Ri(Ri,filename);
	Calculate_Ai(Ai,Ri);

	Ci[0] = log10(Ri[0]*Ri[0]);
	for(i=1;i<13;i++)
	 {
		sum = 0;
		for(k=1;k<=i-1;k++)
		  {
			
			 sum = sum + ((long double)k*Ci[k]*Ai[i-k])/(long double)i;			
		  }
		Ci[i] = Ai[i] + sum;											//Calculating Ci's using cepstral coefficient formula
	 }
	
	Lifter[0] = 1;
	for(i=1;i<=p;i++)
	  {
		 Lifter[i] = 1 + 6*sin((3.14*i)/12);							//calculating tampered window values Q=12
	  }
	int j =0;
	for(j = 0;j<12;j++)
	  {
			rough_c[i] =0;
	  }
	for(i=1;i<=p;i++)
	  {
		Ci[i]*= Lifter[i];												//Multiplying Ci's with tampered window weights
	  }
	
	for(i=0;i<12;i++)
	  {
		rough_c[i] = Ci[i+1];											//storing Ci's in rough_c matrix
	  }		
}






/*******************************Generating Reference files*****************************************/



void Calculate_refvowel(const char *filename,long double ref_mat[][12])
{
	//This function performs normalization,DCshift,stores steady sample values(320 samples) in ref_rough.txt file
	
	Calculate_normalize(filename);										  //1600 samples(320*5) will be stored in array rough
	
	int i=0,m=0,j=0;
	for(i=0;i<5;i++)
	{
		FILE *fptr = fopen("ref_rough.txt","w");
		for(j=m;j<m+320;j++)
		  {
				fprintf(fptr,"%lf\n",rough[j]);								//writes 320 sample values in ref_rough file
		  }
		fclose(fptr);
		double rough_c[12]={0};
		Calculate_Ci("ref_rough.txt",rough_c);
		for(j=0;j<12;j++)
		  {
			 ref_mat[i][j] += rough_c[j];
		  }
		m = m + 320;		
	}	
}

void Vowel_Reference(char s[],long double (*ref_mat)[12])
{
	//This function generates the reference matrix (5 x 12) and writes it in a file(like 214101019_a_reference.txt,..) 
	//s[] contains the prefix of the file(whose reference file needs to be calculated) like  214101019_a_,214101019_e_,214101019_i_,..
	
	char name[60] = "";
	const char *filename= {0};
	int i=1,j=0;
	for(;i<=10;i++)
	  {
		 sprintf(name,"%s%d.txt",s,i);
		 filename = name;
		 cout<<"Processing file "<<name<<endl;
		 Calculate_refvowel(filename,ref_mat);
	  }
	char fname[30]=" ";
	sprintf(fname,"%s%s.txt",s,"reference");
	FILE *fptr = fopen(fname,"w");
	for(i=0;i<5;i++)
		{
			for(j=0;j<12;j++)
			  {
				 ref_mat[i][j]/=10;													//taking average of each entry w.r.t 10 files  
				 fprintf_s(fptr,"%lf\t",ref_mat[i][j]);								//storing values in corresponding reference file
			  }
			fprintf(fptr,"\n");
	    }
	fclose(fptr);
				
}




/***********************Testing *****************************************/



void Test_refvowel(const char *filename,long double test_mat[][12])
{
	//perform normalization,DCshift,discarding initial samples
	
	Calculate_normalize(filename);						                   //1600 samples(320*5) will be stored in rough
	int i=0,m=0,j=0;
	
	for(i=0;i<5;i++)
	{
		FILE *fptr = fopen("test_ref_rough.txt","w");
		for(j=m;j<m+320;j++)
		{
				fprintf(fptr,"%lf\n",rough[j]);								//writes 320 normalised sample values in tst_ref_rough file
		}																	
		fclose(fptr);
		double rough_c[12]={0};
		
		Calculate_Ci("test_ref_rough.txt",rough_c);
		
		for(j=0;j<12;j++)
		{
			test_mat[i][j] = rough_c[j];									//storing Ci's corresponding to each steady frame as a row in test_mat matrix
		}
		m = m + 320;
			
	}
	
}

void Tokura_Distance(long double test_mat[][12])
{
	double w[12] = {1.0,3.0,7.0,13.0,19.0,22.0,25.0,33.0,42.0,50.0,56.0,61.0};
	long double distance[5][12]={0.0};
	long double reference[5][12]={0.0};									   //store the value of reference file
	long double dis_avg[5]={0.0},dis_min[5],temp=0;						   //dis_avg will be used to calculate the average of each row for a vowel, array dis_min stores the distance of testing file with each vowel 
	int i=0,j=0,k=0;
	char vowels[5] = {'a','e','i','o','u'},ch=' ';
	FILE *fpointer;
	
	for(k=0;k<5;k++)
		{
			char name[100] = "214101019_",line[9] ="";
			const char * fname = {0};
			sprintf(name,"%s%c%s",name,vowels[k],"_reference.txt");
			fname = name;
			fpointer = fopen(fname,"r");
			for(i=0;i<5;i++)
			 {
				for(j=0;j<12;j++)
				  {
					 fscanf(fpointer,"%lf",&reference[i][j]);				//storing the reference value of a vowel in reference matrix (5x12); which is stored in corresponding reference file  
				  }
			}
			fclose(fpointer);

			for(i=0;i<5;i++)
			 {
				for(j=0;j<12;j++)
				 {
					 distance[i][j] = w[j]*(test_mat[i][j] - reference[i][j])*(test_mat[i][j] - reference[i][j]);
					 dis_avg[i] += distance[i][j];
				 }
				dis_avg[i] /= 12;											//taking average of each ith row and storing it at index i of dis_avg
			 }
			dis_min[k] = (dis_avg[0]);
			for(i=1;i<5;i++)
			  {
				 dis_min[k] = min(dis_min[k] , dis_avg[i]);					//soring the minimum of all 5 in dis_min[k]; k = 0 for tokura distance from a,k = 1 for tokura distance from e, k = 2 for i,...
			  }
		}
	 cout<<"\nDistances obtained are:  ";
	 double min = dis_min[0];
	 int index=0;
	
	 for(j=0;j<5;j++)
	   {
		 if(min>dis_min[j])
		  {
			 min = dis_min[j];												//finding minimum distance for predicition
			 index = j;
		  }
		 cout<<dis_min[j]<<"  ";
	  }
	cout<<"\n$Predicted vowel is : "<<vowels[index]<<endl;
	cout<<"---------------------------------------------------------------------------"<<endl;
}



void Vowel_Prediction(char s[])
{
  //This function makes prediction for 10 files of a parituclar vowel
	
	char name[60] = "";
	const char *filename= {0};
	int i=11,j=0;
	long double test_mat[5][12];
	for(;i<=20;i++)
	 {
		sprintf(name,"%s%d.txt",s,i);
		filename = name;
		cout<<"\nPredicting file "<<name<<endl;
		Test_refvowel(filename,test_mat);							//it will generate fill the test_mat with Ci's
		Tokura_Distance(test_mat);									//calculating Tokura distance and making predictions

	}
}

	


/******************************main function*************************************/



int _tmain(int argc, _TCHAR* argv[])
{
	long double ref_A[5][12],ref_E[5][12],ref_I[5][12],ref_O[5][12],ref_U[5][12];
	int i=0,j=0;
	for(i=0;i<5;i++)
	{
		for(j=0;j<12;j++)
		{
			ref_A[i][j] = 0.0;
			ref_E[i][j] = 0.0;
			ref_I[i][j] = 0.0;
			ref_O[i][j] = 0.0;
			ref_U[i][j] = 0.0;
		}
	}

	
	cout<<"*****Genearting reference files*****"<<endl;
	Vowel_Reference("214101019_a_",ref_A);
    Vowel_Reference("214101019_e_",ref_E);
	Vowel_Reference("214101019_i_",ref_I);
	Vowel_Reference("214101019_o_",ref_O);
	Vowel_Reference("214101019_u_",ref_U);
	cout<<"\n*****Predicting vowels*****"<<endl;
	Vowel_Prediction("214101019_a_");
    Vowel_Prediction("214101019_e_");
	Vowel_Prediction("214101019_i_");
	Vowel_Prediction("214101019_o_");
	Vowel_Prediction("214101019_u_");
	return 0;
}


