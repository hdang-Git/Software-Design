/*
 * This program performs the matrix multiplication of 2 nxn matrices using 
 * parallel processing on the osprey cluster. This program is adapted 
 * from the provided matrix_time_vector.c. 
 *
 *		Note:
 *			To run this program, ssh into osprey, 
 *			create the hosts file and then ssh into chinook.
 *
 *			To compile:
 *				Ex.		mpicc -o matrixMpiMult matrixMpiMult.c
 *
 *			To execute:
 *			 	Ex.		mpiexec -f ~/hosts -n 3 ./matrixMpiMult 3
 *
 *
 */

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define min(x, y) ((x)<(y)?(x):(y))

//Function prototypes
void printMatrix(double*, int, int, char*);


/*********************************************************************************************
 * This function handles the master and slave processes by dividing up the matrix. The       *
 * master broadcasts the matrix bb to the other slaves, divies up the matrix row in aa into  *
 * an array called buffer, and then sends the buffer row to any of the slaves. The slave     *
 * then calculates the row of aa times the column of bb and returns that back to master in   *
 * ans and the index of the matrix result in anstype. The result of the calculations are     *
 * stored in cc. *                                                                           *
 * The master continously sends new rows to slaves as soon as a process is completed and     *
 * sends the terminate signal when there are no more entries to process.                     *
 *                                                                                           *
 * Preconditions:                                                                            *
 * Expects argv[1] to store the size of the matrix                
 *********************************************************************************************/
int main(int argc, char* argv[])
{
  	int nrows, ncols;
  	double *aa, *bb, *cc;
  	double *buffer, ans;		//buffer stores temp row form aa; 
								//ans stores the calculated element of multiplied matrix
  	double *times;
  	double total_times;
  	int run_index;
  	int nruns;
  	int myid, master, numprocs;
  	double starttime, endtime;
  	MPI_Status status;
  	int i, j, numsent, sender;
  	int anstype, row;
	//create the time
  	srand(time(0));
    //Initialize MPI
	MPI_Init(&argc, &argv);
	//Get the Number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	//Set up processors with ids for master & slaves
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	//get the matrix size from user args
  	if (argc > 1) {
    	nrows = atoi(argv[1]);		//get size of matrix
    	ncols = nrows;
		aa = (double*)malloc(sizeof(double) * nrows * ncols);	//input matrix 1
		bb = (double*)malloc(sizeof(double) * nrows * ncols);	//input matrix 2
		cc = (double*)malloc(sizeof(double) * nrows * ncols);	//store the actual answers
		buffer = (double*)malloc(sizeof(double) * ncols);		//stores the row to be passed from aa
		master = 0;
		if (myid == master) {
		  	// Master Code goes here
			//populate array aa with random values
		  	for (i = 0; i < nrows; i++) {
				for (j = 0; j < ncols; j++) {
		  			aa[i*ncols + j] = (double)rand()/RAND_MAX;	//1;
		  			bb[i*ncols + j] = (double)rand()/RAND_MAX;	//i+j;
					cc[i*ncols + j] = 0;	//zero out the cc matrix
				}
		  	}
			
			printMatrix(aa, nrows, ncols, "\nMatrix aa:");			
			printMatrix(bb, nrows, ncols, "\nMatrix bb:");			
		  	//start the clock
		  	starttime = MPI_Wtime();
		  	numsent = 0;
			//Broadcasts to slaves matrix bb
		  	MPI_Bcast(bb, nrows*ncols, MPI_DOUBLE, master, MPI_COMM_WORLD);	
			//synchronize the processors to get the number of rows sent & send rows to slaves
			//min(numprocs-1, nrows) sends to the existing processes and not to ones that DNE
			// 3 processors v 6 rows -> 3 so split the work better
		  	for (i = 0; i < min(numprocs-1, nrows*ncols); i++) {
				for (j = 0; j < ncols; j++) {
		  			buffer[j] = aa[i * ncols + j];	//copy over rows of aa to buffer
				}  
				//Send rows to available slaves [(i+1) for slave # & (numprocs-1) 
				//for slave count because master is 0]
				MPI_Send(buffer, ncols, MPI_DOUBLE, i+1, i+1, MPI_COMM_WORLD);	
				numsent++;	//number of rows sent out
		  	}
			//Get back the answer
		  	for (i = 0; i < nrows * ncols; i++) { 
				//Get back answer
				MPI_Recv(&ans, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, 
			 		MPI_COMM_WORLD, &status);
				sender = status.MPI_SOURCE;		//stores slave who processed it
				anstype = status.MPI_TAG;		//stores matrix index
				cc[anstype-1] = ans;			//stores result at that index of 2d array
				//Checks if there is a free slave and if free, then give it more work
				if (numsent < nrows * ncols) {	//checks processes < num matrix elements
				  for (j = 0; j < ncols; j++) {	//really < nrows
					//get values for row to send to slave again to repeat the process
					buffer[j] = aa[(numsent)/ncols * nrows + j]; //n/rows * rows + j = row# + j  
				  }  
				  //Send the row to the newly free worker process
				  MPI_Send(buffer, ncols, MPI_DOUBLE, sender, numsent+1, 
					   MPI_COMM_WORLD);
				  numsent++;
				} else {
				//terminate signal which calls on slave with MPI_Tag == 0
				  MPI_Send(MPI_BOTTOM, 0, MPI_DOUBLE, sender, 0, MPI_COMM_WORLD); 
				}
		  	} 
		  	endtime = MPI_Wtime();	//stop the clock
		  	printf("Time to calculate is: %f seconds\n",(endtime - starttime));
			printMatrix(cc, nrows, ncols, "\nResult c:");
		} else {
		  	// Slave Code goes here
		  	MPI_Bcast(bb, nrows * ncols, MPI_DOUBLE, master, MPI_COMM_WORLD);
		  	if (myid <= nrows) {
				while(1) {
				  	MPI_Recv(buffer, ncols, MPI_DOUBLE, master, MPI_ANY_TAG, 
					   MPI_COMM_WORLD, &status);
				  	if (status.MPI_TAG == 0){		//terminating signal received here to stop 
						break;
					}
				  	row = status.MPI_TAG;		//stores row number
				  	ans = 0.0;
			  		for (j = 0; j < ncols; j++) { 
						//buffer[j]  is the row, bb+j*ncols is the column slice 
						//calculate the element in the matrix
						ans += buffer[j] * (*(bb + (row-1)%ncols + j*nrows));
						printf("myid %d: %d ans: %.2f = buffer: %.2f * bb: %.2f\n",
								myid, j, ans, buffer[j], *(bb + (row-1)%ncols + j*nrows));
			  		}
					printf("\nAnswer:%.2f @ matrix entry %d\n", ans, row - 1);
			  		MPI_Send(&ans, 1, MPI_DOUBLE, master, row, MPI_COMM_WORLD);
				}
			}
		}
  	} else {
    	fprintf(stderr, "Usage matrix_times_vector <size>\n");
  	}
  	MPI_Finalize(); //finalize MPI operations
  	return 0;
}

/*********************************************************************************************
 * This function prints out the passed in matrix.                                            *
 *                                                                                           *
 * Preconditions:                                                                            *
 * @params matrix - input matrix that's passed in                                            *
 * @params row - number of rows                                                              *
 * @params col - number of cols                                                              *
 * @params msg - a message from the user to pass in to print out to terminal                 *
 *                                                                                           *
 * Postconditions:                                                                           *
 * @return void                                                                              *
 *********************************************************************************************/
void printMatrix(double *matrix, int row, int col, char* msg){
	printf("%s\n", msg);
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			//print row by row left to right (Think of 2d array as 1d)
			printf(" %3.2e ", *(matrix+i*col+j));	
		} 
		printf("\n");
	}
	printf("\n\n");
}

