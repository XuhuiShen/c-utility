#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "types.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <dlfcn.h>
#include <assert.h>

#define CACHE_LINE_SIZE 64
#define cache_aligned(exp)															\
				exp __attribute__ ((aligned (CACHE_LINE_SIZE)))

#define alignof(x) __alignof__(x)

#define print_size(x) printf(" sizeof " #x": %lu\n", sizeof(x))
#define print_align(x) printf("alignof " #x": %lu\n\n", alignof(x))

void *xmalloc(size_t size);
void *xcalloc(size_t nmemb, size_t size);

#define swap(x, y) ({ typeof(x) __tmp = (x); (x) = (y); (y) = __tmp; })

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)


#define info(format, args...)										\
				do {																		\
								printf(format, ## args);				\
								fflush(stdout);									\
				} while (0)

#define debug(format, args...)																			\
				do {																												\
								fprintf(stderr, "%s(): "format, __func__, ## args);	\
								fflush(stderr);																			\
				} while (0)

#define error(args...)													\
				do {																		\
								debug("error: "args);						\
								exit(-1);												\
				} while (0)

#define warn(args...) debug("warning: "args)

#define container_of(ptr, type, member) ({															\
												const typeof( ((type *)0)->member ) *__mptr = (ptr); \
												(type *)( (char *)__mptr - offsetof(type, member) );})

void load_user_so(int start_i, int end_i, char *argv[]);
void *open_so(const char *path);
void *dlsym_safe(void *so_handle, const char *sym);
char *dirname(const char *path);

static inline size_t find_char(char c, const char *buf, size_t len)
{
				for (size_t i = 0; i < len; i++)
								if (buf[i] == c)
												return i;
				return len;
}

static inline u64 parse_hex(const char *str, size_t len)
{
				u64 x = 0;
				for (size_t i = 0; i < len; i++) {
								int c = str[i];
								if (c >= '0' && c <= '9')
												c -= '0';
								else if (c >= 'a' && c <= 'f')
												c = c - 'a' + 10;
								else if (c >= 'A' && c <= 'F')
												c = c - 'A' + 10;
								else
												assert(0);
								x = x * 16 + c;
				}
				return x;
}

static inline void parse_hex_string(u8 *buf, const char *str, size_t len)
{
        for (size_t i = 0; i < len / 2; i++)
								buf[i] = (u8)parse_hex(&str[2 * i], 2);
}

static inline void to_hex_string(char *str, const u8 *data, u64 data_bytes)
{
				const char *map = "0123456789abcdef";
				for (u64 i = 0; i < data_bytes; i++) {
								u8 c = data[i];
								str[i * 2] = map[c >> 4];
								str[i * 2 + 1] = map[c & 0xf];
				}
}

static inline void print_string(const char *prompt, const char buf[], size_t len)
{
				printf("%s", prompt);
				for (size_t i = 0; i < len; i++)
								printf("%c", buf[i]);
				printf("\n");
}

#endif // __UTILITY_H__
