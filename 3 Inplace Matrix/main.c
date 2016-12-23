/**
 *  Niculae Stefan, gr 231
 *  3 oct 2014
 *  Tema Laborator Sisteme de Operare
 *
 *  Problema F20
 *  Adunare si inmultire a matricelor din fisiere binare
 *  cu accesare directa din fisier (fara e le retine in memorie)
 */

#include <stdio.h>
#include <string.h> // strcmp

// Trying to emulate exceptions
int errorCode = 0;

/**
 *  Returns the element on the position (i, j)
 *  from the matrix in the file FILE
 */
float get (FILE* file, int i, int j)
{
    // The dimensions of the matrix are required in order to set the cursor
    int rows,
        cols;
     
    
    // Read the number of rows and columns
    fread (&rows, sizeof (int), 1, file),
    fread (&cols, sizeof (int), 1, file);
    
    if (i > rows || j > cols) {
        errorCode = -21;
        printf ("Error! Matrix index out of bounds! i = %d, j = %d, but rows = %d, cols = %d",
                i, j, rows, cols);
        return errorCode;
    }
    
    
    int offset = cols * i + j;
    
    // Set the cursor at the position (i, j) in the file
    fseek (file, offset * sizeof (float), SEEK_CUR);
    
    float elem;
    // Read the element
    fread (&elem, sizeof (float), 1, file);
    
    return elem;
}

/**
 *  Sets the elemet at the position (i, j) to x
 *  in the matrix contained in FILE
 */
void set (FILE* file, int i, int j, float x)
{
    // The dimensions of the matrix are required in order to set the cursor
    int rows,
    cols;
    
    // Set the cursor at the beginning of the file
    fseek (file, 0, SEEK_SET);
    
    // Read the number of rows and columns
    fread (&rows, sizeof (int), 1, file),
    fread (&cols, sizeof (int), 1, file);
    
    if (i > rows || j > cols) {
        errorCode = -21;
        printf ("Error! Matrix index out of bounds! i = %d, j = %d, but rows = %d, cols = %d",
                i, j, rows, cols);
        return;
    }
    
    
    int offset = cols * i + j;
    
    // Set the cursor at the position (i, j) in the file
    fseek (file, offset * sizeof (float), SEEK_CUR);
    
    
    // Write the number received as parameter
    fwrite (&x, sizeof (float), 1, file);
}


/**
 *  Checks if the matrix in op1File can be added to the one in op2File
 *  If possible, writes the result in resFile
 */
void matrixSum (FILE* op1File, FILE* op2File, FILE* resFile)
{
    // Set the cursor for all three files at the begining
    fseek (op1File, 0, SEEK_SET),
    fseek (op2File, 0, SEEK_SET),
    fseek (resFile, 0, SEEK_SET);
    
    int rowsA, colsA,
        rowsB, colsB;
    
    // Read the number of rows and columns from each operand
    fread (&rowsA, sizeof (int), 1, op1File),
    fread (&colsA, sizeof (int), 1, op1File),
    
    fread (&rowsB, sizeof (int), 1, op2File),
    fread (&colsB, sizeof (int), 1, op2File);
    
    
    // The number of rows and cols must be the same
    if (rowsA != rowsB ||
        colsA != colsB) {
        printf ("Error! Invalid dimensions for summation: (%d, %d) and (%d, %d)\n",
                rowsA, colsA, rowsB, colsB);
        errorCode = -3;
        return;
    }
    
    // Write the common number of columns and rows into the result file
    fwrite (&rowsA, sizeof (int), 1, resFile),
    fwrite (&rowsA, sizeof (int), 1, resFile);
    
    
    
    // Do the addition
    for (int i = 0; i < rowsA; i++)
        for (int j = 0; j < colsA; j++) {
            
            float sum = get (op1File, i, j) +
                        get (op2File, i, j);
            
            // C(i,j) = A(i,j) + B(i,j)
            set (resFile, i, j, sum);
            
        }
}

/**
 *  Checks if the matrix in op1File can be multiplied by the one in op2File
 *  If possible, writes the result in resFile
 */
void matrixProd (FILE* op1File, FILE* op2File, FILE* resFile)
{
    // Set the cursor for all three files at the begining
    fseek (op1File, 0, SEEK_SET),
    fseek (op2File, 0, SEEK_SET),
    fseek (resFile, 0, SEEK_SET);
    
    int rowsA, colsA,
        rowsB, colsB;
    
    // Read the number of rows and columns from each operand
    fread (&rowsA, sizeof (int), 1, op1File),
    fread (&colsA, sizeof (int), 1, op1File),
    
    fread (&rowsB, sizeof (int), 1, op2File),
    fread (&colsB, sizeof (int), 1, op2File);
    
    
    // Check if A has as many columns as there are rows in B
    if (colsA != rowsB) {
        printf ("Error! Invalid dimensions for multiplication: (%d, %d) and (%d, %d)\n",
                rowsA, colsA, rowsB, colsB);
        errorCode = -4;
        return;
    }
    
    // The number of rows and columns of the resulting matrix
    int rows = rowsA,
        cols = colsB,
    // The common dimension of the two operands
        comm = colsA;
    
    // Write the number of rows and cols in the result file
    fwrite (&rows, sizeof (int), 1, resFile),
    fwrite (&cols, sizeof (int), 1, resFile);
    
    // Initialize the result file with zeroes
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            set (resFile, i, j, 0);
    
    // Do the multiplication
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            for (int i = 0; i < comm; i++) {
                
                // C(r, c) += A(r, i) * B(i, c)
                
                float prev = get (resFile, r, c),
                      prod = get (op1File, r, i) * get (op2File, i, c);
                
                set (resFile, r, c, prev + prod);
                
            }
}


/* DEBUGGING...
void contentsOf (FILE* file)
{
    fseek(file, 0, SEEK_SET);
    
    int x, y;
    fread(&x, sizeof(int), 1, file),
    fread(&y, sizeof(int), 1, file);
    
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            float z;
            fread(&z, sizeof(float), 1, file);
            printf ("%.1f ", z);
        }
        printf ("\n");
    }
}
void setM (FILE* file)
{
    fseek(file, 0, SEEK_SET);
    
    int r = 2, c = 2;
    fwrite(&r, sizeof(int), 1, file),
    fwrite(&c, sizeof(int), 1, file);
    
    float a[10][10];
    a[0][0] = 1, a[0][1] = 2, a[1][0] = 3, a[1][1] = 4;
    
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            fwrite(&a[i][j], sizeof(float), 1, file);
    
}
... DEBUGGING */


int main(int argc, char* argv[])
{
    
    // Check to see if the input is correct
    if (argc != 5) {
        printf ("Error! Invalid number of arguments: need operation, path for two operands and for result. \n Example: F20 sum op1.bin op2.in res.bin\n");
        return -1;
    }
    
    // Open the files from their respective paths
    // for reading and writing in binary file system
    FILE* op1File = fopen (argv[2], "rb"),
        * op2File = fopen (argv[3], "rb"),
        * resFile = fopen (argv[4], "w+b");
    
    // Check if the files have been successfully opened
    if (op1File == NULL ||
        op2File == NULL ||
        resFile == NULL) {
        
        printf ("Error opening the files\n");
        return -3;
        
    }
    
    // Command Handling
    char* operations[2] = { "sum", "pro" };
    
    // If the operation is matrix sum
    if (strcmp (argv[1], operations[0]) == 0) {
        
        matrixSum (op1File, op2File, resFile);
        if (errorCode)
            return errorCode;
        
        printf ("Successfully added the matrix in %s to the one in %s and written the result in %s\n",
                argv[2], argv[3], argv[4]);
        
    }
    
    // If the operation is matrix product
    else if (strcmp (argv[1], operations[1]) == 0) {
        
        matrixProd (op1File, op2File, resFile);
        if (errorCode)
            return errorCode;
        
        printf ("Successfully multiplied the matrix in %s by the one in %s and written the result in %s\n",
                argv[2], argv[3], argv[4]);

    }
    
    else {
        
        printf ("Error! Invalid operation idenfier: expected %s or %s, got %s\n", operations[0], operations[1], argv[1]);
        return -5;
        
    }
    
    fclose (op1File),
    fclose (op2File),
    fclose (resFile);
    
    return 0;
}
