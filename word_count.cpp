/**
    Cluster and Cloud Computing
    word_count.cpp
    
    @author Venkatesh Yeluri
    @version 1.0 April 10, 2014
*/

#include <iostream>       
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <mpi.h>
#include <vector>
#include <time.h>
#include <ctime>
#include <fstream>


using namespace std;

  /**
      Function to remove all the special characters in the text
      file and count the no of words of user input(key) matches
  */

int SearchFunction(int sPoint, int ePoint, const std::string& key, vector<string> &words) { 
  int count = 0;

  for (int i=sPoint; i <= ePoint; i++) {
    if (sPoint != ePoint) {
      string replacer = words[i];

      replace(replacer.begin(), replacer.end(), '!', ' ');
      replace(replacer.begin(), replacer.end(), '@', ' ');
      replace(replacer.begin(), replacer.end(), '#', ' ');
      replace(replacer.begin(), replacer.end(), '$', ' ');
      replace(replacer.begin(), replacer.end(), '%', ' ');
      replace(replacer.begin(), replacer.end(), '^', ' ');
      replace(replacer.begin(), replacer.end(), '&', ' ');
      replace(replacer.begin(), replacer.end(), '*', ' ');
      replace(replacer.begin(), replacer.end(), '(', ' ');
        replace(replacer.begin(), replacer.end(), ')', ' ');
        replace(replacer.begin(), replacer.end(), '-', ' ');
        replace(replacer.begin(), replacer.end(), '_', ' ');
        replace(replacer.begin(), replacer.end(), '=', ' ');
        replace(replacer.begin(), replacer.end(), '+', ' ');
        replace(replacer.begin(), replacer.end(), '`', ' ');
        replace(replacer.begin(), replacer.end(), '~', ' ');
        replace(replacer.begin(), replacer.end(), '[', ' ');
          replace(replacer.begin(), replacer.end(), ']', ' ');
          replace(replacer.begin(), replacer.end(), '{', ' ');
          replace(replacer.begin(), replacer.end(), '}', ' ');
          replace(replacer.begin(), replacer.end(), '|', ' ');
          replace(replacer.begin(), replacer.end(), '\\', ' ');
          replace(replacer.begin(), replacer.end(), '\"', ' ');
          replace(replacer.begin(), replacer.end(), ':', ' ');
          replace(replacer.begin(), replacer.end(), ';', ' ');
          replace(replacer.begin(), replacer.end(), '/', ' '); 
          replace(replacer.begin(), replacer.end(), '<', ' ');
          replace(replacer.begin(), replacer.end(), '>', ' ');
          replace(replacer.begin(), replacer.end(), ',', ' ');
          replace(replacer.begin(), replacer.end(), '.', ' ');
          replace(replacer.begin(), replacer.end(), '\'', ' '); 

          std::string replace = replacer;
          std::string k = key;

          std::transform(replace.begin(), replace.end(), replace.begin(), ::tolower);  
          std::transform(k.begin(), k.end(), k.begin(), ::tolower);

          if (replace == k) {
            count = count + 1;   
          }
        }
      }
      return count;  
     
}
    
    /**
        Main function begins from here
     */

 int main(int argc, char** argv) {

  std::fstream outputFile;

  int n;
  double elapsed_time;
  elapsed_time = - MPI_Wtime();

  int id = 0;  
  int no_nodes = 2; 

  int Master_TotalWords = 0;      /**************************** Master Data - Total Words in file ***********************************/

  int Master_WorkCount = 0;      /************** Master Data - Total Work Count - Defines total matches of Node 1 & 2 **************/

  int Master_Node1_Count = 0;   /**************************** Master Data - Node 1 total count ***********************************/

  int Master_Node2_Count = 0;  /**************************** Master Data - Node 2 total count ***********************************/

    MPI_Status status;

  //MPI Initialisation
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &no_nodes);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  printf("%d, %d\n", id, no_nodes); 

  //Defining MPI Barrier using MPI_COMM_WORLD
  MPI_Barrier(MPI_COMM_WORLD);


  for (int i=1; i<=5; i++) {  // Start Point of Master Loop 
 
  char *file_name = argv[i];  // MPI - FILE NAME to process the data

  string key = argv[6];

  int total_workCount = 0;
  int totalWords = 0;

  std::vector<string> words;  //Defining a std::vector to store each word
  ifstream myfile(file_name); 
  string eachWord;

  /**
      Initializing no of nodes based on user input
      In this case we have 2 nodes, which stores 
      word count value
  */
  int node1, node2; 
  
  int value = 0;


  /** 
      Master node reads the file 
      and adds to Vector 
  */

  // Check file before u process it
  if(!myfile) 
  {
    cout<<"\nError opening output file"<<endl;
    return -1;
  }
  else {
    cout<<"\nFile opened successfully\n"<<endl;
    
    while (getline(myfile, eachWord, ' '))
    {
      words.push_back(eachWord);
    }
  }

  totalWords = words.size();
  Master_TotalWords += totalWords;   // Master Total Words for each block

  printf("Total word count: --  %d, Slave id: -- %d\n", totalWords, id);

  

/**
    Master node to assign work to slave nodes

    &value - value to send to the slave node
    1 - count
    MPI_INT - datatype
    i - Destination (In other words to whom you sending node 2 or 3..)
    0 - tag
    MPI_COMM_WORLD - Standard cmd 
  */
  if(id==0){
    for(int i =1;i<no_nodes;i++) // Cycle through the nodes and send data
    {
      MPI_Send (&value, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      //Testing purpose
      printf("Send function - 1st -----  Master id: %d, Slave id: %d, Value = %d\n", id,  i, value); 

    }
  }else{ 
    // Nodes recive data from Master
    MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

    printf("Receive function - 1st --------  Master id: %d, Slave id: %d, value = %d\n", 0, id, value);
  }

  /** 
      Defining each node (1,2,..) to do some work 

      After sending the message to the slaves,
      we have to define what they should do. 
      In other words define their work.
    */

  if (id == 0) {    //Master node with id = 0

    int block = totalWords/no_nodes;
    value = SearchFunction(0, block-1, key, words);
    total_workCount = total_workCount + value;
    Master_WorkCount += total_workCount;                     // Master_WorkCount --- Total count from two guys (total matches from Node 1 & 2)
    node1 = value;                                           // Value for writing to file
    Master_Node1_Count += node1;                             // Master_Node1_Count --- To write the total value to file 
    printf("Data from the Master node  --  %d\n", value);

  }
  if (id==1) {     // Slave node with id = 1
    int block = totalWords/no_nodes;
    value = SearchFunction(block, totalWords - 1, key, words);
    total_workCount = total_workCount + value;
    Master_WorkCount += total_workCount;                     // Master_WorkCount --- Total count from two guys (total matches from Node 1 & 2)
    node2 = value; // Value for writing to file
    Master_Node2_Count += node2;                             // Master_Node2_Count --- To write the total value to file 
  }

  

  /** 
      Master gets ready to recive data from slave threads
   */

  if(id==0){ 
    for(int i =1;i<no_nodes;i++)
    {
     MPI_Recv(&value, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status); 
     total_workCount = total_workCount + value;
     Master_WorkCount += value; 

     printf("Receive function - 2nd --------  Master id: %d, Slave id: %d, value = %d\n", id, i, value);
     cout<<"Total count form two guys  --  "<<total_workCount<<endl;
   }
 }else{
  		MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // each slave computes its own word count and sends to master

      printf("Send function - 2nd -----  Master id: %d, Slave id: %d, Value = %d\n", 0,  id, value); 
    }

    words.clear();      //Clearing all elements in the Vector

  }                                             /************* End Point of Master Loop **************/


    // stop the timer
    elapsed_time += MPI_Wtime();          

    // Disply results 
    cout<<"Elapsed Time  -- "<<elapsed_time<<endl;

    outputFile.open("mpiOutput.txt", std::fstream::in | std::fstream::out | std::fstream::app);

    /**
          Defining Master and Slave node to write the output
          to a file
      */
    if (id == 0) {
      outputFile<<"Total no of words in the file -- "<<Master_TotalWords<<char(13)<<char(10);
      outputFile<<"Total no of matches -- "<<Master_WorkCount<<char(13)<<char(10);

      outputFile<<char(13)<<char(10);

      outputFile<<"No of matches in Node one -- "<<Master_Node1_Count<<char(13)<<char(10);
      outputFile<<"Node one process time -- "<<elapsed_time<<"sec"<<char(13)<<char(10);
    }

    if (id == 1) {
      outputFile<<"No of matches in Node two -- "<<Master_Node2_Count<<char(13)<<char(10);
      outputFile<<"Node two process time -- "<<elapsed_time<<"sec"<<char(13)<<char(10);

    }
    outputFile.close();

    // MPI Programs end with MPI_Finalize
	  MPI_Finalize();	
	  return 0;                
}


