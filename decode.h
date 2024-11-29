
#ifndef DECODE_H
#define DECODE_H

#include "types.h" // user defined types
#include "common.h" // user defined types
#include <string.h> // user defined types
#include <stdio.h>
/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_BUF_SIZE 8

typedef struct decodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    char image_data[MAX_BUF_SIZE];
    int extn_size;

    int file_size;
    /* output file name */
    char output_fname[20];
    FILE *fptr_output;
} DecodeInfo;


// Decoding functions

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char argc, char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo);

/* Store Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

Status decode_file_extn_size(DecodeInfo *decInfo);

/* Decode a byte ifrom LSB of image data array */
Status decode_byte_from_lsb(char data , char *image_buffer);

/* Decode secret file extenstion */
Status decode_extn_secret_file(int size,DecodeInfo *decInfo);

Status decode_secret_file_size(DecodeInfo *decinfo);

/* Decode secret file data*/
Status decode_secret_file(int size,DecodeInfo *decInfo);

Status decode_file(char *str, DecodeInfo *decInfo);

#endif


