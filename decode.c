
#include "common.h"
#include "types.h"
#include <stdio.h>
#include <string.h>
#include "decode.h"
#include <stdlib.h>
#include <unistd.h>

/* function for do decoding and check all status of function calls */
Status do_decoding(DecodeInfo *decInfo)
{
    /* opening files for decoding */
    if(open_decode_files(decInfo) == d_success)
    {
	printf("INFO: Open_decode_files is SUCCESS\n");
    }
    else
    {
	printf("INFO: Open_decode_files is Failure\n");
	return d_failure;
    }

    /* decoding magic string */
    if(decode_magic_string(decInfo) == d_success)
    {
	printf("INFO: Magic string is decoded successfully\n");
    }
    else
    {
	printf("INFO: Magic string is not decoded successfully\n");
	return d_failure;
    }
    //function calling for decoding the file extention size and file size
    if(decode_file_extn_size(decInfo) == d_success)
    {
	printf("INFO: Decode Secret File Extenstion Size is Success\n");
    }
    else
    {
	printf("ERROR: Decode Secret File Extenstion Size is Failure\n");
	return d_failure;
    }
    //function calling for decoding the secret file extentiom & validating the condition
    if(decode_extn_secret_file(decInfo->extn_size,decInfo) == d_success)
    {

	printf("INFO: Decoding Extenstion File is Success\n");

    }
    else
    {
	printf("ERROR: Decoding Extenstion File is Failure\n");
	return d_failure;
    }
    //function calling for decoding secret file size & validating the condition
    if(decode_secret_file_size(decInfo) == d_success)
	printf("INFO: Decoding Secret File size is Success\n");
    else
    {
	printf("ERROR: Decoding Secret File Size is Failure\n");
	return d_failure;
    }

    //function calling for decoding the secret file data
    if(decode_secret_file(decInfo->file_size,decInfo) == d_success)
	printf("INFO: Decoding Secret File  is Success\n");
    else
    {
	printf("ERROR: Decoding Secret File is Failure\n");
	return d_failure;
    }
    return d_success;
}
/* Read and validate decode args from argv*/
Status read_and_validate_decode_args(char argc,char *argv[],DecodeInfo *decInfo)
{
    //check more than 2 arguments was passed or not
    if(argc >2)
    {
	//check bmp file
    if(strcmp(strstr(argv[2],"."),".bmp") == 0)  

	decInfo->stego_image_fname = argv[2]; 
   //if file not found send as failed message 
    else
    {
	printf("ERROR: Please enter the .bmp file\n");
	return d_failure;
    }
    }
    //if arguments less than 2 print error message
    else
    {
        printf("INFO: ./lsb_steg: Deconding: ./lsb_steg -d <.bmp file> [output file]\n");
	return d_failure;
    }
    if(argv[3] != NULL)
    {
	//assign argv[3] to output fname 
	strcpy(decInfo->output_fname,argv[3]); 
    }
    else
	strcpy(decInfo->output_fname,"output");//create default file name

    return d_success;
}

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decInfo)
{
    /*open the .bmp file*/
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname,"r");
    /*Error handling*/
    if(decInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr,"ERROR : Unable to open file %s\n",decInfo->stego_image_fname);
	return d_failure;
    }
    return d_success;

}

/* Store Magic String*/
Status decode_magic_string(DecodeInfo *decInfo)
{
    char magic_string[3] = {0};
    //int i = strlen(MAGIC_STRING);

    char buffer[8];
    /*Skip the 54 bytes*/
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);

    for(int i=0; i<2; i++)
    {
	fread(buffer,1,8,decInfo -> fptr_stego_image);

	magic_string[i] = decode_byte_from_lsb(magic_string[i],buffer);
    }
    magic_string[3] = '\0';

    if(strcmp(magic_string,MAGIC_STRING) == 0)
	return d_success;
    else
	return d_failure;
}


/* Decode a byte into LSB of image data array
 * Description: To decoding the secret data each bit of #(magic string) from 1 byte of LSb data
 */
Status decode_byte_from_lsb(char data ,char *image_buffer)
{
    int bit = 7;
    unsigned char ch = 0x00;
    for (int i = 0; i < 8; i++)
    {
	//image buffer and with 1 shift bit-- times and or with ch 
	ch = ((image_buffer[i] & 0x01) << bit--) | ch; 
    }
    //return ch 
    return ch;
}

//function definition for encode size
Status decode_file_extn_size(DecodeInfo *decInfo) 
{
    char str[32];
    int size = 0;
    int bit = 31;

    fread(str, 32, 1, decInfo->fptr_stego_image);   //reading the image buffer

    for(int i=0;i<32;i++)
    {
	//get size from below operation
	size = ((str[i] & 1) << bit-- ) | size;
    }

    decInfo->extn_size = size;    //store  in size

    //checking if it is .txt or .c or .h or .sh
    if(decInfo->extn_size == strlen(".txt"))
	return d_success;
    else if(decInfo->extn_size == strlen(".c"))
	return d_success;
    else if(decInfo->extn_size == strlen(".h"))
	return d_success;
    else if(decInfo->extn_size == strlen(".sh"))
	return d_success;
    else
	return d_failure;
}

/* Decode secret file extenstion */
Status decode_extn_secret_file(int size,DecodeInfo *decInfo)
{

    char extn_secret_file[size+1];
    char buffer[8];
    for(int i = 0;i<size;i++)
    {
	extn_secret_file[i] = 0;
        //read stego image
	fread(buffer,1,8,decInfo->fptr_stego_image);

	extn_secret_file[i] = decode_byte_from_lsb(extn_secret_file[i],buffer);
    }
    extn_secret_file[size] = '\0';
    //combine secret file extension to given output fname
    strcat(decInfo->output_fname,extn_secret_file);
    decInfo->fptr_output =fopen(decInfo->output_fname,"w");
    if(decInfo->fptr_output == NULL)
    {
	perror("fopen");
	fprintf(stderr,"ERROR : unable to open file %s\n",decInfo->output_fname);
	return d_failure;
    }
    return d_success; //returning success

}

//function for decode secret file size
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char str[32];
    int size =0;
    int bit = 31;
    fread(str,32,1,decInfo->fptr_stego_image);

    for(int i=0;i<32;i++)
    {
	size = ((str[i] & 1)  << bit -- )| size;
    }
    decInfo-> file_size = size;
    return d_success;
}

/* Decode secret file data
 * Input: secret file size (structure), stego image, decoded file
 * Output: decoded secret file data
 * Return Value: e_success after decoded
 */
Status decode_secret_file(int size,DecodeInfo *decInfo)
{
    char str[8];

    for(int i=0;i< size ;i++)
    {
	fread(str , 8 , 1, decInfo->fptr_stego_image);
	decode_file(str,decInfo);
	fwrite(str,1,1,decInfo->fptr_output);
    }
    //secret file decoded succesfully
    return d_success;
}
//function decode file
Status decode_file(char *str,DecodeInfo *decInfo)
{
    char ch =0;
    int bit = 7;
    for(int i=0;i<8;i++)
    {
	ch = ((str[i] & 1) << bit--) | ch;
    }
    str[0] = ch;
    return d_success;
}


