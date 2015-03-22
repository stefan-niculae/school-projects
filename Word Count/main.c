/**
 *  Niculae Stefan, gr 231
 *  8 oct 2014
 *  Tema Laborator Sisteme de Operare
 *
 *  Problema F6
 *  Simularea comenzii wc (-l -w -c) din fisier sau stdin
 *  (numarul de linii cuvinte sau caractere)
 */

#include <stdio.h>
#include <stdlib.h> // malloc
#include <string.h> // strcat, strcpy
#include <wctype.h> // iswspace

#define MAX_DIM 100 // Buffer size
char STDINPATH[3] = "..."; // Differentiate a folder path from the stdin

/**
 *  Returns the number of lines in the INPUT string
 *  taking into account the source of the input (stdin or file)
 */
int nrOfLines (char* input, int fromStdIn) {
    
    int newLines = 0;
    
    // Go through the input string and count the number
    // of new line characters (a blank line is counted as a line)
    for (int i = 0; input[i] != '\0'; i++)
        if (input[i] == '\n')
            newLines++;
    
    
    // We have NEWLINES new lines that means we have
    // NEWLINES + 1 lines in the input string
    newLines++;
    
    // As input from the standard input terminates with
    // ctrl-D on a new line, we must not count that as a
    // line of the input string
    if (fromStdIn)
        newLines--;
    
    return newLines;
}

/**
 *  Returns the number of words in the INPUT string
 *  (separated by white space)
 */
int nrOfWords (char* input) {
    
    int words = 0;
    
    // Count the first word (if the input is not empty)
    if (strcmp (input, "") != 0)
        words++;
    
    // Go through the input string
    for (int i = 0; input[i + 1] != '\0'; i++)
        // We count the number of words, not the number of white spaces
        // "a b" contains 2 words, as does "a    b"
        if ( iswspace (input[i]) &&
            !iswspace (input[i + 1])
            )
            words++;
    
    return words;
}

/**
 *  Returns the number of characters in the INPUT string
 */
int nrOfCharacters (char* input, int fromStdIn)
{
    int characters;
    
    for (characters = 0; input[characters] != '\0'; characters++);
    
    
    // If input source is the standard input, we decrement one
    // because the user has to press Ctrl-D on a new line to terminate input
    // and we do not count that as a character
    if (fromStdIn && strcmp (input, "") != 0)
        characters--;
    
    return characters;
}

/**
 *  Reads the input string either from a file or from the stdin
 *  and returns it as a string
 */
char* getInput (char* path)
{
    char buffer[MAX_DIM];
    char* input = (char*)malloc (MAX_DIM * sizeof (char));
    if (input == NULL) {
        perror ("Error! Not enough space\n");
        exit (-3);
    }
    
    // Initialize INPUT as empty
    strcpy (input, "");
    
    
    // If the path is the standard input
    if (strcmp (path, STDINPATH) == 0) {
        
        printf ("Enter your string to be counted. \nTerminate input by pressing Ctrl-D on a new line.\n");
        
        // Read input line by line from stdin into a buffer...
        while ( fgets (buffer, sizeof (buffer), stdin) != NULL )
            // ...and concatenate it to the to the INPUT string
            strcat (input, buffer);
    }
    
    // If the input is a file
    else {
        
        // Open the file at PATH in read mode
        FILE* file = fopen (path, "r");
        // Check if successfully opened
        if (file == NULL) {
            perror ("Error opening the file!");
            exit (-1);
        }
        
        // Same as stdin, only the source is different
        while ( fgets (buffer, sizeof (buffer), file) != NULL )
            strcat (input, buffer);
        
        fclose (file);
        
    }
    
    return input;
}

/**
 *  Returns the number of actual arguments (not options)
 *  also sets showLines to true if the -l option has been entered
 *  (same for -w, -c)
 */
int parseOptions (int argc, char* argv[], int* showLines, int* showWords, int* showCharacters)
{
    // Number of arguments that are not options
    int notOptions = 0;
    
    // First argument is the path of the executable, so we ignore it
    for (int i = 1; i < argc; i++) {
        
        // Options start with -
        if (argv[i][0] != '-') notOptions++;
        
        else if (strcmp (argv[i], "-l") == 0) *showLines = 1;
        else if (strcmp (argv[i], "-w") == 0) *showWords = 1;
        else if (strcmp (argv[i], "-c") == 0) *showCharacters = 1;
        
    }
    
    // If no option has been entered
    if (!*showLines && !*showWords && !*showCharacters)
        // Set all options to true
        *showLines = *showWords = *showCharacters = 1;
    
    return notOptions;
}

int main (int argc, char* argv[])
{
    // Parse command line arguments
    int showLines, showWords, showCharacters;
    int actualArgs = parseOptions (argc, argv,
                                   &showLines, &showWords, &showCharacters);
    
    // Command line handling
    char* input;
    int fromStdIn;
    
    // No arguments passed, that means we read from stdin
    if (actualArgs == 0) {
        input = getInput (STDINPATH);
        fromStdIn = 1;
    }
    
    // For one argument passed, we see it as the path of a file
    else if (actualArgs == 1) {
        input = getInput (argv[1]);
        fromStdIn = 0;
    }
    
    // We only implement wc for one file or stdin
    // thus we only accept zero or one arguments
    else {
        printf ("Error! WC takes zero arguments for reading from the standard input \nor one argument that is the path of a file to read from\n");
        exit (-2);
    }
    
    // Process options
    if (showLines)      printf ("%d line(s) \n", nrOfLines (input, fromStdIn));
    if (showWords)      printf ("%d word(s) \n", nrOfWords (input));
    if (showCharacters) printf ("%d character(s) \n", nrOfCharacters (input, fromStdIn));
    
    
    free (input);
    return 0;
}

