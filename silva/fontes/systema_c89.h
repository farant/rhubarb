/* systema_c89.h - Superficies bibliothecae normae ISO C89
 *
 * PLAGULA CONSILII OFFICINAE (silva-semantica-design.md DECISUS 1,
 * officina-spec-v2.md): UNUM documentum = superficies externa
 * semanticae = superficies aedificatorum machinulae (M2 eandem
 * leget). Manu scripta semel, in aeternum versionata (exemplar
 * latina.h). VANILLA C89 - a silva ipsa parsatur (numquam a clang
 * compilatur; haruspex de typis systematis solum OPACITATEM
 * asserit, numquam interna - partitio provenientiae, consilium
 * par VIII).
 *
 * Formae typorum SYNTHETICAE sunt (FILE incompleta, jmp_buf
 * simplex): machinula aedificata numquam interna systematis
 * dereferat (regula M2 nominata). Typi ordine definiuntur ne
 * quid ante definitionem usurpetur - parsura sine ambiguis.
 */

/* ==================================================
 * Typi fundamentales (stddef.h)
 * ================================================== */

typedef unsigned long size_t;
typedef long          ptrdiff_t;
typedef int           wchar_t;

#define NULL ((void*)0)

/* ==================================================
 * stdarg.h (va_* magicae - machinula eas specialiter tractabit;
 * corpus variadica non definit - audit 2026-07-08)
 * ================================================== */

typedef char* va_list;

/* ==================================================
 * errno.h
 * ================================================== */

extern int errno;

#define EDOM   33
#define ERANGE 34

/* ==================================================
 * limits.h (praecipua)
 * ================================================== */

#define CHAR_BIT   8
#define SCHAR_MIN  (-128)
#define SCHAR_MAX  127
#define UCHAR_MAX  255
#define CHAR_MIN   SCHAR_MIN
#define CHAR_MAX   SCHAR_MAX
#define SHRT_MIN   (-32768)
#define SHRT_MAX   32767
#define USHRT_MAX  65535
#define INT_MIN    (-2147483647 - 1)
#define INT_MAX    2147483647
#define UINT_MAX   4294967295U
#define LONG_MIN   (-2147483647L - 1L)
#define LONG_MAX   2147483647L
#define ULONG_MAX  4294967295UL

/* ==================================================
 * ctype.h
 * ================================================== */

int isalnum(int c);
int isalpha(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);
int tolower(int c);
int toupper(int c);

/* ==================================================
 * string.h
 * ================================================== */

void*  memcpy(void* destinatio, const void* fons, size_t n);
void*  memmove(void* destinatio, const void* fons, size_t n);
char*  strcpy(char* destinatio, const char* fons);
char*  strncpy(char* destinatio, const char* fons, size_t n);
char*  strcat(char* destinatio, const char* fons);
char*  strncat(char* destinatio, const char* fons, size_t n);
int    memcmp(const void* a, const void* b, size_t n);
int    strcmp(const char* a, const char* b);
int    strcoll(const char* a, const char* b);
int    strncmp(const char* a, const char* b, size_t n);
size_t strxfrm(char* destinatio, const char* fons, size_t n);
void*  memchr(const void* s, int c, size_t n);
char*  strchr(const char* s, int c);
size_t strcspn(const char* s, const char* reicienda);
char*  strpbrk(const char* s, const char* accipienda);
char*  strrchr(const char* s, int c);
size_t strspn(const char* s, const char* accipienda);
char*  strstr(const char* s, const char* quaesitum);
char*  strtok(char* s, const char* separatores);
void*  memset(void* s, int c, size_t n);
char*  strerror(int errnum);
size_t strlen(const char* s);

/* ==================================================
 * stdio.h
 * ================================================== */

typedef struct _systema_FILE FILE;
typedef long fpos_t;

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

#define EOF          (-1)
#define BUFSIZ       1024
#define FILENAME_MAX 1024
#define FOPEN_MAX    20
#define L_tmpnam     1024
#define TMP_MAX      25
#define SEEK_SET     0
#define SEEK_CUR     1
#define SEEK_END     2
#define _IOFBF       0
#define _IOLBF       1
#define _IONBF       2

int    remove(const char* via);
int    rename(const char* vetus, const char* novum);
FILE*  tmpfile(void);
char*  tmpnam(char* s);
int    fclose(FILE* fluxus);
int    fflush(FILE* fluxus);
FILE*  fopen(const char* via, const char* modus);
FILE*  freopen(const char* via, const char* modus, FILE* fluxus);
void   setbuf(FILE* fluxus, char* buffer);
int    setvbuf(FILE* fluxus, char* buffer, int modus, size_t n);
int    fprintf(FILE* fluxus, const char* forma, ...);
int    fscanf(FILE* fluxus, const char* forma, ...);
int    printf(const char* forma, ...);
int    scanf(const char* forma, ...);
int    sprintf(char* s, const char* forma, ...);
int    sscanf(const char* s, const char* forma, ...);
int    vfprintf(FILE* fluxus, const char* forma, va_list arg);
int    vprintf(const char* forma, va_list arg);
int    vsprintf(char* s, const char* forma, va_list arg);
int    fgetc(FILE* fluxus);
char*  fgets(char* s, int n, FILE* fluxus);
int    fputc(int c, FILE* fluxus);
int    fputs(const char* s, FILE* fluxus);
int    getc(FILE* fluxus);
int    getchar(void);
char*  gets(char* s);
int    putc(int c, FILE* fluxus);
int    putchar(int c);
int    puts(const char* s);
int    ungetc(int c, FILE* fluxus);
size_t fread(void* datum, size_t magnitudo_elementi, size_t n,
    FILE* fluxus);
size_t fwrite(const void* datum, size_t magnitudo_elementi,
    size_t n, FILE* fluxus);
int    fgetpos(FILE* fluxus, fpos_t* positio);
int    fseek(FILE* fluxus, long offset, int unde);
int    fsetpos(FILE* fluxus, const fpos_t* positio);
long   ftell(FILE* fluxus);
void   rewind(FILE* fluxus);
void   clearerr(FILE* fluxus);
int    feof(FILE* fluxus);
int    ferror(FILE* fluxus);
void   perror(const char* s);

/* ==================================================
 * stdlib.h
 * ================================================== */

typedef struct { int quot; int rem; }   div_t;
typedef struct { long quot; long rem; } ldiv_t;

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define RAND_MAX     2147483647
#define MB_CUR_MAX   1

double        atof(const char* s);
int           atoi(const char* s);
long          atol(const char* s);
double        strtod(const char* s, char** finis);
long          strtol(const char* s, char** finis, int basis);
unsigned long strtoul(const char* s, char** finis, int basis);
int           rand(void);
void          srand(unsigned int semen);
void*         calloc(size_t n, size_t magnitudo_elementi);
void          free(void* monstrator);
void*         malloc(size_t magnitudo_petita);
void*         realloc(void* monstrator, size_t magnitudo_petita);
void          abort(void);
int           atexit(void (*functio)(void));
void          exit(int conditio);
char*         getenv(const char* titulus);
int           system(const char* imperium);
void*         bsearch(const void* clavis, const void* basis,
    size_t n, size_t magnitudo_elementi,
    int (*comparator)(const void*, const void*));
void          qsort(void* basis, size_t n,
    size_t magnitudo_elementi,
    int (*comparator)(const void*, const void*));
int           abs(int valor);
div_t         div(int numerator, int denominator);
long          labs(long valor);
ldiv_t        ldiv(long numerator, long denominator);
int           mblen(const char* s, size_t n);
int           mbtowc(wchar_t* pwc, const char* s, size_t n);
int           wctomb(char* s, wchar_t wc);
size_t        mbstowcs(wchar_t* pwcs, const char* s, size_t n);
size_t        wcstombs(char* s, const wchar_t* pwcs, size_t n);

/* ==================================================
 * math.h
 * ================================================== */

#define HUGE_VAL 1e308

double acos(double x);
double asin(double x);
double atan(double x);
double atan2(double y, double x);
double cos(double x);
double sin(double x);
double tan(double x);
double cosh(double x);
double sinh(double x);
double tanh(double x);
double exp(double x);
double frexp(double valor, int* exponens);
double ldexp(double x, int exponens);
double log(double x);
double log10(double x);
double modf(double valor, double* pars_integra);
double pow(double x, double y);
double sqrt(double x);
double ceil(double x);
double fabs(double x);
double floor(double x);
double fmod(double x, double y);

/* ==================================================
 * time.h
 * ================================================== */

typedef long clock_t;
typedef long time_t;

#define CLOCKS_PER_SEC 1000000L

struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
};

clock_t    clock(void);
double     difftime(time_t finis, time_t initium);
time_t     mktime(struct tm* tempus);
time_t     time(time_t* sedes);
char*      asctime(const struct tm* tempus);
char*      ctime(const time_t* tempus);
struct tm* gmtime(const time_t* tempus);
struct tm* localtime(const time_t* tempus);
size_t     strftime(char* s, size_t maximum, const char* forma,
    const struct tm* tempus);

/* ==================================================
 * setjmp.h
 * ================================================== */

typedef int jmp_buf[48];

int  setjmp(jmp_buf ambitus);
void longjmp(jmp_buf ambitus, int valor);

/* ==================================================
 * signal.h
 * ================================================== */

typedef int sig_atomic_t;
typedef void (*_systema_tractator_signi)(int);

#define SIG_DFL ((_systema_tractator_signi)0)
#define SIG_IGN ((_systema_tractator_signi)1)
#define SIG_ERR ((_systema_tractator_signi)-1)
#define SIGABRT 6
#define SIGFPE  8
#define SIGILL  4
#define SIGINT  2
#define SIGSEGV 11
#define SIGTERM 15

_systema_tractator_signi signal(int signum,
    _systema_tractator_signi tractator);
int raise(int signum);

/* ==================================================
 * locale.h
 * ================================================== */

#define LC_ALL      0
#define LC_COLLATE  1
#define LC_CTYPE    2
#define LC_MONETARY 3
#define LC_NUMERIC  4
#define LC_TIME     5

struct lconv {
    char* decimal_point;
    char* thousands_sep;
    char* grouping;
    char* int_curr_symbol;
    char* currency_symbol;
    char* mon_decimal_point;
    char* mon_thousands_sep;
    char* mon_grouping;
    char* positive_sign;
    char* negative_sign;
    char  int_frac_digits;
    char  frac_digits;
    char  p_cs_precedes;
    char  p_sep_by_space;
    char  n_cs_precedes;
    char  n_sep_by_space;
    char  p_sign_posn;
    char  n_sign_posn;
};

char*         setlocale(int categoria, const char* locale);
struct lconv* localeconv(void);

/* ==================================================
 * assert.h (macro - machinula specialiter; hic pro completudine)
 * ================================================== */

void _systema_assertio_fracta(const char* expressio,
    const char* via, int linea);
