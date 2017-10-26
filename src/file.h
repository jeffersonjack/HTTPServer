#ifndef FILE_H
#define FILE_H

#define MAXFILENAME 255   /* 255 is the max file name length on EXT FS */
#define DIR_OK      1
#define DIR_NOSLASH 2

/* get the size of the file (in bytes) of the file at the specified (relative)
   path. -1 will be returned if the file does not exist. */
int filesize(const char *path);

/* return 1 if 'path' is a directory, 0 if it is a normal file, and -1 if the
   file doesn't exist */
int directory(const char *path);

/* put the contents of the file at the specified path into the buffer. */
char *getfilecontents(const char *path, char *buffer);

/* append the filename to the base directory (defined above) and store the
   result in the buffer provided */
void makefilepath(const char *file, char **buffer);

/* generate a directory listing of the directory at 'path' and store the result
   (in HTML form) in 'buffer'. On success, return the length of the buffer, on
   failure, return 0. */
int getdirlist(const char *path, char **buffer, int bsize);

#endif /* FILE_H */
