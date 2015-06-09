#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <utmpx.h>
#include <fcntl.h>
#include <ftw.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0

#define NO "n\n"
#define YES "y\n"

#define A 'a'
#define B 'b'
#define N 'n'
#define smallR 'r'
#define bigR 'R'
#define F 'f'
#define H 'h'
#define I 'i'
#define S 's'
#define L 'l'

#define MAX_SIZE 100
#define CHAR_PERM_SIZE 11
#define TIME_SIZE 20