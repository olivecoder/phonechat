/*
  filemutex.h
  Grants exclusive access to process using file system
  Robert Almeida, 2006.02.12
  $Id: filemutex.h 1093 2009-09-15 20:04:42Z robert $
*/
  
#ifndef _FILEMUTEX_H_
#define _FILEMUTEX_H_
#include <string>
#include <exception>

using namespace std;

class FileMutex {
 public:
  FileMutex(const string &fileName);
  virtual ~FileMutex();
  bool getLock(int timeout=2000); // timeout in milliseconds 
  bool isLocked();
 private:
  int fd;
  bool locked;
  string fileName;
};

class FileException: public exception {
 public:
  FileException(const string& fileName, int sys_errno) throw(): exception() {
    this->fileName=fileName;
    this->sys_errno=sys_errno;
  }
  virtual ~FileException() throw() {}
  virtual const char *what() throw();
 private:
  string msg;
  string fileName;
  int sys_errno;
};
#endif
