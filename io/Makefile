#
# Important! Dependencies are done automatically by 'make dep', which also
# removes any old dependencies. Do not modify it...
# 2021, d3phys
#

OBJS  = iommap.o parse.o

io.o: subdirs $(OBJS) 
	$(LD) -r -o $@ $(OBJS)

include $(TOPDIR)/Rules.makefile

### Dependencies ###
logs.o: logs.cpp /home/d3phys/Code/hash-table/include/logs.h
