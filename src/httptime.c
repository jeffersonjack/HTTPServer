#include <time.h>

#include "http.h"

void httptime(time_t *secs, char *buffer)
{
  struct tm *tm = gmtime(secs);
  /* date/time format, as specified in RFC7231 */
  char format[] = "%a, %d %b %Y %H:%M:%S %Z";
  
  strftime(buffer, TIMESTRLEN, format, tm);
}
