// LBG.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "math.h"
#include "stdlib.h"
#include "iostream"
using namespace std;

#define M 6340

long double universe[6340][12]={0.0};
long double codebook[8][12]={0.0};



void initialize_universe()
{
	//This function stores universe.csv file in universe array
		cout<<"----------------------------------------------------- Reading Universe --------------------------------------------------------------------------\n\n";
		FILE *fptr = fopen("universe.csv","r");
		char line[2000];
		int r=0,c=0;																							//r->row , c->column
		while(fgets(line,2000,fptr))
		{
			char *token;
			token = strtok(line , ",");																			//breaking each row of universe.csv row using ',' as delimiter and generating tokens
			c=0;
			while(token!=NULL)
			{
				universe[r][c] = atof(token);																	//storing token values in universe array
				c++;
				token = strtok(NULL,",");																		//to continue the process of tokenization
			}
			r++;
		}
		fclose(fptr);
}


long double cal_distortion(int j , int i)
{
	//This function calculates the distance between universe[j] vector and centroid[i] using tokhura distance as the distance metric
		long double dist=0.0;
		int k=0;
		double w[12] = {1.0,3.0,7.0,13.0,19.0,22.0,25.0,33.0,42.0,50.0,56.0,61.0};
		for(k=0;k<12;k++)
			{
				dist = dist + w[k]*(universe[j][k] - codebook[i][k])*(universe[j][k] - codebook[i][k]);
			}
		return dist;
}

long double K_means(int size)
{
	//This function performs K_means algorithm
		long double new_centroid[8][12]={0.0},distortion = 0.0, d = 0.0, avg_distortion=0.0,prev_distortion =0.0;	//array new_centroid is used while updating the centroids
		int Ci[6340] ={0}, Mi[8] = {0};																				//array Ci[j] stores the Cluster to which universe[j] is mapped to ; Mi[j] stores the number of universe vectors mapped to Cluster j					
		bool flag = true;
		int i=0,j=0,m=0;

		while(flag)
		{
			avg_distortion = 0.0;
			for(i=0;i<8;i++)
			{
				Mi[i] = 0;
			}
			for(j=0;j<M;j++)
			{
				distortion = pow(10,9.0);																			//setting distortion to some large value
				for(i=0;i<size;i++)
				{
					d = cal_distortion(j,i);																		//calling cal_distortion function to calculate distortion by obtaining tokhura distance between universe[j] vector and codebook[i](cluster i)
					if(d < distortion)
					{
						distortion = d;
						Ci[j] = i;																					//mapping universe[j] to cluster with which distortion is minimum
					}
				}
				avg_distortion += distortion;																		//incrementing the number of universe vector mapped to cluster Ci[j]
				Mi[Ci[j]] += 1;
			}
			avg_distortion /= M;
			m=m+1;
			if(abs(avg_distortion - prev_distortion)<0.00001)														//stop iterating while loop if the difference between previous and current distortion is less than delta(here 0.0001)
			{
				flag = false;
			}
			else{
				prev_distortion = avg_distortion;
				cout<<avg_distortion<<endl;
				for(i=0;i<8;i++)
					for(j=0;j<12;j++)
						new_centroid[i][j] = 0.0;
				for(i=0;i<M;i++)
					for(j=0;j<12;j++)
						new_centroid[Ci[i]][j] += universe[i][j];													//summing up all the universe vector assigned to a particular cluster
				for(i=0;i<8;i++)
					for(j=0;j<12;j++)
					{
						new_centroid[i][j] /= Mi[i];																//taking average w.r.t the number of universe vectors assigned to the cluster
						codebook[i][j] = new_centroid[i][j];														//updating codebook
					}
			}
		}
		
		return avg_distortion;
}


void LBG(long double epsilon, int Max_Size)
{
	
		int size = 1,i=0,j=0;
		for(i=0;i<M;i++)
		 {
			for(j=0;j<12;j++)
			{
				codebook[0][j] += universe[i][j];																		//summing up all the universe vectors as a single cluster
			}
		 }
		for(j=0;j<12;j++)
			{
				codebook[0][j] /=M;																						//taking average w.r.t universe t obtain global centroid
			}
		cout<<"Codebook Size = "<<size<<" , ";
		while(size < Max_Size)																							//iterating till codebook size < 8
		 {
			cout<<"Codebook is : \n\n";
			for(i=0;i<size;i++)
			  {
				for(j=0;j<12;j++)																													
				 {																										//printing the codebook
					cout<<codebook[i][j]<<" "; 																					
				 }
				cout<<"\n\n";
		      }
			cout<<"------------------------------------------------------------------------------------------------------------------------------------------------------";
			for(i=0;i<size;i++)
			  {
				for(j=0;j<12;j++)																						//splitting the codebook vectors
				 {
					codebook[size+i][j] = codebook[i][j]*(1-epsilon);													//obatining y- using the formula y- = y*(1-epsilon)
					codebook[i][j] = codebook[i][j]*(1+epsilon);														//obatining y+ using the formula y+ = y*(1+epsilon)
				 }
		      }
			size = 2*size;																								//doubling the size codebook
			long double distortion = K_means(size);
			cout<<"\n\nCodebook Size = "<<size<<" , Distortion :"<<distortion<<"\n\n"; 
		}
		cout<<"Final Cluster Centroids are: \n"<<endl;
		for(i=0;i<8;i++)
			{
				cout<<"Centroid "<<i<<" : ";
				for(j=0;j<12;j++)
					{
						cout<<codebook[i][j]<<" ";
					}
				cout<<"\n\n";;
			}
		cout<<"------------------------------------------------------------------------------------------------------------------------------------------------------------";
}

int _tmain(int argc, _TCHAR* argv[])
{
	initialize_universe();																		//Reading universe 
	LBG(0.03,8);																				//Calling LBG function and passing epsilon(here0.03) and Max codebook size(here 8) as arguments	
	
	return 0;
}

