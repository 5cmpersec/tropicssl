/**
 * \file bignum.h
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
#ifndef TROPICSSL_BIGNUM_H
#define TROPICSSL_BIGNUM_H

#include "tropicssl/config.h"
#if defined(TROPICSSL_BIGNUM)
#if defined(TROPICSSL_SELF_TEST) || defined(TROPICSSL_FS_IO)
#include <stdio.h>
#endif

#include <string.h>
#include <inttypes.h>

#define MPI_CHK(f) if( ( ret = f ) != 0 ) goto cleanup

/*
 * Maximum size MPIs are allowed to grow to in number of limbs.
 */
#define TROPICSSL_MPI_MAX_LIMBS                             10000

/*
 * Define the base integer type, architecture-wise
 */
#if defined(TROPICSSL_HAVE_INT8)
typedef   int8_t  t_sint;
typedef uint8_t  t_uint;
typedef uint16_t       t_udbl;
#else
#if defined(TROPICSSL_HAVE_INT16)
typedef  int16_t t_sint;
typedef uint16_t t_uint;
typedef uint32_t t_udbl;
#else
  typedef  int32_t t_sint;
  typedef uint32_t t_uint;
  #if ( defined(_MSC_VER) && defined(_M_IX86) )      || \
      ( defined(__GNUC__) && (                          \
        defined(__amd64__) || defined(__x86_64__)    || \
        defined(__ppc64__) || defined(__powerpc64__) || \
        defined(__ia64__)  || defined(__alpha__)     || \
        (defined(__sparc__) && defined(__arch64__))  || \
        defined(__s390x__) ) )
      #define TROPICSSL_HAVE_INT64
  #endif
  #if defined(TROPICSSL_HAVE_INT64)
    typedef uint64_t t_udbl;
  #endif
#endif /* TROPICSSL_HAVE_INT16 */
#endif /* TROPICSSL_HAVE_INT8  */

/**
 * \brief          MPI structure
 */
typedef struct {
	int s;			/*!<  integer sign      */
	size_t n;		/*!<  total # of limbs  */
	t_uint *p;		/*!<  pointer to limbs  */
} mpi;

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * \brief          Initialize one or more mpi
	 */
	void mpi_init(mpi * X, ...);

	/**
	 * \brief          Unallocate one or more mpi
	 */
	void mpi_free(mpi * X, ...);

	/**
	 * \brief          Enlarge to the specified number of limbs
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
	 */
	int mpi_grow(mpi * X, size_t nblimbs);

	/**
	 * \brief          Copy the contents of Y into X
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
	 */
	int mpi_copy(mpi * X, const mpi * Y);

	/**
	 * \brief          Swap the contents of X and Y
	 */
	void mpi_swap(mpi * X, mpi * Y);

	/**
	 * \brief          Set value from integer
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
	 */
	int mpi_lset(mpi * X, t_sint z);

	/**
	 * \brief          Return the number of least significant bits
	 */
	size_t mpi_lsb(const mpi * X);

	/**
	 * \brief          Return the number of most significant bits
	 */
	size_t mpi_msb(const mpi * X);

	/**
	 * \brief          Return the total size in bytes
	 */
	size_t mpi_size(const mpi * X);

	/**
	 * \brief          Import from an ASCII string
	 *
	 * \param X        destination mpi
	 * \param radix    input numeric base
	 * \param s        null-terminated string buffer
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful, or an TROPICSSL_ERR_MPI_XXX error code
	 */
	int mpi_read_string(mpi * X, int radix, const char *s);

	/**
	 * \brief          Export into an ASCII string
	 *
	 * \param X        source mpi
	 * \param radix    output numeric base
	 * \param s        string buffer
	 * \param slen     string buffer size
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful, or an TROPICSSL_ERR_MPI_XXX error code
	 *
	 * \note           Call this function with *slen = 0 to obtain the
	 *                 minimum required buffer size in *slen.
	 */
	int mpi_write_string(const mpi * X, int radix, char *s, size_t *slen);

#if defined(TROPICSSL_FS_IO)
	/**
	 * \brief          Read X from an opened file
	 *
	 * \param X        destination mpi
	 * \param radix    input numeric base
	 * \param fin      input file handle
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful, or an TROPICSSL_ERR_MPI_XXX error code
	 */
	int mpi_read_file(mpi * X, int radix, FILE * fin);

	/**
	 * \brief          Write X into an opened file, or stdout
	 *
	 * \param p        prefix, can be NULL
	 * \param X        source mpi
	 * \param radix    output numeric base
	 * \param fout     output file handle
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful, or an TROPICSSL_ERR_MPI_XXX error code
	 *
	 * \note           Set fout == NULL to print X on the console.
	 */
	int mpi_write_file(const char *p, const mpi * X, int radix, FILE * fout);
#endif

	/**
	 * \brief          Import X from unsigned binary data, big endian
	 *
	 * \param X        destination mpi
	 * \param buf      input buffer
	 * \param buflen   input buffer size
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
	 */
	int mpi_read_binary(mpi * X, const uint8_t *buf, size_t buflen);

	/**
	 * \brief          Export X into unsigned binary data, big endian
	 *
	 * \param X        source mpi
	 * \param buf      output buffer
	 * \param buflen   output buffer size
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_BUFFER_TOO_SMALL if buf isn't large enough
	 *
	 * \note           Call this function with *buflen = 0 to obtain the
	 *                 minimum required buffer size in *buflen.
	 */
	int mpi_write_binary(const mpi * X, uint8_t *buf, size_t buflen);

	/**
	 * \brief          Left-shift: X <<= count
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
	 */
	int mpi_shift_l(mpi * X, size_t count);

	/**
	 * \brief          Right-shift: X >>= count
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
	 */
	int mpi_shift_r(mpi * X, size_t count);

	/**
	 * \brief          Compare unsigned values
	 *
	 * \return         1 if |X| is greater than |Y|,
	 *                -1 if |X| is lesser  than |Y| or
	 *                 0 if |X| is equal to |Y|
	 */
	int mpi_cmp_abs(const mpi * X, const mpi * Y);

	/**
	 * \brief          Compare signed values
	 *
	 * \return         1 if X is greater than Y,
	 *                -1 if X is lesser  than Y or
	 *                 0 if X is equal to Y
	 */
	int mpi_cmp_mpi(const mpi * X, const mpi * Y);

	/**
	 * \brief          Compare signed values
	 *
	 * \return         1 if X is greater than z,
	 *                -1 if X is lesser  than z or
	 *                 0 if X is equal to z
	 */
	int mpi_cmp_int(const mpi * X, t_sint z);

	/**
	 * \brief          Unsigned addition: X = |A| + |B|
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
	 */
	int mpi_add_abs(mpi * X, const mpi * A, const mpi * B);

	/**
	 * \brief          Unsigned substraction: X = |A| - |B|
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_NEGATIVE_VALUE if B is greater than A
	 */
	int mpi_sub_abs(mpi * X, const mpi * A, const mpi * B);

	/**
	 * \brief          Signed addition: X = A + B
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
	 */
	int mpi_add_mpi(mpi * X, const mpi * A, const mpi * B);

	/**
	 * \brief          Signed substraction: X = A - B
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
	 */
	int mpi_sub_mpi(mpi * X, const mpi * A, const mpi * B);

	/**
	 * \brief          Signed addition: X = A + b
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
	 */
	int mpi_add_int(mpi *X, const mpi * A, t_sint b);

	/**
	 * \brief          Signed substraction: X = A - b
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
	 */
	int mpi_sub_int(mpi *X, const mpi * A, t_sint b);

	/**
	 * \brief          Baseline multiplication: X = A * B
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
	 */
	int mpi_mul_mpi(mpi * X, const mpi * A, const mpi * B);

	/**
	 * \brief          Baseline multiplication: X = A * b
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
	 */
	int mpi_mul_int(mpi * X, const mpi * A, t_sint b);

	/**
	 * \brief          Division by mpi: A = Q * B + R
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed,
	 *                 TROPICSSL_ERR_MPI_DIVISION_BY_ZERO if B == 0
	 *
	 * \note           Either Q or R can be NULL.
	 */
	int mpi_div_mpi(mpi * Q, mpi * R, const mpi * A, const mpi * B);

	/**
	 * \brief          Division by int: A = Q * b + R
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed,
	 *                 TROPICSSL_ERR_MPI_DIVISION_BY_ZERO if b == 0
	 *
	 * \note           Either Q or R can be NULL.
	 */
	int mpi_div_int(mpi * Q, mpi * R, const mpi * A, t_sint b);

	/**
	 * \brief          Modulo: R = A mod B
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed,
	 *                 TROPICSSL_ERR_MPI_DIVISION_BY_ZERO if B == 0
	 *                 TROPICSSL_ERR_MPI_NEGATIVE_VALUE if B < 0
	 */
	int mpi_mod_mpi(mpi * R, const mpi * A, const mpi * B);

	/**
	 * \brief          Modulo: r = A mod b
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed,
	 *                 TROPICSSL_ERR_MPI_DIVISION_BY_ZERO if b == 0
	 *                 TROPICSSL_ERR_MPI_NEGATIVE_VALUE if b < 0
	 */
	int mpi_mod_int(t_uint * r, const mpi * A, t_sint b);

	/**
	 * \brief          Sliding-window exponentiation: X = A^E mod N
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed,
	 *                 TROPICSSL_ERR_MPI_BAD_INPUT_DATA if N is negative or even
	 *
	 * \note           _RR is used to avoid re-computing R*R mod N across
	 *                 multiple calls, which speeds up things a bit. It can
	 *                 be set to NULL if the extra performance is unneeded.
	 */
	int mpi_exp_mod(mpi * X, const mpi * A, const mpi * E, const mpi * N, mpi * _RR);

	/**
	 * \brief          Fill an MPI X with size bytes of random
	 *
	 * \param X        Destination MPI
	 * \param size     Size in bytes
	 * \param f_rng    RNG function
	 * \param p_rng    RNG parameter
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
	 */
	int mpi_fill_random(mpi * X, size_t size, int (*f_rng)(void *), void *p_rng);

	/**
	 * \brief          Greatest common divisor: G = gcd(A, B)
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed
	 */
	int mpi_gcd(mpi * G, const mpi * A, const mpi * B);

	/**
	 * \brief          Modular inverse: X = A^-1 mod N
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful,
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed,
	 *                 TROPICSSL_ERR_MPI_BAD_INPUT_DATA if N is negative or nil
	 *                 TROPICSSL_ERR_MPI_NOT_ACCEPTABLE if A has no inverse mod N
	 */
	int mpi_inv_mod(mpi * X, const mpi * A, const mpi * N);

	/**
	 * \brief          Miller-Rabin primality test
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful (probably prime),
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed,
	 *                 TROPICSSL_ERR_MPI_NOT_ACCEPTABLE if X is not prime
	 */
	int mpi_is_prime(mpi * X, int (*f_rng) (void *), void *p_rng);

	/**
	 * \brief          Prime number generation
	 *
	 * \param X        destination mpi
	 * \param nbits    required size of X in bits
	 * \param dh_flag  if 1, then (X-1)/2 will be prime too
	 * \param f_rng    RNG function
	 * \param p_rng    RNG parameter
	 *
	 * \return         TROPICSSL_ERR_MPI_OKAY if successful (probably prime),
	 *                 TROPICSSL_ERR_MPI_MALLOC_FAILED if memory allocation failed,
	 *                 TROPICSSL_ERR_MPI_BAD_INPUT_DATA if nbits is < 3
	 */
	int mpi_gen_prime(mpi * X, size_t nbits, int dh_flag,
			  int (*f_rng) (void *), void *p_rng);

#if defined(TROPICSSL_SELF_TEST)
	/**
	 * \brief          Checkup routine
	 *
	 * \return         0 if successful, or 1 if the test failed
	 */
	int mpi_self_test(int verbose);
#endif

#ifdef __cplusplus
}
#endif

#endif              /* TROPICSSL_BIGNUM */
#endif				/* bignum.h */
