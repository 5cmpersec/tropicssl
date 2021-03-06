/**
 * \file sha4.h
 *
 *  Based on XySSL: Copyright (C) 2006-2008  Christophe Devine
 *
 *  Copyright (C) 2009  Paul Bakker <polarssl_maintainer at polarssl dot org>
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of PolarSSL or XySSL nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef TROPICSSL_SHA4_H
#define TROPICSSL_SHA4_H

#include "tropicssl/config.h"

#if defined(TROPICSSL_SHA4)
#if defined(_MSC_VER) || defined(__WATCOMC__)
#define UL64(x) x##ui64
#define int64 __int64
#else
#include <string.h>
#include <inttypes.h>

#define UL64(x) x##ULL
#endif

/**
 * \brief          SHA-512 context structure
 */
typedef struct {
	uint64_t total[2];	/*!< number of bytes processed  */
	uint64_t state[8];	/*!< intermediate digest state  */
	uint8_t buffer[128];	/*!< data block being processed */

	uint8_t ipad[128];	/*!< HMAC: inner padding        */
	uint8_t opad[128];	/*!< HMAC: outer padding        */
	int is384;		/*!< 0 => SHA-512, else SHA-384 */
} sha4_context;

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * \brief          SHA-512 context setup
	 *
	 * \param ctx      context to be initialized
	 * \param is384    0 = use SHA512, 1 = use SHA384
	 */
	void sha4_starts(sha4_context * ctx, int is384);

	/**
	 * \brief          SHA-512 process buffer
	 *
	 * \param ctx      SHA-512 context
	 * \param input    buffer holding the  data
	 * \param ilen     length of the input data
	 */
	void sha4_update(sha4_context * ctx, const uint8_t *input, size_t ilen);

	/**
	 * \brief          SHA-512 final digest
	 *
	 * \param ctx      SHA-512 context
	 * \param output   SHA-384/512 checksum result
	 */
	void sha4_finish(sha4_context * ctx, uint8_t output[64]);

	/**
	 * \brief          Output = SHA-512( input buffer )
	 *
	 * \param input    buffer holding the  data
	 * \param ilen     length of the input data
	 * \param output   SHA-384/512 checksum result
	 * \param is384    0 = use SHA512, 1 = use SHA384
	 */
	void sha4(const uint8_t *input, size_t ilen,
		  uint8_t output[64], int is384);

#if defined(TROPICSSL_FS_IO)
	/**
	 * \brief          Output = SHA-512( file contents )
	 *
	 * \param path     input file name
	 * \param output   SHA-384/512 checksum result
	 * \param is384    0 = use SHA512, 1 = use SHA384
	 *
	 * \return         0 if successful, 1 if fopen failed,
	 *                 or 2 if fread failed
	 */
	int sha4_file(const char *path, uint8_t output[64], int is384);
#endif

	/**
	 * \brief          SHA-512 HMAC context setup
	 *
	 * \param ctx      HMAC context to be initialized
	 * \param is384    0 = use SHA512, 1 = use SHA384
	 * \param key      HMAC secret key
	 * \param keylen   length of the HMAC key
	 */
	void sha4_hmac_starts(sha4_context * ctx, const uint8_t *key,
			      size_t keylen, int is384);

	/**
	 * \brief          SHA-512 HMAC process buffer
	 *
	 * \param ctx      HMAC context
	 * \param input    buffer holding the  data
	 * \param ilen     length of the input data
	 */
	void sha4_hmac_update(sha4_context * ctx, const uint8_t *input,
			      size_t ilen);

	/**
	 * \brief          SHA-512 HMAC final digest
	 *
	 * \param ctx      HMAC context
	 * \param output   SHA-384/512 HMAC checksum result
	 */
	void sha4_hmac_finish(sha4_context * ctx, uint8_t output[64]);

	/**
	 * \brief          Output = HMAC-SHA-512( hmac key, input buffer )
	 *
	 * \param key      HMAC secret key
	 * \param keylen   length of the HMAC key
	 * \param input    buffer holding the  data
	 * \param ilen     length of the input data
	 * \param output   HMAC-SHA-384/512 result
	 * \param is384    0 = use SHA512, 1 = use SHA384
	 */
	void sha4_hmac(const uint8_t *key, size_t keylen,
		       const uint8_t *input, size_t ilen,
		       uint8_t output[64], int is384);

#if defined(TROPICSSL_SELF_TEST)
	/**
	 * \brief          Checkup routine
	 *
	 * \return         0 if successful, or 1 if the test failed
	 */
	int sha4_self_test(int verbose);
#endif

#ifdef __cplusplus
}
#endif

#endif              /* TROPICSSL_SHA4 */
#endif				/* sha4.h */
