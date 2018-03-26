AM_YFLAGS=-d
AM_LFLAGS=--header-file=json-scan.h
BUILT_SOURCES = \
	json-parse.h \
	json-parse.c \
	json-scan.c
	query-parse.h \
	query-parse.c \
	query-scan.h \
	query-scan.c 

lib_LTLIBRARIES = libaejson.la

query-scan.c: query-scan.l
query-scan.h: query-scan.c

json-scan.c: json-scan.l
parser.c: json-scan.c


libaejson_la_LDFLAGS = \
	-no-undefined \
	-version $(libaejson_MAJOR):$(libaejson_MINOR):$(libaejson_AGE) \
	-lae0 \
	-lrt \
	-lpthread

libaejson_la_CPPFLAGS = \
	-g -Wall -std=gnu99 \
	-I ../include/

libaejson_la_SOURCES = \
	json-parse.y \
	json-scan.l \
	query-parse.y \
	query-scan.l \
	parser.c \
	value.c \
	object.c \
	pair.c \
	query.c \
	strlit.c 


#headerdir = $(prefix)/include/aejson
#header_DATA = ../include/aejson/aejson.h

clean-local:
	rm -rf \
	json-scan.c json-scan.h json-parse.c json-parse.h \
	query-scan.c query-scan.h query-parse.c query-parse.h	
