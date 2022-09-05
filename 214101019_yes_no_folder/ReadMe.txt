							YES/NO Assignment 1					(Roll No. 214101019)



   * Implementation breif description:

	1. Calculate the DC Shift, ZCR per frame,Energy per frame for the ambient audio text file.
	2. Normalize the audio text file data.
	3. Calculate the ZCR,Energy per frame for the normalized data.
	4. Generate start marker and end marker, if difference between start and end marker is too small then discard it.
	5. Depending on the average ZCR value of frames beloonging in between start and end marker classify as YES/NO.

  *How to execute code:
		1. Extract 214101019_yes_no_.zip folder
		2. Open noise_energy_zcr.sln 
		3. Once noise_energy_zcr opens Run the project
		4. Breakpoint has been added at return 0 (of main function) so that output can be seen.
		5. Output contains zcr,enery,DC shift of ambient followed by number of words identified, start and end marker and YES/NO decision.

	Note: Input files has been already included in the project file  
	Note: By default audio.txt is used for implementation to run audio2.txt go to line 199 of code and change audio.txt to audio2.txt(it contains 3 yes followed by 3 nos)
				
		 			    
    *Working and flow of code:
					
					 FUNCTION CALLS

					-------main()------				
					/        |      \
				       /         |       \
				      /          |        \
			noise_dc_zcr()      solve()	markers()				
			   /		       /  \
			  /		      /    \
			 / 		     /	    \
      number_of_samples()	 normalize()      frames_ene_zcr()	
		|		
		|
		|
          discard_samples()






		1. The main function contains 3 function calls 
			a. noise_ene_zcr() => which calculates the DC Shift,Energy per frame,ZCR per frame of the ambient.txt file i.e. this function identifies the features of ambient.txt file.
			b. solve() => which normalizes the audio.txt data and stores in normalized.txt file, calculates zcr,energy per frame and stores in frames_file.txt file.
			c. markers() => which marks start and end marker, followed by classification of the word.

		2. noise_ene_zcr()
			->This function calculates the DC_Shift, Average energy per frame, ZCR per frame of Ambient audio 
			->1 frame = 100 samples
			->It opens ambient.txt file and calculates the zcr,energy followed by taking its average
		
		3.solve()
			->this function makes 2 function calls
				a. number_of_sample => which simply calculates number of sample in file 
				b. frames_ene_zcr => to normalize the audio samples and calculate energy,zcr per frame
		4.marker()
			->This function finds start and end markers and then classifies the word
				** To find end marker following logic is used
					^TO SET START MARKER =>> For three subsequent frames following conditions are chaecked
						->if energy of first frame is less than 12.75% of average energy
						->and energy of second ,third frame is greater than 12.75% of average enrgy(i.e. increasing) 
						->then mark it as start marker 
					^TO SET END MARKER =>> For three subsequent frames following condiotns are checkcked
						-> if energy of first frame is greater than 12.75% of  average energy 
						->and energy of second ,third frame is less than 12.75% of average enrgy(i.e. decreasing) 
						->then mark it as end marker 
				** To classify as YES/NO following logic is used
					->the energy,zcr for each frame belonging between start and end marker is summed up
					->Then the zcr,energy is divided the number of frames (end marker - start marker)
					->if zcr is <0.7% of energy zcr then it is NO else YES 
		

	*Inputs Used:
			1. ambient.txt => containing the background noise of the room 
			2. audio.txt =>containing yes/no text data (it contains 4 yes followed by 4 no)
	
					
		

 