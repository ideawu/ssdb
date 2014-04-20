/**domain socket from redis source code.**/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>

#define ANET_OK 0
#define ANET_ERR -1
#define ANET_ERR_LEN 256
#define ANET_CONNECT_NONE 0
#define ANET_CONNECT_NONBLOCK 1

int anetUnixServer(char *err, const char *path, mode_t perm);
int anetUnixConnect(char *err, const char *path);
int anetUnixNonBlockConnect(char *err, char *path);
int anetUnixAccept(char *err, int s);

int anetUnixGenericConnect(char *err, const char *path, int flags);
int anetNonBlock(char *err, int fd);
void anetSetError(char *err, const char *fmt, ...);
int anetGenericAccept(char *err, int s, struct sockaddr *sa, socklen_t *len) ;
int anetListen(char *err, int s, struct sockaddr *sa, socklen_t len) ;
int anetCreateSocket(char *err, int domain);
