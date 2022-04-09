#
# Important! Dependencies are done automatically by 'make dep', which also
# removes any old dependencies. Do not modify it...
#
# 2021, d3phys
#

#
# Awesome flags collection
# Copyright (C) 2021, 2022 ded32, the TXLib creator
#
TXFLAGS =  -g --static-pie -std=c++14 -fmax-errors=100 -Wall -Wextra       \
	   -Weffc++ -Waggressive-loop-optimizations -Wc++0x-compat 	   \
	   -Wc++11-compat -Wc++14-compat -Wcast-align -Wcast-qual 	   \
	   -Wchar-subscripts -Wconditionally-supported -Wconversion        \
	   -Wctor-dtor-privacy -Wempty-body -Wfloat-equal 		   \
	   -Wformat-nonliteral -Wformat-security -Wformat-signedness       \
	   -Wformat=2 -Winline -Wlarger-than=8192 -Wlogical-op 	           \
	   -Wmissing-declarations -Wnon-virtual-dtor -Wopenmp-simd 	   \
	   -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls \
	   -Wshadow -Wsign-conversion -Wsign-promo -Wstack-usage=8192      \
	   -Wstrict-null-sentinel -Wstrict-overflow=2 			   \
	   -Wsuggest-attribute=noreturn -Wsuggest-final-methods 	   \
	   -Wsuggest-final-types -Wsuggest-override -Wswitch-default 	   \
	   -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused   \
	   -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix 	   \
	   -Wno-missing-field-initializers -Wno-narrowing 	           \
	   -Wno-old-style-cast -Wno-varargs -fcheck-new 		   \
	   -fsized-deallocation -fstack-check -fstack-protector            \
	   -fstrict-overflow -flto-odr-type-merging 	   		   \
	   -fno-omit-frame-pointer                                         \
	   -fPIE                                                           \
	   -fsanitize=address 	                                           \
	   -fsanitize=alignment                                            \
	   -fsanitize=bool                                                 \
 	   -fsanitize=bounds                                               \
	   -fsanitize=enum                                                 \
	   -fsanitize=float-cast-overflow 	                           \
	   -fsanitize=float-divide-by-zero 			           \
	   -fsanitize=integer-divide-by-zero                               \
	   -fsanitize=leak 	                                           \
	   -fsanitize=nonnull-attribute                                    \
	   -fsanitize=null 	                                           \
	   -fsanitize=object-size                                          \
	   -fsanitize=return 		                                   \
	   -fsanitize=returns-nonnull-attribute                            \
	   -fsanitize=shift                                                \
	   -fsanitize=signed-integer-overflow                              \
	   -fsanitize=undefined                                            \
	   -fsanitize=unreachable                                          \
	   -fsanitize=vla-bound                                            \
	   -fsanitize=vptr                                                 \
	   -lm -pie                                          

CXXFLAGS = -D LOGS_COLORS $(TXFLAGS)
#CXXFLAGS +=  -D LOGS_MEMORY -D DEBUG 

SUBDIRS = logs list ht io

CXX = g++
CPP = $(CXX) -E 

TOPDIR	:= $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)

#
# Header files
#
HPATH =  $(TOPDIR)/include 	\
	 $(TOPDIR)/ht 	    	\
	 $(TOPDIR)/io 	    	\
	 $(TOPDIR)/logs    	\
	 $(TOPDIR)/list/include \

BUILD = build

OBJS  = main.o
make: subdirs $(OBJS) 
	@mkdir -p log
	$(CXX) $(CXXFLAGS) -o $(BUILD)/build $(addprefix $(BUILD)/,$(OBJS)) \
			      list/list.o logs/logs.o ht/ht.o io/io.o
	$(BUILD)/build


LOGS = logs.html
.EXPORT_ALL_VARIABLES: CXX CXXFLAGS CPP LOGS

include $(TOPDIR)/Rules.makefile

### Dependencies ###
main.o: main.cpp
