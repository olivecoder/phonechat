/*
  nextroom.cpp
  Robert Almeida, 2006.02.10
  $Id: privateroom.cpp 1093 2009-09-15 20:04:42Z robert $
*/

#include "meetme.h"

int main (int argc, char **argv)
{
  Meetme meetme;
 
  if ( argc>=2 ) {
    meetme.setVarName(argv[1]);
  }
  meetme.privateRoom();
}
