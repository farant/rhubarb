/* systema_posix.h - Supplementum POSIX/Darwin superficiei systematis
 *
 * PLAGULA CONSILII OFFICINAE (M2d): supplementum ad systema_c89.h -
 * oneratores OFFICINAE SOLI (cursor, fusor) textus CONCATENANT in
 * parsuram UNAM (systema_c89.h + haec plagula). Silva-latus (percursus,
 * haruspex) plagulam ISO solam onerat - superficies normae pura manet,
 * baselines silvae immotae.
 *
 * FORMAE HIC VERAE SUNT (non syntheticae!): membra structurarum
 * dereferantur ab interpretatis et monstratores hospiti VERO
 * traduntur (stat/gettimeofday aedificata scribunt per formam
 * hospitis). Ergo forma quaeque contra capita vera clang certificatur
 * - PORTA auspex_posix (officina/auspex_posix.sh). Mensurae Darwin
 * arm64, inodus 64-bit: stat 144, timeval 16, timespec 16 octeti.
 *
 * Typi iam in systema_c89.h (size_t, time_t, FILE...) NON iterantur -
 * concatenatio redefinitionem vetat.
 */

/* ==================================================
 * Typi fundamentales POSIX (Darwin)
 * ================================================== */

typedef long               intptr_t;      /* stdint.h - C99, corpus */
typedef int                dev_t;
typedef unsigned short     mode_t;
typedef unsigned short     nlink_t;
typedef unsigned long long ino_t;         /* inodus 64-bit */
typedef unsigned int       uid_t;
typedef unsigned int       gid_t;
typedef long long          off_t;
typedef long long          blkcnt_t;
typedef int                blksize_t;
typedef int                suseconds_t;   /* tv_usec 32-bit! */

/* ==================================================
 * sys/time.h
 * ================================================== */

struct timespec {
    time_t tv_sec;
    long   tv_nsec;
};

struct timeval {
    time_t      tv_sec;
    suseconds_t tv_usec;
};

int gettimeofday(struct timeval* tp, void* tzp);

/* ==================================================
 * sys/stat.h (forma Darwin arm64, 144 octeti - certificata)
 * ================================================== */

struct stat {
    dev_t           st_dev;
    mode_t          st_mode;
    nlink_t         st_nlink;
    ino_t           st_ino;
    uid_t           st_uid;
    gid_t           st_gid;
    dev_t           st_rdev;
    struct timespec st_atimespec;
    struct timespec st_mtimespec;
    struct timespec st_ctimespec;
    struct timespec st_birthtimespec;
    off_t           st_size;
    blkcnt_t        st_blocks;
    blksize_t       st_blksize;
    unsigned int    st_flags;
    unsigned int    st_gen;
    int             st_lspare;
    long long       st_qspare[2];
};

#define S_IFMT  0170000
#define S_IFDIR 0040000
#define S_IFREG 0100000

#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)

#define S_IRUSR 0400
#define S_IWUSR 0200
#define S_IXUSR 0100

int stat(const char* via, struct stat* sedes);
int mkdir(const char* via, mode_t modus);

/* ==================================================
 * unistd.h (pars parva - Unda 1; opendir/readdir = Unda 1b nomine)
 * ================================================== */

char* getcwd(char* sedes, size_t mensura);
int   unlink(const char* via);
