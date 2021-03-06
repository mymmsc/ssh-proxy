#ifndef SSH_LIBCRYPTO_H
#define SSH_LIBCRYPTO_H

#include "ssh-includes.h"

#ifdef HAVE_LIBCRYPTO

#include <openssl/dsa.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/hmac.h>
#ifdef HAVE_OPENSSL_ECC
#include <openssl/evp.h>
#endif

typedef SHA_CTX* SHACTX;
typedef SHA256_CTX* SHA256CTX;
typedef MD5_CTX*  MD5CTX;
typedef HMAC_CTX* HMACCTX;
#ifdef HAVE_ECC
typedef EVP_MD_CTX *EVPCTX;
#else
typedef void *EVPCTX;
#endif

#define SHA_DIGEST_LEN SHA_DIGEST_LENGTH
#ifdef MD5_DIGEST_LEN
    #undef MD5_DIGEST_LEN
#endif
#define MD5_DIGEST_LEN MD5_DIGEST_LENGTH

#ifdef HAVE_OPENSSL_ECC
#define EVP_DIGEST_LEN EVP_MAX_MD_SIZE
#endif

#include <openssl/bn.h>
#include <openssl/opensslv.h>
#define OPENSSL_0_9_7b 0x0090702fL
#if (OPENSSL_VERSION_NUMBER <= OPENSSL_0_9_7b)
#define BROKEN_AES_CTR
#endif
typedef BIGNUM*  bignum;
typedef BN_CTX* bignum_CTX;

#define bignum_new() BN_new()
#define bignum_free(num) BN_clear_free(num)
#define bignum_set_word(bn,n) BN_set_word(bn,n)
#define bignum_bin2bn(bn,datalen,data) BN_bin2bn(bn,datalen,data)
#define bignum_bn2dec(num) BN_bn2dec(num)
#define bignum_dec2bn(bn,data) BN_dec2bn(data,bn)
#define bignum_bn2hex(num) BN_bn2hex(num)
#define bignum_rand(rnd, bits, top, bottom) BN_rand(rnd,bits,top,bottom)
#define bignum_ctx_new() BN_CTX_new()
#define bignum_ctx_free(num) BN_CTX_free(num)
#define bignum_mod_exp(dest,generator,exp,modulo,ctx) BN_mod_exp(dest,generator,exp,modulo,ctx)
#define bignum_num_bytes(num) BN_num_bytes(num)
#define bignum_num_bits(num) BN_num_bits(num)
#define bignum_is_bit_set(num,bit) BN_is_bit_set(num,bit)
#define bignum_bn2bin(num,ptr) BN_bn2bin(num,ptr)
#define bignum_cmp(num1,num2) BN_cmp(num1,num2)

SHA256CTX sha256_init(void);
void sha256_update(SHA256CTX c, const void *data, unsigned long len);
void sha256_final(unsigned char *md, SHA256CTX c);

struct ssh_cipher_struct *ssh_get_ciphertab(void);

#endif /* HAVE_LIBCRYPTO */

#endif /* ! SSH_LIBCRYPTO_H */
