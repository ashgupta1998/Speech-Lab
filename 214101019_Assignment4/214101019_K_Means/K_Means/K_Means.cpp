// K_Means.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "iostream"
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define M 6340


using namespace std;
long double codebook[8][12]={0.0},universe[6340][12]={0.0};


void initialize_universe()
{

	//This function stores universe.csv file in universe array 
		cout<<"---------------- Reading Universe ---------------\n";
		FILE *fptr = fopen("universe.csv","r");
		char line[2000];
		int r=0,c=0;																											//r->row , c->column
		while(fgets(line,2000,fptr))
		{
			char *token;
			token = strtok(line , ",");																							//breaking each row of universe.csv row using ',' as delimiter and generating tokens 
			c=0;
			while(token!=NULL)
			{
				universe[r][c] = atof(token);																					//storing token values in universe array 
				c++;
				token = strtok(NULL,",");																						//to continue the process of tokenization
			}
			r++;
		}
		fclose(fptr);
}



void initialize_codebook()
{
	//This function initializes codebook for kmeans with 8 random vectors from universe using rand()  function
		int i=0,j,temp=0;
		cout<<"\n------------ Initializing Codebook --------------\n";
		cout<<"\nCodebook initialized as: \n"<<endl;
		srand(time(0));
		for(i=0;i<8;i++)
			{				
				 temp = rand()%3641;																							//obtaining  random value and using that as the index of universe vector
				 for(j = 0;j<12;j++)
				   {
					   codebook[i][j] = universe[temp][j];																		//copying value of universe[temp] vector in codebook[i]
					   cout<<codebook[i][j]<<" ";
				   }
				 cout<<"\n\n";
		    }
}


long double cal_distortion(int j , int i)
{
	 
		long double dist=0.0;
		int k=0;
		long double w[12] = {1.0,3.0,7.0,13.0,19.0,22.0,25.0,33.0,42.0,50.0,56.0,61.0};
		for(k=0;k<12;k++)
			{
				dist = dist + w[k]*((universe[j][k] - codebook[i][k])*(universe[j][k] - codebook[i][k]));
			}
		return dist;
}

void K_means(int size)
{
	//This function performs K_means algorithm

		long double new_centroid[8][12]={0.0},distortion = 0.0, d = 0.0, avg_distortion=0.0,prev_distortion =0.0;					//array new_centroid is used while updating the centroids
		int Ci[6340] ={0}, Mi[8] = {0};																								//array Ci[j] stores the Cluster to which universe[j] is mapped to ; Mi[j] stores the number of universe vectors mapped to Cluster j							
		bool flag = true;
		int i=0,j=0,m=0;
		cout<<"----------------------Running K_Means Algorithm -------------------------------\n\n";
		while(flag)
		{
			avg_distortion = 0.0;
			for(i=0;i<8;i++)
			{
				Mi[i] = 0;
			}
			for(j=0;j<M;j++)
			{
				distortion = pow(10,9.0);																					    	//setting distortion to some large value
				for(i=0;i<size;i++)
				{
					d = cal_distortion(j,i);																						//calling cal_distortion function to calculate distortion by obtaining tokhura distance between universe[j] vector and codebook[i](cluster i)
					if(d < distortion)
					{
						distortion = d;
						Ci[j] = i;																									//mapping universe[j] to cluster with which distortion is minimum
					}
				}
				avg_distortion += distortion;
				Mi[Ci[j]] += 1;																										//incrementing the number of universe vector mapped to cluster Ci[j]
			}
		
			avg_distortion = avg_distortion/6340 ;
			cout<<"Iteration "<<m+1<<" -> Distortion: "<<avg_distortion<<endl;
			m=m+1;
			if(abs(avg_distortion - prev_distortion)<0.00001)																		
			{
				flag = false;																									   //stop iterating while loop if the difference between previous and current distortion is less than delta(here 0.0001)
			}
			else
			{
				prev_distortion = avg_distortion;
				for(i=0;i<8;i++)
					for(j=0;j<12;j++)
						new_centroid[i][j] = 0.0;
				for(i=0;i<M;i++)
					for(j=0;j<12;j++)
					{
						new_centroid[Ci[i]][j] += universe[i][j];																	//summing up all the universe vector assigned to a particular cluster
					}
				for(i=0;i<8;i++)
					for(j=0;j<12;j++)
					{
						new_centroid[i][j] /= Mi[i];																				//taking average w.r.t the number of universe vectors assigned to the cluster
						codebook[i][j] = new_centroid[i][j];																		//updating codebook
					}
			}
		}
		cout<<"\nFinal distortion is : "<<avg_distortion<<endl;
		cout<<"\n------------------Final Cluster Centroids are-------------------------\n "<<endl;
		for(i=0;i<8;i++)
			{
				cout<<" Centroid "<<i+1<<" :";
				for(j=0;j<12;j++)
					{
						cout<<codebook[i][j]<<" ";
					}
				cout<<"\n\n";
			}
	
}

int _tmain(int argc, _TCHAR* argv[])
{
	int i=0,j=0;
	initialize_universe();																										     //Reading universe 
	initialize_codebook();																									         //Initializing codebook
	K_means(8);																														 //call to K_means with number of clusters = 8
	return 0;
}

