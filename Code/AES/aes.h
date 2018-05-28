#ifndef _cross_aes_h_
#define _cross_aes_h_

#define AES_BLOCK_SIZE 16
#define AES_KEY_BITS   128
#define AES_MAXNR 14

struct aes_key_st
{
	unsigned int rd_key[4 * (AES_MAXNR + 1)];
	int rounds;
	aes_key_st()
	{
		rounds = 10;
		for (int i = 0; i < (AES_MAXNR + 1); i++)
		{
			rd_key[i] = 0;
		}
	}
};

typedef struct aes_key_st T_AES_KEY;

struct aes_st
{
	T_AES_KEY stEncKey;
	T_AES_KEY stDecKey;
};
typedef struct aes_st T_AES;

int AES_set_encrypt_key(const unsigned char *userKey, const int bits, T_AES_KEY *key);
int AES_set_decrypt_key(const unsigned char *userKey, const int bits, T_AES_KEY *key);

void AES_cbc_encrypt(const unsigned char *in, unsigned char *out,
	const unsigned long length, const T_AES_KEY *key,
	const unsigned char *ivec, const int enc);

int taes_128_cbc_pkcs7_encrypt_len(int nInBufLen);
int taes_128_cbc_pkcs7_encrypt(const char* pInBuf, int nInBufLen, const char* pKey, int nKeyLen, unsigned char* pOutBuf);
int taes_128_cbc_pkcs7_decrypt(const char* pInBuf, int nInBufLen, const char* pKey, int nKeyLen, unsigned char* pOutBuf, int* nOutBufLen);

#endif