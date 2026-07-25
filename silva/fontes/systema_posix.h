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

/* ==================================================
 * UNDA 2 (tractus hospitii 2026-07-20): fossae + signa + processus.
 * Valores macrorum Darwin certificati per auspex_posix (porta eadem
 * quae formas certificat - valor falsus in syscallum verum flueret).
 * ================================================== */

/* Typi Unda 2 */
typedef int          pid_t;
typedef long         ssize_t;
typedef unsigned int socklen_t;
typedef unsigned int useconds_t;

/* unistd.h - processus + I/O crudum */
pid_t   fork(void);
int     pipe(int* fossae);
ssize_t read(int fossa, void* sedes, size_t mensura);
ssize_t write(int fossa, const void* datum, size_t mensura);
int     close(int fossa);
void    _exit(int status);
int     usleep(useconds_t microsecunda);
int     dup2(int fossa_vetus, int fossa_nova);
int     execvp(const char* plagula, char* const argumenta[]);

/* unistd.h - fossae normales */
#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

/* signal.h (valores signorum communes) */
#define SIGINT  2
#define SIGKILL 9
#define SIGTERM 15

int kill(pid_t processus, int signum);

/* sys/wait.h - macrota ut functiones declarata (forma sufficit;
 * silva signaturam petit, non expansionem) */
#define WNOHANG   1     /* Darwin: 0x00000001 */
#define WUNTRACED 2     /* Darwin: 0x00000002 */

pid_t waitpid(pid_t processus, int* status, int optiones);
int WIFEXITED(int status);
int WEXITSTATUS(int status);
int WIFSIGNALED(int status);
int WTERMSIG(int status);

/* fcntl.h (valores Darwin) */
#define O_NONBLOCK 0x0004
#define F_GETFL    3
#define F_SETFL    4
#define F_SETFD    2
#define F_GETFD    1
#define FD_CLOEXEC 1

int fcntl(int fossa, int mandatum, ...);

/* sys/select.h - fd_set opacum (silva membra non tangit; FD_*
 * ut functiones declarata, ut sys/wait.h supra) */
typedef struct { int __opacum[32]; } fd_set;

void FD_ZERO(fd_set* copia);
void FD_SET(int fossa, fd_set* copia);
void FD_CLR(int fossa, fd_set* copia);
int  FD_ISSET(int fossa, fd_set* copia);

int select(int numerus, fd_set* legendi, fd_set* scribendi,
           fd_set* errantes, struct timeval* mora);

/* errno.h (valores Darwin communes; errno ipsum in systema_c89.h) */
#define EINTR       4
#define EAGAIN      35
#define EWOULDBLOCK 35

/* sys/socket.h + netinet/in.h + netinet/tcp.h (valores Darwin) */
#define SOL_SOCKET   0xffff
#define SO_REUSEADDR 0x0004
#define SO_KEEPALIVE 0x0008
#define SO_NOSIGPIPE 0x1022
#define IPPROTO_TCP  6
#define TCP_NODELAY  0x01

int getsockopt(int fossa, int gradus, int titulus, void* valor,
               socklen_t* mensura);
int setsockopt(int fossa, int gradus, int titulus, const void* valor,
               socklen_t mensura);
