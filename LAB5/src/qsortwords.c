/******************************************************************************
 * (c) 2010-2020 IST AED Team
 * Last modified: abl 2020-04-21
 *
 * NAME
 *   sortwords.c
 *
 * DESCRIPTION
 *   Main program for unique word finding and sorting with tables
 *
 * COMMENTS
 *   Code for preliminary distribution, to be completed
 ******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "word.h"

enum sort_order {ascending, descending};

enum sort_criteria {alphabetic, length, occurrences};


int OP_CNT = 0;     /* global variable, to simplify complexity assessment */


/******************************************************************************
 * sort ()
 *
 * Arguments: a - table of items to sort
 *            l, r - limits on table to consider
 *            less - item comparison function
 * Returns: (void)
 * Side-Effects: table is sorted in place
 *
 * Description: implements "some" sorting algorithm for abstract type (Item),
 *       using also an abstract type comparison function (*less)
 *           a[] - array of abstract type Item to be sorted
 *           l - index of first element in the array to be sorted
 *           r - last element of the array to be sorted
 *           (*less)(Item,Item) - abstract type comparison function
 *****************************************************************************/

/* Lomuto partition scheme */
void sort(Item arr[], int l, int r, int (*less) (Item, Item))
{
    int i, j;
    Item pivot, tmp;

    if (l >= r || l < 0) return;
    pivot = arr[r];
    OP_CNT++;
    /* Temporary pivot index */
    i = l - 1;
    for (j = l ; j < r - 1; j++) {

        /* If the current element is less than or equal to the pivot */
        if (less(arr[j], pivot)) {

            /* Move the temporary pivot index forward */
            i = i + 1;

            /* Swap the current element with the element at the temporary pivot index */
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            OP_CNT += 4;
        }
        OP_CNT++;
    }
    /* Move the pivot element to the correct pivot position (between the smaller and larger elements) */
    i = i + 1;
    tmp = arr[r];
    arr[r] = arr[i];
    arr[i] = tmp;
    OP_CNT += 4;

    sort(arr, l, i - 1, less); /* Left side of pivot */
    sort(arr, i + 1, r, less); /* Right side of pivot */

    return;
}


/******************************************************************************
* main ()
*
*****************************************************************************/

int main(int argc, char **argv)
{
    int TnumWords;              /* total number of words in input file */
    int numWords;               /* number of distint words in input file */
    WordTab wordtab;              /* table holding the structure */
    FILE *out;

    /* default initialization for alfabetic order and ascending */
    enum sort_criteria criterio = alphabetic;
    enum sort_order sentido  = ascending;
    char *file = argv[1];

    argv++;             /* get past prog and file names */
    while (*(++argv)) {
        if (strcmp(*argv, "-alfabetica") == 0)
            criterio = alphabetic;
        else if (strcmp(*argv, "-ocorrencias") == 0)
            criterio = occurrences;
        else if (strcmp(*argv, "-comprimento") == 0)
            criterio = length;
        else if (strcmp(*argv, "-a") == 0)
            sentido = ascending;
        else if (strcmp(*argv, "-d") == 0)
            sentido = descending;
        else {
            fprintf(stderr, "Wrong arguments\n");
            exit(3);
        }
    }
    TnumWords = AllocWordArray(&wordtab, file);
    printf("Total number of words: %d\n", TnumWords);

    numWords = FillInWordArray(wordtab, file);
    printf("Number of different words = %d\n", numWords);

    OP_CNT = 0;

    /*  Call the sorting function using as argument the
       appropriate comparison function selected by user option */

    if ((criterio == alphabetic) && (sentido == ascending)) {

        /*==== TODO ====*/
        /* -- sort(....); -- */
        sort((void **)wordtab, 0, numWords - 1, &LessAlphabetic);

    } else if ((criterio == length) && (sentido == ascending)) {
        sort((void **)wordtab, 0, numWords - 1, &LessLength);
    } else if ((criterio == occurrences) && (sentido == ascending)) {
        sort((void **)wordtab, 0, numWords - 1, &LessNumUses);
    }
    /* other user options */
    /*==== TODO ====*/

    /* ---------------------------------------- */
    printf("Accesses count for sort: %d\n", OP_CNT);

    WriteFile(wordtab, file, numWords);
    /*  printf("Number of different words: %d\n", n_palavras);  */

    out = fopen("out_qsortwords.csv", "a");
    if (!out) goto skip_csv;

    fprintf(out, "%i,%i\n", numWords, OP_CNT);

    fclose(out);

    /* -- Insert code to call functions to free allocated memory -- */
skip_csv:

    /*==== TODO ====*/
    FreeWordArray(&wordtab, numWords);

    /* ------------------------------------------------------------ */

    return 0;
}
