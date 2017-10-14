include Makefile.sources

XMLRPC_C_CONFIG = /usr/local/bin/xmlrpc-c-config

AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_builddir)/src \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/src/gallium/include \
	-I$(top_srcdir)/src/gallium/drivers \
	-I$(top_srcdir)/src/gallium/auxiliary \
	-I$(top_srcdir)/src/gallium/winsys \
	-I$(top_srcdir)/src/gallium/state_trackers/fake_pipe \
	$(shell $(XMLRPC_C_CONFIG) client abyss-server --cflags)

if HAVE_CLOVER_ICD
AM_CFLAGS += -DHAVE_CLOVER_ICD
else
# Only install the headers if we are building a stand-alone implementation
cldir = $(includedir)/CL
cl_HEADERS = \
	$(top_srcdir)/include/CL/cl.h \
	$(top_srcdir)/include/CL/cl.hpp \
	$(top_srcdir)/include/CL/cl_d3d10.h \
	$(top_srcdir)/include/CL/cl_d3d11.h \
	$(top_srcdir)/include/CL/cl_dx9_media_sharing.h \
	$(top_srcdir)/include/CL/cl_egl.h \
	$(top_srcdir)/include/CL/cl_ext.h \
	$(top_srcdir)/include/CL/cl_gl.h \
	$(top_srcdir)/include/CL/cl_gl_ext.h \
	$(top_srcdir)/include/CL/cl_platform.h \
	$(top_srcdir)/include/CL/opencl.h
endif

noinst_LTLIBRARIES = librpcclient.la

librpcclient_la_SOURCES = $(CPP_SOURCES)
