HPC-Data-Processing
===================

This repo tells you how to Initialise the MPI commands in C++ and process the large text files which are of size 5GB or more.

Project context
---------------
The context of this whole code is to proccess a large textfile, generally of size 3GB or more and count the no of occurrence of a user give key word.

The main function takes two arguments textfilename.txt and key word to search through textfile and count no of occurrences.
The for loop present in the code, to process large textfile (5GB in this case) by breaking it into smaller files (1GB min) and search through by adding all the words into a Vector<string> words. 

After sending the message to the slave nodes, each node is defined to do a certain job. In this case access the vector<string> at a given index and count the occurrences of the key. 

Finally, send all the caluculated information to Master node and write to a file.
