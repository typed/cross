#include "Aes/aes.h"
#include <iostream>

int main(int argc, char** argv)
{
    // check usage
    if (argc != 2) {
        fprintf(stderr, "%s <plain text>\n", argv[0]);
        exit(-1);
    }

	char* input_string = argv[1];
	int src_len = strlen(input_string);

	char* key =		"Abcdefgh12345678";
	int key_len = strlen(key);

	int enc_len = taes_128_cbc_pkcs7_encrypt_len(src_len);
	unsigned char* enc_str = new unsigned char[enc_len];
	taes_128_cbc_pkcs7_encrypt(input_string, src_len, key, key_len, enc_str);

	int dec_len = 0;
	char* dec_str = new char[enc_len];
	taes_128_cbc_pkcs7_decrypt((const char*)enc_str, enc_len, key, key_len, (unsigned char*)dec_str, &dec_len);

	printf("input_string = %s\n", input_string);
    printf("encrypted string = ");
    for (int i = 0; i < enc_len; ++i) {
        printf("%x%x", (enc_str[i] >> 4) & 0xf, enc_str[i] & 0xf);    
    }
	printf("decrypted string = %s\n", dec_str);
    printf("\n");
	delete[] enc_str;
	delete[] dec_str;
    return 0;
}