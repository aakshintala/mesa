include Makefile.sources

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_builddir)/src \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/src/gallium/include \
	-I$(top_srcdir)/src/gallium/drivers \
	-I$(top_srcdir)/src/gallium/auxiliary \
	-I$(top_srcdir)/src/gallium/winsys

if HAVE_CLOVER_ICD
AM_CPPFLAGS += -DHAVE_CLOVER_ICD
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

librpcclient_la_CXXFLAGS = \
	-std=c++11 \
	$(VISIBILITY_CXXFLAGS) \
	`pkg-config --cflags protobuf grpc` \
	-L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc` \
	-Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed \
	-ldl

librpcclient_la_SOURCES = $(CPP_SOURCES)
