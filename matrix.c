#include "matrix.h"

/**
 * This function initializes and returns a dense matrix 
 * as a dynamic two-dimensional array.
 */
DenseMatrix initDenseMatrix(int columnLength, int rowLength) {
	DenseMatrix denseMatrix = (DenseMatrix)malloc(sizeof(DenseMatrix_t)); // the memory for dense matrix
	int** matrix = (int**)malloc(sizeof(int*)*rowLength); // the memory for 2D array. This array is matrix of dense matrix. Firstly created for rows. The second pointer is for columns.
	
	for (int i=0; i<rowLength ;i++){
		matrix[i] = (int*)malloc(sizeof(int)*columnLength); // the memory for columns of array. columns added to all rows.
	}

	for (int i=0; i<rowLength ;i++){
		for (int j=0; j<columnLength ; j++){
			matrix[i][j] = 0; // all points made zero in initilazing.
		}
	}

	denseMatrix->matrix = matrix;
	denseMatrix->rowLength = rowLength;
	denseMatrix->columnLength = columnLength;

	return denseMatrix;
}

/**
 * This function deallocates all the rows and 
 * columns of the given dense matrix.
 */
void freeDenseMatrix(DenseMatrix denseMatrix) {
	for (int i=0; i<denseMatrix->rowLength; i++) {
		free(denseMatrix->matrix[i]);
	}
	free(denseMatrix->matrix);
	free(denseMatrix);
}

/**
 * This function creates a dense matrix and fills 
 * the elements in the given parse matrix as a set.
 */
DenseMatrix parseMatrixToDenseMatrix(Set parseMatrix, int columnLength, int rowLength) {
	DenseMatrix denseMatrix = initDenseMatrix(columnLength, rowLength); 
	for (int i=0; i<parseMatrix->cardinality; i++){

		int* data = (int*)parseMatrix->elements[i]->data; // keep the data of parse matrix
		int x = data[0]; // x value of data
		int y = data[1]; // y value of data
		int number = data[2]; // number value of data
		if (x<rowLength && y<columnLength) {
    		denseMatrix->matrix[x][y] = number; // put the number in dense matrix according to (x,y) coordinates.
		}
	}
	return denseMatrix;
}

/**
 * This function creates and returns a parse matrix 
 * as a set depending on the given dense matrix's elements.
 */
Set denseMatrixToParseMatrix(DenseMatrix denseMatrix) {
	Set parseMatrix = initSet(); // parse matrix is a set.
	for (int i=0; i<denseMatrix->rowLength; i++){
		for (int j=0; j<denseMatrix->columnLength; j++){
			int number = denseMatrix->matrix[i][j]; // take the numbers from their coordinates in dense matrix.
			if (number != 0){
				Element element = createMatrixPointElement(i,j,number); // i is x, j is y coordinates. So create matrix point according to that.
				insertElement(parseMatrix,element); // insert matrix point element that created.
			}
		}
	}
	return parseMatrix;
}

/**
 * This matrix creates a new dense matrix, and the 
 * matrix is the addition of the given two matrices.
 */
DenseMatrix addDenseMatrices(DenseMatrix dm1, DenseMatrix dm2) {
	if (dm1->rowLength != dm2->rowLength || dm1->columnLength != dm2->columnLength){ // the sizes of both dense matrix are must be same.
		printf("The size of matrixes are different"); // if it is not, give warning.
		return NULL;
	}
	else {

		DenseMatrix denseMatrix = initDenseMatrix(dm1->columnLength, dm1->rowLength); // create a new matrix.

		for (int i=0; i<denseMatrix->rowLength; i++){
			for (int j=0; j<denseMatrix->columnLength; j++){
				denseMatrix->matrix[i][j] = dm1->matrix[i][j] + dm2->matrix[i][j]; // add the total value for a (x,y) point from dm1 and dm2.
			}
		}
		return denseMatrix;
	}
}
/**
 * This matrix creates a new sparse matrix as a set, 
 * and the matrix is the addition of the given 
 * two sparse matrices.
 */

// There is a add funciton for dense matrixes. So if we use that function, the work will be easier, better and more clear.
// We can use translation and addition functions in that we created:
Set addSparseMatrices(Set sm1, Set sm2, int columnLength, int rowLength) {
	DenseMatrix denseMastrix1 = parseMatrixToDenseMatrix(sm1,columnLength,rowLength); // translate the first parse matrix to dense matrix
	DenseMatrix denseMastrix2 = parseMatrixToDenseMatrix(sm2,columnLength,rowLength); // translate the second parse matrix to dense matrix
	DenseMatrix newDenseMatrix = addDenseMatrices(denseMastrix1,denseMastrix2); // make addition the dense matrixes
	Set sparseMatrix = denseMatrixToParseMatrix(newDenseMatrix); // again translate dense matrix to parse matrix
	freeDenseMatrix(denseMastrix1); // free all dense matrixes that created
    freeDenseMatrix(denseMastrix2); 
    freeDenseMatrix(newDenseMatrix);
	return sparseMatrix;
}



// I tried calculate directly parse matrix addition.
// The using other functions are enough for this function. 
// It is not working properly but can develope:

/*Set addSparseMatrices(Set sm1, Set sm2, int columnLength, int rowLength) {
	Set newSparseMatrix = unite(sm1,sm2);

	for (int i=0; i<newSparseMatrix->cardinality; i++){
		Element element1 = newSparseMatrix->elements[i];
		int* data1 = (int*)element1->data;
		int x1 = data1[0];
		int y1 = data1[1];
		int number1 = data1[2];

		if(x1>=rowLength || y1>=columnLength){
			removeElement(newSparseMatrix,element1);
			i--;
			continue;
		}

		for(int j=0; j<newSparseMatrix->cardinality; j++){
			if(i!=j){
				Element element2 = newSparseMatrix->elements[j];
				int* data2 = (int*)element2->data;
				int x2 = data2[0];
				int y2 = data2[1];
				int number2 = data2[2];

				if(x2>=rowLength || y2>=columnLength){
				removeElement(newSparseMatrix,element2);
				j--;
				continue;
				}

				if(x1==x2 && y1==y2){
					int total = number1 + number2;
					Element addedElement = createMatrixPointElement(x1,y1,total);
					insertElement(newSparseMatrix,addedElement);
					removeElement(newSparseMatrix,element1);
					removeElement(newSparseMatrix,element2);
					i--;
					break;
				}
			}
		}
	}
	return newSparseMatrix;
}*/