# SeqAlignment
Dr. Pevzner Lab Rotation Spring 2016 VS

run SeqAlignment.exe in command line with the following parameters: 

--scoringMatrix/-sm     Path to the scoring matrix 						(required)
--type/-t 				Type of the file with reads 				(required)
--reads/-r				Path to the file with reads 				(required)
--outputFormat/-of 			Specifies the format of the output 			(required)
--start/-s				Index of the first candidate in “many” file type	(optional)
--end/-e				Index of the last candidate in the “many” file type 	(optional)

Example:
SeqAlignment.exe -sm ../input/alignment_probs_newchem_20k_noins.txt -r ../input/nhood.2.fasta -t single -s 0 -e 1000 -of short

When the program is done, it creates a 'results.txt' file in the same directory where the .exe file is.
It contains the output results according to the specified parameters.
