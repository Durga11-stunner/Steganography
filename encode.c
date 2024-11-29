#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include"common.h"
/* Function Definitions */
//check encoding process
Status do_encoding(EncodeInfo *encInfo)
{
    
    if(open_files(encInfo)==e_success)
    {
	printf("INFO: Open files successfully\n");
    }
    else
    {
	printf("ERROR: Open files is failure\n");
	return e_failure;
    } 
    if(check_capacity(encInfo)==e_success)
    {
	printf("INFO: Check capicity is success\n");
    }
    else
    {
	printf("ERROR: Check capacity is failure\n");
	return e_failure;
    }   
    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success)
    {
	printf("INFO: Copy bmp header is success\n");
    }
    else
    {
	printf("ERROR: Copying bmp header is not successful\n");
	return e_failure;
    }
    //check magic string is encoded or not
    if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)
    {
	printf("INFO: Encode Magic string is success:\n");
    }
    else
    {
	printf("ERROR: Encode Magic string is fail\n");
	return e_failure;
    }
    //check secret file extn size is done or not
    if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    {
	printf("INFO: Secret file data is encoded\n");
    }
    else
    {
	printf("ERROR: Secret file data is not encoded\n");
	return e_failure;
    }
    //check secret file extension is encoded or not
    if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo)==e_success)
    {
	printf("INFO: Secret file extention is encoded\n");
    }
    else
    {
	printf("ERROR: Secret file extention is not encoded\n");
	return e_failure;
    }
    //check secret file size is done or not
    if(encode_secret_file_size(encInfo->size_secret_file, encInfo ) == e_success)
    {
	printf("INFO: secret file size is encoded\n");   
    }
    else
    {
	printf("ERROR: secret file size is not encoded\n");   
	return e_failure;
    }
    //check secret file data is encoded or not
    if(encode_secret_file_data(encInfo) == e_success)
    {
	printf("INFO: Secret file data is encoded\n");
    }
    else
    {
	printf("ERROR: Secret file data is not encoded\n");
	return e_failure;
    }
    //check remaining data in image is copied or not
    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
	printf("INFO: Remaining image data is copied\n");
	return e_success;
    }
    else
    {
	printf("ERROR: Remaining image data is not copied\n");
	return e_failure;
    }
}

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("INFO: width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("INFO: height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}


uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    printf("INFO: Opening required files\n");
    
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    printf("INFO: Opened beautiful.bmp\n");
    
    if (encInfo->fptr_src_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

	return e_failure;
    }

    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    printf("INFO: Opened secret.txt\n");
    if (encInfo->fptr_secret == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

	return e_failure;
    }

    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    printf("INFO: Opened default.bmp\n");
    if (encInfo->fptr_stego_image == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

	return e_failure;
    }
    return e_success;
}
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e") == 0)
	return e_encode;
    else if(strcmp(argv[1],"-d") == 0)
	return e_decode;
    else
	return e_unsupported;

}
Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo)
{
    if(argc >2)
    {
	if(strcmp(strstr(argv[2],"."), ".bmp") == 0)
	    encInfo->src_image_fname=argv[2];
	else
	    return e_failure;
    }
    else
    {
	printf("INFO: ./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt file> [Output file]\n");
	return e_failure;
    }
    
    if(argc>3)
    {
	
	if(strcmp(strstr(argv[3],"."), ".txt") == 0)
	{
	    encInfo->secret_fname=argv[3];
	    strcpy(encInfo->extn_secret_file,".txt");
	}
	
	else if(strcmp(strstr(argv[3],"."),".c") == 0)
	{
	    
	    encInfo->secret_fname = argv[3];

	    
	    strcpy(encInfo->extn_secret_file,".c");

	}
	
	else if(strcmp(strstr(argv[3],"."),".h") ==0 )
	{
	    encInfo->secret_fname = argv[3];

	    strcpy(encInfo->extn_secret_file,".h");

	}
	
	else if(strcmp(strstr(argv[3],"."),".sh") == 0)
	{
	    encInfo->secret_fname = argv[3];

	    strcpy(encInfo->extn_secret_file,".sh");
	}
	else
	{
	    return e_failure;
	}

    }
    
    else
    {
	printf("INFO: ./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt file> [Output file]\n");
	return e_failure;
    }

    
    if(argv[4]==NULL)
    {
	
	encInfo->stego_image_fname="default.bmp";
	printf("INFO: Output File not mentioned. Creating default.bmp\n");
    }
    else
	encInfo->stego_image_fname=argv[4];
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    int image_capacity;
    int size_secret_file;
   
    image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    if(image_capacity > (54 + 16 + 32+ 32 + 8* size_secret_file))  
	return e_success;
    else
	return e_failure;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char str[54];

    fseek(fptr_src_image, 0L, SEEK_SET);

    fread(str, 54, 1, fptr_src_image);

    fwrite(str, 54, 1, fptr_dest_image);
    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image((char*) magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}

//Function defintion for encoding data to output image file
//Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char str[8];
    for(int i = 0; i < size; i++)
    {
	fread(str, 8, 1, fptr_src_image);
	encode_byte_to_lsb(data[i], str);
	fwrite(str, 8, 1, fptr_stego_image);
    }
    return e_success;
}

Status encode_byte_to_lsb(char data, char *str)
{ 
    for(int i = 0; i < 8; i++)
    {
	if((data >> (7 - i) & 1))
	{
	    str[i] = str[i] | 1;
	}
	else
	    str[i] = str[i] & ~1;
    }

}

Status encode_size_to_lsb(int size, char *image_buffer)
{
    unsigned int data = 8;
    for(int i = 0; i < 32; i++)
    {
	if((size >> (31 - i)) & 1)
	{
	    image_buffer[i] = image_buffer[i]  | 1;
	}
	else
	    image_buffer[i] = image_buffer[i] & ~1;
    }
}

Status encode_secret_file_extn_size(int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char str[32];
    fread(str, 32, 1, fptr_src_image);
    encode_size_to_lsb(size, str);
    fwrite(str, 32, 1, fptr_stego_image);
    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image((char *)file_extn, strlen(file_extn), encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_size(long int size, EncodeInfo *encInfo)
{
    char str[32];

    fread(str, 32, 1, encInfo->fptr_src_image);
    encode_size_to_lsb( size, str);
    fwrite(str, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}


Status encode_secret_file_data(EncodeInfo *encInfo)
{
    fseek(encInfo->fptr_secret, 0, SEEK_SET);

    char str[encInfo->size_secret_file];

    fread(str, encInfo->size_secret_file, 1, encInfo->fptr_secret);
    encode_data_to_image(str, strlen(str), encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while(( fread ( &ch, 1, 1, fptr_src) ) > 0)
    {
	fwrite( &ch, 1, 1, fptr_dest);
    }
    return e_success;
}

