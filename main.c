/*Description:LSB Image Steganography 
Sample Input:1. ./a.out -e beautiful.bmp secret.txt
             2. ./a.out -d default.bmp secret_msg.txt
Sample Output:Encoding
               INFO: ## Encoding procedure started ##
               INFO: Output File not mentioned. Creating default.bmp
               INFO: Opening required files
               INFO: Opened beautiful.bmp
               INFO: Opened secret.txt
               INFO: Opened default.bmp
               INFO: Open files successfully
               INFO: width = 1024
               INFO: height = 768
               INFO: Check capicity is success
               INFO: Copy bmp header is success
               INFO: Encode Magic string is success:
               INFO: Secret file data is encoded
	       INFO: Secret file extention is encoded
               INFO: secret file size is encoded
               INFO: Secret file data is encoded
               INFO: Remaining image data is copied
               INFO: ## Encoding Done Successfully

	      Decoding
	       INFO: ## Decoding Procedure Started ##
               INFO: Read and validation of decode is success
               INFO: Open_decode_files is SUCCESS
               INFO: Magic string is decoded successfully
               INFO: Decode Secret File Extenstion Size is Success
               INFO: Decoding Extenstion File is Success
               INFO: Decoding Secret File size is Success
               INFO: Decoding Secret File  is Success
               INFO: ## Decoding Done Successfully ##
// Decoded information to secret_msg.txt file
 */
#include<stdio.h>
#include"types.h"
#include"common.h"
#include"encode.h"
#include"decode.h"
int main(int argc, char **argv)
{
    int status;
    if(argc==1)
    {
	printf("INFO: Please pass arguments through command line \n");
	printf("INFO: ./a.out -e <.bmp> <.txt> [optional stego.bmp]\n");
	printf("INFO: ./a.out -d <.bmp> [optional decode.txt]\n");
    }
    else
    {
	status=check_operation_type(argv);
	if(status==e_encode)
	{
	    printf("INFO: ## Encoding procedure started ##\n");
	    EncodeInfo encinfo;
	    if (read_and_validate_encode_args(argc,argv, &encinfo) == e_success)
	    {
		printf("INFO: Read and validation of encode is success\n"); 
	
		status=do_encoding(&encinfo);
		if(status==e_success)
		{
		    printf("INFO: ## Encoding Done Successfully ##\n");
		}
		else
		{
		    printf("INFO: Failed to Encode\n");
		}
	    }
	    else
	    {
		printf("ERROR: Read and validation is failed\n");
		printf("INFO: ./a.out -e <.bmp> <.txt> [optional stego.bmp]\n");
	    }
	}
	
	else if(status == e_decode)
	{
	    printf("INFO: ## Decoding Procedure Started ##\n");
	    DecodeInfo decInfo;
	    
	    if(read_and_validate_decode_args(argc,argv, &decInfo) == d_success)
	    {
		printf("INFO: Read and validation of decode is success\n");
	
		if(do_decoding(&decInfo)== d_success)
		{
		    printf("INFO: ## Decoding Done Successfully ##\n");
		}
		else
		{
		    printf("INF0: Failed to Decode\n");
		}
	    }
	    
	    else
	    {
		printf("ERROR: Read and validation is failed\n");
		printf("INFO: ./a.out -d <.bmp> [optional decode.txt]\n");
	    }
	}
	
	else
	{
	    printf("INFO: ./a.out -e <.bmp> <.txt> [optional stego.bmp]\n");
	    printf("INFO: ./a.out -d <.bmp> [optional decode.txt]\n");
	}
    }
    return 0;
}
