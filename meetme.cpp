/*
  meetme.cpp
  Robert Almeida, 2005.02.10
  $Id: meetme.cpp 1093 2009-09-15 20:04:42Z robert $
*/

#include "meetme.h"
#include "filemutex.h"

Meetme::Meetme() 
{
  maxUsers=5;
  varName="MEETMEROOM";
}

Meetme::~Meetme() 
{
}

void Meetme::setMaxUsers(int maxUsers) 
{
  if ( maxUsers>=2 ) this->maxUsers=maxUsers;
}

void Meetme::setVarName(const string& varName) 
{
  this->varName = varName;
}

int Meetme::count(const string& room) 
{
  char cmd[256];
  const char var[]="counter";
  string counter;

  snprintf(cmd,sizeof(cmd)-1,"EXEC MEETMECOUNT %s|%s",room.c_str(),var);
  exec(cmd);
  counter = getVar(var);

  return atoi(counter.c_str());
}

void Meetme::readRoomList(void)
{
  char line[80], room[16], *s;
  ifstream conf("/etc/asterisk/meetme.conf");

  while ( (!conf.eof()) && (!conf.bad()) && 
	  conf.getline(line,sizeof(line)-1) ) {
    s = strtok(line," ");
    if ( strcmp(s,"conf")==0 )  {
      s = strtok(NULL," ");
      if ( strcmp(s,"=>")==0 ) {
	s = strtok(NULL," ");
	strncpy(room,s,sizeof(room)-1);
	while ( *s && (*s>='0') && (*s<='9')) s++;
	if (*s=='\0') {
	  roomList.push_back(room);
	}
      }
    }
  }
  conf.close();
}

void Meetme::bestRoom(void)
{
  const int maxUsers=5;
  string room, bestRoom, smallerRoom;
  int users, bestUsers, smallerUsers;
  vector<string>::iterator index;
  bool found=false;
  char tmp[80];

  readRoomList();
  if ( roomList.empty() ) {
    AGI_LOG("empty room list!",3);
    exit(1);
  }
  AGI_LOG("selecting the best room",5);
  bestUsers=0;
  smallerRoom=roomList.front();
  smallerUsers=count(smallerRoom);
  for( index=roomList.begin(); index!=roomList.end(); index++ ) {
    users=count(*index);
    snprintf(tmp,sizeof(tmp)-1,"Room #%s has %i users",index->c_str(),users);
    if ( (users<maxUsers) && (users>bestUsers) ) { 
      bestUsers=users;
      bestRoom=*index;
      found=true;
    }
    if ( users < smallerUsers ) {
      smallerRoom=*index;
      smallerUsers=users;
    }
  }
  if ( ! found ) bestRoom=smallerRoom;
  setVar(varName,bestRoom);
}

void Meetme::nextRoom(void)
{
  const int maxUsers=5;
  string currentRoom, nextRoom;
  vector<string>::iterator index, currentRoomIndex;
  int users; 
  bool found;

  readRoomList();
  if ( roomList.empty() ) {
    AGI_LOG("empty room list!",3);
    exit(1);
  }
  nextRoom = getVar(varName);
  currentRoom = getVar(varName);
  found = false;
  for( index=roomList.begin(); index!=roomList.end() && !found; index++ ) {
    found = (*index==currentRoom);
  }
  if ( found ) {
    currentRoomIndex = index;
    found = false;
    for ( ++index; index!=roomList.end() && !found; index++) {
      users=count(*index); 
      if ( (users<maxUsers) && (users>0) ) { 
	found = true;
	nextRoom=*index;
      }
    }
    if ( ! found ) {
      for ( index=roomList.begin(); index!=currentRoomIndex && !found; index++) {
	users=count(*index); 
	if ( (users<maxUsers) && (users>0) ) { 
	  found = true;
	  nextRoom=*index;
	}
      }
    }
  }
  setVar(varName,nextRoom);
}

void Meetme::privateRoom(void)
{
  string room, privateroom, family, lockFileName;
  char s[16];
  unsigned u;
  bool found;
  int users;

  family = "privateroom";
  room = getVar(varName);
  lockFileName = "/var/lock/privateroom" + room;
  FileMutex mutex(lockFileName);
  if ( mutex.getLock() ) {
    found = false;
    privateroom = dbGet(family,room);
    if ( atoi(privateroom.c_str()) ) {
      users = count(privateroom);
      if ( users<2 ) {
	found = true;
	setVar(varName,privateroom);
	if ( users>0 ) dbDel(family,room);
      }
    } 
    // new private room
    if ( !found ) {
      u = 70000000;
      do {
	snprintf(s,sizeof(s)-1,"%u",u+getpid());
	u += 1000000;
      } while ( count(s)>0 );
      privateroom=s;
      setVar(varName,privateroom);
      dbPut(family,room,privateroom);
    }
  }
}
