# $Id: Makefile 1093 2009-09-15 20:04:42Z robert $
# Robert Almeida - 25/6/2002
# updated at may 3, 2003

# compiler & linker flags
CP	 = cp
LDCONFIG = ldconfig
OPT      = -Wall -O3 -s -W -fexceptions 
DEBUG    = -Wall -O0 -fexceptions -ggdb -DDEBUG
INCPATH  = -I/usr/include/postgresql 
POSTGRES = -DHAVE_NAMESPACE_STD -DHAVE_CXX_STRING_HEADER -DDLLIMPORT=""
CFLAGS   = $(POSTGRES) $(INCPATH) $(LIBSPATH) $(OPT)
CXXFLAGS += $(CFLAGS)
LFLAGS   = 

# auto targgets & prerequisites
CSOURCE   = $(wildcard *.c) 
CPPSOURCE = $(wildcard *.cpp)

# project dependent
APPS     = bestroom nextroom privateroom
DLLS     = 
LIBS     = -lcppagi

# automatic variables
COBJS    = $(patsubst %.c, %.o, $(CSOURCE))
CPPOBJS  = $(patsubst %.cpp, %.o, $(CPPSOURCE))
SOURCE   = $(CSOURCE) $(CPPSOURCE) 
OBJS     = $(COBJS) $(CPPOBJS) 

# general rules
# $< 
all: $(OBJS) $(APPS) $(DLLS)

#$(APPS): $(OBJS)
#	$(CXX) $(CXXFLAGS) $(LFLAGS) -o $@ $^ $(LIBS) 

bestroom: bestroom.o meetme.o filemutex.o
	$(CXX) $(CXXFLAGS) $(LFLAGS) -o $@ $^ $(LIBS) 

nextroom: nextroom.o meetme.o filemutex.o
	$(CXX) $(CXXFLAGS) $(LFLAGS) -o $@ $^ $(LIBS) 

privateroom: privateroom.o meetme.o filemutex.o
	$(CXX) $(CXXFLAGS) $(LFLAGS) -o $@ $^ $(LIBS) 

clean:
	$(RM) $(APPS)
	$(RM) $(OBJS)
	$(RM) $(DLLS)
	$(RM) *~

build:
	$(MAKE) clean
	$(MAKE) all

install:
	$(MAKE) $(APPS)
	$(CP) $(APPS) /var/lib/asterisk/agi-bin


