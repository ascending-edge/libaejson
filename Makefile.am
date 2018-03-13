NAME = aejson
LIBNAME = libaejson
AM_CFLAGS = -Wall -std=gnu99
SUBDIRS = $(LIBNAME) test

.PHONY: source-release

source-release:
	git archive $$(cat version.txt) \
	--prefix $(LIBNAME)/ \
	--output ../../Releases/$(LIBNAME)-$$(cat version.txt).tar.gz

