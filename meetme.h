/*
  meetme.h
  Robert Almeida, 2005.02.10
  $Id: meetme.h 1093 2009-09-15 20:04:42Z robert $
*/

#include <cppagi.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Meetme: public agi {
public:
  Meetme();
  virtual ~Meetme();
  void bestRoom(void);
  void nextRoom(void);
  void privateRoom(void);
  void setMaxUsers(int maxUsers);
  void setVarName(const string& varName);
private:
  int maxUsers;
  string varName;
  vector<string> roomList;
  void readRoomList(void);
  int count(const string& room);
};
