/*
  filemutex.cpp
  Grants exclusive access to process using file system
  Robert Almeida, 2006.02.12
  $Id: filemutex.cpp 1093 2009-09-15 20:04:42Z robert $
*/

#include <unistd.h>
#include <fcntl.h> 
#include <cerrno> 
#include <cstdio> 
#include <string>
#include "filemutex.h"

using namespace std;

const char* FileException::what()
  throw() {
  char s[128];
  snprintf(s, sizeof(msg)-1, "%s in file %s",
	   strerror(sys_errno), fileName.c_str());
  msg=s;
  return msg.c_str();
}

FileMutex::FileMutex(const string& fileName)
{
  this->fileName=fileName;
}

FileMutex::~FileMutex()
{
  if ( fd>0 ) {
    close(fd);
    remove(fileName.c_str());
  }
}

bool FileMutex::getLock(int timeout)
{
  int delay=200; // milliseconds
  do {
    fd=open(fileName.c_str(), O_WRONLY | O_CREAT | O_EXCL);
    locked = (fd>0);
    if (!locked) {
      if (errno!=EEXIST) {
	throw FileException(fileName, errno);
      } else if (timeout>0) {
	usleep(delay*1000);
	timeout -= delay;
      }
    }
  } while ((!locked) && (timeout>0));
  return (locked);
}

bool FileMutex::isLocked() 
{
  return locked;
}
