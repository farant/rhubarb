/* systema_posix.h - Supplementum POSIX/Darwin superficiei systematis
 *
 * Supplementum ad systema_c89.h. Duo modi consumptionis:
 * (1) oneratores officinae (cursor, fusor) plagulam TOTAM
 * concatenant in parsuram unam; (2) superficies iudicii tres
 * (examen, percursus, legatus - design B 2026-07-27) sectiones
 * DERIVANT ex inclusionibus plagulae iudicatae ipsius - marca
 * sectio caput= infra quam plagulam evocet dicit, semper= basim
 * omnibus dat, poscit= dependentias trahit.
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
 * <sectio semper="verum"/>
 * Typi fundamentales POSIX (Darwin) - BASIS omnium sectionum
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
 * <sectio caput="sys/time.h"/>
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
 * <sectio caput="sys/stat.h" poscit="sys/time.h"/>
 * sys/stat.h (forma Darwin arm64, 144 octeti - certificata).
 * poscit: struct timespec (st_atimespec et cetera).
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

/* nomina compatibilitatis POSIX (Darwin: macra ad timespec) */
#define st_atime st_atimespec.tv_sec
#define st_mtime st_mtimespec.tv_sec
#define st_ctime st_ctimespec.tv_sec

int stat(const char* via, struct stat* sedes);
int mkdir(const char* via, mode_t modus);

/* ==================================================
 * <sectio caput="unistd.h"/>
 * unistd.h (pars parva - Unda 1; opendir/readdir Unda 1b nomine -
 * IMPLETA in Unda 3, sectio dirent.h infra)
 * ================================================== */

char* getcwd(char* sedes, size_t mensura);
int   unlink(const char* via);

/* ==================================================
 * UNDA 2 (tractus hospitii 2026-07-20): fossae + signa + processus.
 * Valores macrorum Darwin certificati per auspex_posix (porta eadem
 * quae formas certificat - valor falsus in syscallum verum flueret).
 * ================================================== */

/* <sectio semper="verum"/> Typi Unda 2 - BASIS: socklen_t sectionem
 * socketorum tenet, ssize_t unistd, pid_t signal/wait */
typedef int          pid_t;
typedef long         ssize_t;
typedef unsigned int socklen_t;
typedef unsigned int useconds_t;

/* <sectio caput="unistd.h"/> unistd.h - processus + I/O crudum */
pid_t   fork(void);
int     pipe(int* fossae);
ssize_t read(int fossa, void* sedes, size_t mensura);
ssize_t write(int fossa, const void* datum, size_t mensura);
int     close(int fossa);
void    _exit(int status);
int     usleep(useconds_t microsecunda);
int     dup2(int fossa_vetus, int fossa_nova);
int     execvp(const char* plagula, char* const argumenta[]);

/* <sectio caput="stdlib.h"/> stdlib.h POSIX (getenv in systema_c89.h
 * ut C89; hae POSIX solae) */
int setenv(const char* nomen, const char* valor, int superscribere);
int unsetenv(const char* nomen);
int putenv(char* par);

/* <sectio caput="unistd.h"/> unistd.h - fossae normales */
#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

/* <sectio caput="signal.h"/> signal.h (valores signorum communes) */
#define SIGINT  2
#define SIGKILL 9
#define SIGALRM 14
#define SIGTERM 15

int kill(pid_t processus, int signum);

/* <sectio caput="sys/wait.h"/> sys/wait.h - macrota ut functiones
 * declarata (forma sufficit; silva signaturam petit, non
 * expansionem) */
#define WNOHANG   1     /* Darwin: 0x00000001 */
#define WUNTRACED 2     /* Darwin: 0x00000002 */

pid_t waitpid(pid_t processus, int* status, int optiones);
int WIFEXITED(int status);
int WEXITSTATUS(int status);
int WIFSIGNALED(int status);
int WTERMSIG(int status);

/* <sectio caput="fcntl.h"/> fcntl.h (valores Darwin) */
#define O_NONBLOCK 0x0004
#define O_WRONLY   0x0001
#define O_CREAT    0x0200
#define O_TRUNC    0x0400
#define F_GETFL    3
#define F_SETFL    4
#define F_SETFD    2
#define F_GETFD    1
#define FD_CLOEXEC 1

int fcntl(int fossa, int mandatum, ...);
int open(const char* via, int vexilla, ...);

/* <sectio caput="sys/select.h,sys/time.h" poscit="sys/time.h"/>
 * sys/select.h - fd_set opacum (silva membra non tangit; FD_*
 * ut functiones declarata, ut sys/wait.h supra).
 * poscit: struct timeval (parametrum morae in select).
 * caput alterum sys/time.h: Darwin select/fd_set etiam per
 * sys/time.h praebet (mos BSD) - tessera_pons ita includit. */
typedef struct { int __opacum[32]; } fd_set;

void FD_ZERO(fd_set* copia);
void FD_SET(int fossa, fd_set* copia);
void FD_CLR(int fossa, fd_set* copia);
int  FD_ISSET(int fossa, fd_set* copia);

int select(int numerus, fd_set* legendi, fd_set* scribendi,
           fd_set* errantes, struct timeval* mora);

/* <sectio caput="errno.h"/> errno.h (valores Darwin communes; errno
 * ipsum in systema_c89.h) */
#define EINTR       4
#define EAGAIN      35
#define EWOULDBLOCK 35

/* <sectio caput="sys/socket.h,netinet/in.h,netinet/tcp.h"/>
 * sys/socket.h + netinet/in.h + netinet/tcp.h (valores Darwin) */
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

/* promotio ex blocis tcp_posix (01KYB2Z7XA, 2026-07-27) -
 * transcriptio verbatim, formae Darwin certificatae */
#define AF_UNSPEC   0
#define AF_INET     2
#define SOCK_STREAM 1
#define SO_RCVTIMEO 0x1006
#define SO_SNDTIMEO 0x1005
#define INADDR_ANY  0u

struct sockaddr {
    unsigned char sa_len;
    unsigned char sa_family;
    char          sa_data[14];
};

struct in_addr {
    unsigned int s_addr;
};

struct sockaddr_in {
    unsigned char  sin_len;
    unsigned char  sin_family;
    unsigned short sin_port;
    struct in_addr sin_addr;
    char           sin_zero[8];
};

unsigned short htons(unsigned short valor);
unsigned int   htonl(unsigned int valor);
unsigned short ntohs(unsigned short valor);

int socket(int genus, int forma, int protocollum);
int bind(int fossa, const struct sockaddr* titulus, socklen_t mensura);
int listen(int fossa, int agmen);
int accept(int fossa, struct sockaddr* titulus, socklen_t* mensura);
int connect(int fossa, const struct sockaddr* titulus,
            socklen_t mensura);
int getsockname(int fossa, struct sockaddr* titulus,
                socklen_t* mensura);
int getpeername(int fossa, struct sockaddr* titulus,
                socklen_t* mensura);
ssize_t send(int fossa, const void* buffer, size_t mensura,
             int vexilla);
ssize_t recv(int fossa, void* buffer, size_t mensura, int vexilla);

/* <sectio caput="arpa/inet.h"/> arpa/inet.h */
const char* inet_ntop(int genus, const void* fons, char* destinatio,
                      socklen_t mensura);

/* <sectio caput="netdb.h" poscit="sys/socket.h"/> netdb.h
 * (forma Darwin: ai_canonname ANTE ai_addr - contra Linucem!
 * certificatur; poscit: struct sockaddr) */
struct addrinfo {
    int              ai_flags;
    int              ai_family;
    int              ai_socktype;
    int              ai_protocol;
    socklen_t        ai_addrlen;
    char*            ai_canonname;
    struct sockaddr* ai_addr;
    struct addrinfo* ai_next;
};

int getaddrinfo(const char* hospes, const char* servitium,
                const struct addrinfo* consilia,
                struct addrinfo** fructus);
void freeaddrinfo(struct addrinfo* fructus);
const char* gai_strerror(int status);

/* <sectio caput="poll.h"/> poll.h (promotio ex bloco reactoris) */
#define POLLIN  0x0001
#define POLLOUT 0x0004
#define POLLERR 0x0008
#define POLLHUP 0x0010

typedef unsigned int nfds_t;

struct pollfd {
    int   fd;
    short events;
    short revents;
};

int poll(struct pollfd* fossae, nfds_t numerus, int mora);

/* ==================================================
 * UNDA 3 (re-pinnatio exclusionum 2026-07-27): dirent + mman +
 * termios + ioctl + utime + supplementa stdio/unistd/signal/errno.
 * Classes ex classificatione XXXVIII REICE mensuratae: dirent
 * XXIV plagulas sanat (ansa ambulationis corporis ubique eadem).
 * Formae contra capita vera per auspex_posix certificatae.
 * ================================================== */

/* <sectio caput="dirent.h"/> dirent.h (forma Darwin, inodus 64-bit;
 * DIR opacum - monstrator solus traditur, magnitudo numquam) */
typedef struct _telldir DIR;

struct dirent {
    ino_t              d_ino;
    unsigned long long d_seekoff;
    unsigned short     d_reclen;
    unsigned short     d_namlen;
    unsigned char      d_type;
    char               d_name[1024];
};

#define DT_DIR 4

DIR*           opendir(const char* via);
struct dirent* readdir(DIR* dir);
int            closedir(DIR* dir);

/* <sectio caput="sys/mman.h"/> sys/mman.h (valores Darwin) */
#define PROT_READ   0x01
#define PROT_WRITE  0x02
#define MAP_PRIVATE 0x0002
#define MAP_ANON    0x1000
#define MAP_FAILED  ((void*)-1)

void* mmap(void* sedes, size_t mensura, int tutela, int vexilla,
           int fossa, off_t offset);
int   munmap(void* sedes, size_t mensura);

/* <sectio caput="termios.h"/> termios.h (forma Darwin arm64:
 * tcflag_t = unsigned long, NCCS 20) */
struct termios {
    unsigned long c_iflag;
    unsigned long c_oflag;
    unsigned long c_cflag;
    unsigned long c_lflag;
    unsigned char c_cc[20];
    unsigned long c_ispeed;
    unsigned long c_ospeed;
};

#define TCSAFLUSH 2
#define ISIG      0x00000080
#define VINTR     8
#define VQUIT     9
#define VMIN      16
#define VTIME     17

#define _POSIX_VDISABLE 0xff

int  tcgetattr(int fossa, struct termios* modus);
int  tcsetattr(int fossa, int quando, const struct termios* modus);
void cfmakeraw(struct termios* modus);

/* <sectio caput="sys/ioctl.h"/> sys/ioctl.h (winsize + TIOCGWINSZ) */
struct winsize {
    unsigned short ws_row;
    unsigned short ws_col;
    unsigned short ws_xpixel;
    unsigned short ws_ypixel;
};

#define TIOCGWINSZ 0x40087468UL

int ioctl(int fossa, unsigned long petitio, ...);

/* <sectio caput="utime.h"/> utime.h */
struct utimbuf {
    time_t actime;
    time_t modtime;
};

int utime(const char* via, const struct utimbuf* tempora);

/* <sectio caput="stdio.h"/> stdio.h POSIX (FILE* ex fossa) */
FILE* fdopen(int fossa, const char* modus);

/* <sectio caput="unistd.h"/> unistd.h - supplementa Undae 3 */
int dup(int fossa);
int isatty(int fossa);

/* <sectio caput="signal.h"/> signal.h POSIX (sigaction + signa
 * terminalia; sig_atomic_t/signal/raise in systema_c89.h ut C89) */
typedef unsigned int sigset_t;

struct sigaction {
    void     (*sa_handler)(int);
    sigset_t sa_mask;
    int      sa_flags;
};

#define SIGBUS   10
#define SIGCONT  19
#define SIGTSTP  18
#define SIGWINCH 28

int sigaction(int signum, const struct sigaction* actio,
              struct sigaction* pristina);
int sigemptyset(sigset_t* copia);

/* <sectio caput="errno.h"/> errno.h - supplementa Undae 3 */
#define EEXIST 17
