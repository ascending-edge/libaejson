noinst_PROGRAMS = aej-test
aej_test_DEPENDENCIES = ../libaejson/.libs/libaejson.la
#ae_test_LDADD = ../libae/.libs/libae0.la
aej_test_LDADD = \
	../libaejson/.libs/libaejson.la \
	-lae0 \
	-lrt \
	-lpthread
aej_test_CPPFLAGS = \
	-g -Wall -std=gnu99 \
	-I ../include
aej_test_SOURCES = \
	aej-test.c
