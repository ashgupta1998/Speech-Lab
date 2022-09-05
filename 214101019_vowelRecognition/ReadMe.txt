
									Vowel Recognition Assignment 	
						
																					(Roll_NO. 214101019)
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Steps to run code:


	1. Extract 214101019_vowelRecognition.zip

	2. Open Vowel_Recognition.sln file present in Vowel_Recognition folder in Visual Studio->Select Vowel_Recognition_.cpp from Source files present in Solution Explorer on the Right side of the window.

	3. The input folders required are included however the inputs(includes ambient.txt,100 vowel recordings) are still provided in the input folder  

	4. Break point is added at return 0; in main function so as to view output

	5. Run the code and file assertion error is prompted then make sure that all required files are present in the folder

        6. Output prints the file being processed for reference file generation, Test files used for prediction followed by printing Tokura distance of that test file from each vowel and then prints the vowel            	   predicted


 Note: ambient.txt is used to find DC shift 

 Accuracy of 94% is obtained 
 Detailed accuracy is as under:
 Vowel => Accuracy 
 a     =>  100%
 e     =>  80%
 i     =>  100%
 o     =>  100%
 u     =>  90%
	
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 Implementation procedure:

	1. DC shift is calculated using ambient noise

	2. For each vowel out of 20 recording first 10 are used for generation of reference file and last 10 for prediction

	3. Steps to generate reference file for a vowel:
		a. A vowel file is selected and its DC shift is obtained and normalization is done

		b. Initial frames 5000-5500 frames are discarded and next 1600 samples are considered for steady frame (since all are vowels and as vowels are periodic in nature thus discarding is done to 				obtain steady frames).

		c. This 1600 samples i.e. 5 frames are stored in a array named rough[1600]

		d. Using rough array, 320 normalised samples are extracted as a frame and is written in a file named ref_rough 

		e. The ref_rough file is passed to function Calculate_Ci for further processing i.e calulation of Ri followed by Ai and Ci and finally application of raised sine window on Ci's.

		e. The 12 Ci's obtained is stored as row in ref_mat . ref_mat is reference matrix computed for each vowel which stores the reference file for that vowel. Eg. ref_mat_A, ref_mat_e,..

		f. Steps c to e are repeated 4 more times and each time a new row gets added to ref_mat

		g. Steps a to f is repeated for 9 more time i.e. 9 more files of the same vowel under conideration and in each step the previous ref_mat is carried forward and values are added to it.

		h. Finally for each element of ref_mat average w.r.t 10 is taken and ref_matrix for the vowel under consideration is obtained

	4. Similarly reference file for remianing 4 vowels are obtained and the 5 reference text files will be used as reference file for those vowels.

	5. For Testing:
		a. Take input files for testing (10 files per vowel) and for each file do the following.

		b. Perfom normalization and extract 5 steady frames

		b. Calculate Ai,Ri,Ci and apply raised sine window on Ci's.

		c. For each steady frame store Ci's as a row so total 5 rows each of size 12 will be obtianed.

		d. Using the reference text files obtained in step 3 calculate Tokura's Distance for each element

		e. For each row take average w.r.t 12 

		f. Now take minimum of the 5 values (as 5 rows thus 5 vaues) as distance from that corresponding vowel and store it in matrix dis_min[5](this mmatrix stores distance from each vowel, index 1 					stores distance from a , index 2 stores distance from e, ...) .

		g. Repeat steps d to f for remaining 4 reference text files print the dis_min[5] matrix

		h. The vowel from which the minimum distance is obtained is predicted as the vowel. 

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Function Calls:


							FUNCTION CALLS

							-------main()------				
							/              \
						       /                \
				      		      /                  \
					Vowel_Reference()      		Vowel_Prediction()				
					   /		      			\
			 ---------Calculate_refvowel------		  ----Test_refvowel---------------------
				 	 / 		     			       \			\
              		 ------ Calculate_Ci()--------       		   ------ Calculate_Ci()--------       ---Tokura_Distance()--
	    			 /    	|	\				   /        |	      \
	  		        /       |        \				  /         |          \
			       / 	|         \			         /   	    |           \
	Calculate_normalize()    Calculate_Ai()  Calculate_Ri()  Calculate_normalize()   Calculate_Ai()  Calculate_Ri()
		|
		|
		|
	Calculate_dc()
	    			
	
  
























	 