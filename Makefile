#
# Important! Dependencies are done automatically by 'make dep', which also
# removes any old dependencies. Do not modify it...
#
# 2022, d3phys
#

TOPDIR	:= $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)

# Header files #
HPATH = $(TOPDIR)/include 	 \
	$(TOPDIR)/ht 	    	 \
	$(TOPDIR)/io 	    	 \
	$(TOPDIR)/logs    	 \
	$(TOPDIR)/list/include 

ifeq ($(MAKELEVEL),0)
CXX 	 = g++
CXXFLAGS = $(addprefix -I, $(HPATH)) $(LOGSFLAGS) #$(TXFLAGS) 
LOGSFLAGS = -D LOGS_COLORS -D LOGS_FILE='"logs.html"'
endif

# Subdirectories #
SUBDIRS = logs list ht io init

htab.o: list.o ht.o io.o
	$(LD) -r -o htab.o list/list.o ht/ht.o io/io.o

anal: list.o ht.o io.o logs.o
	$(CXX) $(CXXFLAGS) -c -o init/init.o init/anal.cpp
	$(CXX) $(CXXFLAGS) -o anal init/init.o list/list.o logs/logs.o ht/ht.o io/io.o
	./anal

perf: list.o ht.o io.o logs.o
	$(CXX) $(CXXFLAGS) -c -o init/init.o init/perf.cpp
	$(CXX) $(CXXFLAGS) -o perf init/init.o list/list.o logs/logs.o ht/ht.o io/io.o
	./perf

%.o:
	cd $(patsubst %.o,%, $@) && $(MAKE)

.EXPORT_ALL_VARIABLES: CXX CXXFLAGS CPP LOGS

include $(TOPDIR)/Rules.makefile

### Dependencies ###
main.o: main.cpp
