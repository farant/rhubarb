/* http_r_servus.h - servus HTTP localis pro sondis http_r* (fork) */
#ifndef HTTP_R_SERVUS_H
#define HTTP_R_SERVUS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* auscultat in 127.0.0.1:0; redde portum, *fd impletur */
static int auscultare(int* fd)
{
    struct sockaddr_in a;
    socklen_t lon = sizeof(a);
    int unus = 1;
    *fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(*fd, SOL_SOCKET, SO_REUSEADDR, &unus, sizeof(unus));
    memset(&a, 0, sizeof(a));
    a.sin_family = AF_INET;
    a.sin_addr.s_addr = htonl(0x7f000001);
    a.sin_port = 0;
    if (bind(*fd, (struct sockaddr*)&a, sizeof(a)) != 0) { perror("bind"); exit(3); }
    listen(*fd, 8);
    getsockname(*fd, (struct sockaddr*)&a, &lon);
    return ntohs(a.sin_port);
}

static void effundere(const char* titulus, const char* s, int n)
{
    int k;
    printf("  [%s] petitio recepta (%d octeti):\n    ", titulus, n);
    for (k = 0; k < n; k++)
    {
        if (s[k] == '\r') printf("\\r");
        else if (s[k] == '\n') printf("\\n\n    ");
        else putchar(s[k]);
    }
    printf("\n");
    fflush(stdout);
}

/* filius: n connexiones accipit; pro quaque petitionem legit (usque ad
 * CRLFCRLF + mora brevis), effundit, moram_ms dormit, responsum mittit */
static pid_t servire(int fd, const char* titulus, const char** responsa,
                     const int* morae_ms, int n)
{
    pid_t pid;
    fflush(stdout);
    pid = fork();
    if (pid == 0)
    {
        int k;
        for (k = 0; k < n; k++)
        {
            char buf[16384];
            int tot = 0;
            int c = accept(fd, NULL, NULL);
            struct timeval tv;
            if (c < 0) _exit(4);
            tv.tv_sec = 0; tv.tv_usec = 300000;
            setsockopt(c, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
            for (;;)
            {
                ssize_t r = recv(c, buf + tot, sizeof(buf) - 1 - (size_t)tot, 0);
                if (r <= 0) break;
                tot += (int)r;
                buf[tot] = 0;
                if (strstr(buf, "\r\n\r\n")) { /* paulo plus pro corpore */
                    tv.tv_sec = 0; tv.tv_usec = 100000;
                    setsockopt(c, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
                }
            }
            effundere(titulus, buf, tot);
            if (morae_ms && morae_ms[k] > 0) usleep((useconds_t)morae_ms[k] * 1000);
            send(c, responsa[k], strlen(responsa[k]), 0);
            close(c);
        }
        _exit(0);
    }
    return pid;
}

static void exspectare(pid_t pid)
{
    int st;
    kill(pid, SIGTERM);
    waitpid(pid, &st, 0);
}

static double nunc_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
}
#endif
