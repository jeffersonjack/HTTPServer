#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <ctype.h>
#include <fcntl.h>

#include "server.h"

int main(int argc, char **argv)
{
  int portno, sock, c, uid, pid;
  struct passwd *pw;
  char *endp, directory[256];

  portno = 80;
  memset(directory, '\0', 256);

  /* make sure the server was started correctly
  if (argc == 1) {
    portno = 80;
    printf("port\n");
  }*/

  /* turn off option error printing */
  opterr = 0;

  while ((c = getopt(argc, argv, "p:d:h")) != -1) {
    switch (c) {
      case 'p':
        /* check we were given a number */
        portno = strtol (optarg, &endp, 10);
        if (*endp != '\0') {
          fprintf (stderr, "Usage: %s [-p port] [-d directory]\n", *argv);
          return 1;
        }
        break;
      case 'h':
        /* print usage and exit */
        fprintf(stderr, "Usage: %s [-p port] [-d directory]\n", *argv);
        return 0;
      case 'd':
        /* set directory */
        strncpy(directory, optarg, 255);
        break;
      case '?':
        /* invalid option */
        fprintf(stderr, "Usage: %s [-p port] [-d directory]\n", *argv);
        return 1;
    }
  }


  /* if we don't have a port number by now, the usage was incorrect */
  if (portno < 1) {
    fprintf(stderr, "Usage: %s [-p port] [-d directory]\n", *argv);
    return 1;
  }

  /* check we were given a valid port number */
  if ((portno < 1) || (portno > 65535)) {
    fprintf(stderr, "Error: invalid port number\n");
    return 1;
  }

  /* fork #1 */
  if ((pid = fork()) < 0) {
    perror("fork");
    return 1;
  }

  if (pid == 0) {
    /* in child process */

    /* detatch from controlling tty */
    if (setsid() < 0) {
      perror("setsid");
      return 1;
    }

    /* who are we? */
    uid = getuid();

    /* get a socket to listen on and listen on it */
    sock = getlisteningsocket(portno);

    if (uid == 0) {
      /* we're root, which is dangerous; change user! */
      //pw = malloc(sizeof(struct passwd));
      if ((pw = getpwnam("httpd")) == NULL) {
        /* try a different user */
        if ((pw = getpwnam("http")) == NULL) {
          /* try a different user */
          if ((pw = getpwnam("daemon")) == NULL) {
            fprintf(stderr, "Error: couldn't relinquish root permissions\n");
            /* exit */
            return 1;
          }
        }
      }
      if (setgid(pw->pw_gid) == -1) {
        perror("setgid");
        fprintf(stderr, "Error: couldn't relinquish root permissions\n");
        return 1;
      }
      if (setuid(pw->pw_uid) == -1) {
        perror("setuid");
        fprintf(stderr, "Error: couldn't relinquish root permissions\n");
        return 1;
      }
    }

    /* fork #2 */
    if ((pid = fork()) < 0) {
      perror("fork");
      return 1;
    }

    if (pid == 0) {
      /* in child process */

      /* if a directory was specified on the command line, attempt to change to that directory. */
      if (directory[0] && directory[255] == '\0') {
          if (chdir(directory) < 0)
            perror("chdir");
      }
      else if (uid == 0)
        if (chdir("/var/www/") < 0)
          perror("chdir");

      acceptconnections(sock);
    }
    else {
      /* in parent process - exit successfully */
      return 0;
    }
  }
  else {
    /* in parent process - exit successfully */
    return 0;
  }
}
