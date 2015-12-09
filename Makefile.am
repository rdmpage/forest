# $Id: Makefile.am,v 1.2 2005/04/18 23:02:08 rdmp1c Exp $

GRAPHDIR	= graph
LIBS		= -lGTL -L$(GRAPHDIR) -lgraph -L/usr/local/lib
INCLUDES 	= -I$(GRAPHDIR) -I/usr/local/include

AM_CXXFLAGS=-Wno-deprecated
AM_CFLAGS=-Wno-deprecated

SUBDIRS = graph

# The actual programs
bin_PROGRAMS = forest script gml2dot html
forest_SOURCES = forest.cpp
script_SOURCES = script.cpp
gml2dot_SOURCES = $(GRAPHDIR)/gml2dot.cpp
html_SOURCES = html.cpp

   
