int emParalelo( int myR, int worldSize ) {
int aux;
MPI_Status st;
if( myR == 0 ) {
MPI_Send((void*)&myR, 1, MPI_INT, myR+1, 0, MPI_COMM_WORLD);
MPI_Recv(&aux, 1, MPI_INT, worldSize-1, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
} else {
dest = (myR == worldSize-1)?0:myR+1;
MPI_Recv(&aux,1,MPI_INT,myR-1, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
aux += myR;
MPI_Send(&aux, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
}
return aux+myR; // Será descartado quando myRank != 0
}
