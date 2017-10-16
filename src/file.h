#ifndef FILE_H
#define FILE_H

#define BASEDIR "/home/jack/Documents/Uni/Year3/networks/Assignments/HTTPServer/www"

/* get the size of the file (in bytes) of the file at the specified (relative)
   path. -1 will be returned if the file does not exist. */
int filesize(const char *path);

/* put the contents of the file at the specified path into the buffer. */
char *getfilecontents(const char *path, char *buffer);

/* append the filename to the base directory (defined above) and store the
   result in the buffer provided */
void makefilepath(const char *file, char **buffer);

#endif /* FILE_H */
