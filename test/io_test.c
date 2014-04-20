/*
Team Name: Team++
Project 2: io_test.c
CMPS 111: Spring 2014

CREATED (4-19-2014)
*/

#include <stdio.h>
#include <stdlib.h>

/*
int chksum(char *str, size_t length): Calculates the checksum of a buffer of bytes.
Arguments: (1) char *buffer - Buffer to be checksummed.
           (2) size_t length - The size of the buffer. Should be 1024 bytes or less.
*/
int chksum(char *buffer, size_t length)
{
    char *byte = buffer; /* Pointer to each byte in the buffer. */
    int checksum = 0; /* Will hold the checksum for this buffer segment. */
    size_t i; /* Loop counter. */

    for (i = 0; i < length; ++i) {
        checksum += (int) (*byte++); /* Add each byte to the checksum keeping a running total, and increment the pointer. */
    }

    return checksum; /* Return the checksum to the caller. */
}

/*
main(int argc, char *argv[]): The entry point.
*/
int main (int argc, char *argv[]) {
    int i; /* Loop counter. */
    FILE *in_file, *out_file; /* Input and output files. */
    size_t in_length; /* Holds the length of the in_file in bytes. */
    char buffer[1024]; /* Load 1024 byte segments of the in_file to this buffer. */
    int checksum = 0; /* The total checksum to be printed at the end. */

    if (argc < 3) { /* Main usage check. */
        printf("usage: io_test <input file> <output file>\n");
        exit(0);
    }

    if ( !(in_file = fopen(argv[1], "rb")) ) { /* Open the in_file for reading. */
        printf("io_test - failed to open input file\n");
        exit(1);
    }

    if ( !(out_file = fopen(argv[2], "wb")) ) { /* Open the out_file for writing. */
        printf("io_test - failed to open output file\n");
        exit(1);
    }

    while (!feof(in_file)) { /* While we have not reached the end of the in_file. */
        in_length = fread(buffer, sizeof(char), sizeof(buffer), in_file); /* Read 1024 bytes of in_file to the buffer. */
        fwrite(buffer , sizeof(char), sizeof(buffer), out_file); /* Write 1024 bytes of what we just read to out_file. */
        
        checksum += chksum(buffer, in_length); /* Calculate the checksum for this buffer segment and keep a running total. */

        for (i = 0; i < 1024; ++i) {
            buffer[i] = 0; /* Clear the buffer! */
        }
    }

    printf("Checksum: %#x\n", checksum); /* Print the checksum. */

    fclose(in_file); /* Close input file. */
    fclose(out_file); /* Close output file. */

    return 0;
}