#ifndef LIBC_H
#define LIBC_H

extern int  Libc_sprintf(char *str, const char *fmt, ...);
extern double Libc_strtod(const char *s00, char **se);
extern int  Libc_stricmp(const char* dst, const char* src);
extern bool Libc_atoi64(const char* num, ClrInt64& res);
extern bool Libc_atoi64(int type, const char* pNum, ClrInt64& val);
extern bool Libc_i64toa(ClrInt64& val, char* pBuffer, unsigned length, int base);

#ifdef PLATFORM_QSORT_SUPPORT
extern void qsort(void *base, unsigned num, unsigned width, int (*comp)(const void *, const void *));
#endif

#ifdef PLATFORM_BSEARCH_SUPPORT
extern void* bsearch(const void *key, const void *base, size_t num, size_t width, int (*comp)(const void*, const void*));
#endif

#endif //#define LIBC_H
