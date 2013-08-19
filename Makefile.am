# $Id: Makefile.am,v 1.2 2005/04/18 23:02:08 rdmp1c Exp $

GRAPHDIR	= graph
LIBS		= -lGTL -L$(GRAPHDIR) -lgraph 
INCLUDES 	= -I$(GRAPHDIR) 

AM_CXXFLAGS=-Wno-deprecated
AM_CFLAGS=-Wno-deprecated

SUBDIRS = graph

# The actual programs
bin_PROGRAMS = forest script gml2dot
forest_SOURCES = forest.cpp
script_SOURCES = script.cpp
gml2dot_SOURCES = $(GRAPHDIR)/gml2dot.cpp

   
