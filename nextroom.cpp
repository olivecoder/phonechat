/*
  nextroom.cpp
  Robert Almeida, 2006.02.10
  $Id: nextroom.cpp 1093 2009-09-15 20:04:42Z robert $
*/

#include "meetme.h"

int main (int argc, char **argv)
{
  Meetme meetme;
  int maxUsers;

  if ( argc>=2 ) {
    meetme.setVarName(argv[1]);
  }
  if ( argc>=3 ) {
    maxUsers = atoi(argv[2]);
    meetme.setMaxUsers(maxUsers);
  }
  meetme.nextRoom();
}
