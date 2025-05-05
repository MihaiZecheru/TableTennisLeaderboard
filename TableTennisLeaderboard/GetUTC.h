#ifndef GET_UTC_H
#define GET_UTC_H

String GetUTC()
{
  time_t now;
  time(&now);

  struct tm* utc_tm = gmtime(&now); // break into UTC time fields

  char time_str[20];
  sprintf(time_str, "%04d-%02d-%02d %02d:%02d:%02d",
    utc_tm->tm_year + 1900,
    utc_tm->tm_mon + 1,
    utc_tm->tm_mday,
    utc_tm->tm_hour,
    utc_tm->tm_min,
    utc_tm->tm_sec
);

  return String(time_str);
}


#endif