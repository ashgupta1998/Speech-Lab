							Assignemnt 4 
														Roll No. 214101019
-------------------------------------------------------------------------------------------------------------------------------------------------------------

Steps to run K_Means:

	1. Extract 214101019_Assignment4 zip file
	2. Open 214101019_K_Means folder 
	3. Open K_Means.sln in Microsoft Visual Studio 2010
	4. If K_Means code file is not opened then : Go to Solution Explorer (on the right side of the window) -> Go to source files -> Open K_Means.cpp code file 
	5. Run the code file
	6. Output prints the Initial CodeBook , distortion obtained in each iteration , final distortion , final Codebook

Steps to run K_Means:

	1. Extract 214101019_Assignment4 zip file
	2. Open 214101019_LBG folder 
	3. Open LBG.sln in Microsoft Visual Studio 2010
	4. If Lbg code file is not directly opened then : Go to Solution Explorer (on the right side of the window) -> Go to source files -> Open LBG.cpp code file 
	5. Run the code file
	6. Output prints the Initial codebook , codebook and distortion for size = 2,4,8 ,  final Codebook


Note:

	i).  Break Point is already added at the return statement of main function(in both K_means.cpp and LBG.cpp)
	ii). Input file universe.csv is already contained(in both K_means.cpp and LBG.cpp) 



Final Distortion Range: about 3 to 4 (in both K_means and LBG)

-------------------------------------------------------------------------------------------------------------------------------------------------------------

						 K_means Function Calls

					     -----------main---------
					     /	        |	    \
					    /		|            \
					   /		|	      \
		       initialize_universe()   initialize_codebook() K_means()
									|
									|
								       	|
								   cal_distortion()



						LBG Function Calls 

					     -----------main---------
					      /	         	  \
					     /		           \
					    /		            \ 
		          initialize_universe()   		  LBG() 
								    |
								    |
								    |
								K_means()
								     |
								     |
								cal_distortion()
-------------------------------------------------------------------------------------------------------------------------------------------------------------
Implementation Procedure for K_means:

	1. Read universe.csv file and store values in universe[6340][12] array

	2. Initialize codebook with 8 random universe vectors(serving as 8 clusters)

	3. Set prev_distoriton to a large value and avg_distortion to 0.

	4. Map each universe vector to the nearest cluster by calculating distortion (using tokhura distance as distance metric) and keep adding distortion of each universe vector to avg_distortion.

	5. Divide avg_distortion by M(total #universe vectors)
	
	6. While mapping maintain two arrays :
		i).  Ci[6340] in which Ci[j] stores Cluster number to which universe vector j is mapped to. 

		ii). Mi[8] in which Mi[j] stores the number of universe vectors assigned to cluster j 

	7. If absolute difference between prev_distortion and avg_distortion is less then 0.00001 then terminate else go to step 8

	8. set prev_distortion to avg_distortion

	9. Update codebook by calculating centroid for each Clusters

	10. Go to step 4.
-------------------------------------------------------------------------------------------------------------------------------------------------------------
Implementation Procedure for LBG:

	1. Read universe.csv file and store values in universe[6340][12] array

	2. Initialize codebook as global centroid ; codebook size = 1 

	3. Split each codebook vector (y(i))  using the following formula:

		i).  y(i)+ = y(i)*(1 + epsilon)

		ii). y(i)- = y(i)*(1 - epsilon)

	4. Set size = 2 * size

	5. Call K_means algorithm to get the best set of centroids for the split codebook i.e. codebook of twice the size

	6. Iterate steps 3,4,5 while size < Maximum size (here 8)

-------------------------------------------------------------------------------------------------------------------------------------------------------------