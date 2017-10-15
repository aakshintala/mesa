HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)

CXX = g++
CPPFLAGS += `pkg-config --cflags protobuf grpc`
CXXFLAGS += -std=c++11

ifeq ($(SYSTEM),Darwin)
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc` \
           -lgrpc++_reflection\
		   -ldl
else
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc` \
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed \
		   -ldl
endif

PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

XMLRPC_C_CONFIG = /usr/local/bin/xmlrpc-c-config

INCLUDES = -I. $(shell $(XMLRPC_C_CONFIG) client abyss-server --cflags)

LIBS_CLIENT = \
  $(shell $(XMLRPC_C_CONFIG) client --libs)

LIBS_SERVER_ABYSS = \
  $(shell $(XMLRPC_C_CONFIG) abyss-server --libs)

LIBS_SERVER_CGI = \
  $(shell $(XMLRPC_C_CONFIG) cgi-server --libs)

LIBS_BASE = \
  $(shell $(XMLRPC_C_CONFIG) --libs)

.PHONY: new-server server proto

new-server: server.c
	gcc server.c -o server.out \
		$(INCLUDES) $(LIBS_SERVER_ABYSS) $(LIBS_SERVER_CGI) $(LIBS_BASE)

server: proto
	OCL_ICD_VENDORS=/etc/OpenCL/vendors/mesa.icd \
	g++ -std=c++11 main.cpp \
		-Iinclude $(LDFLAGS) \
		-Wl,-rpath,include -Linclude -lrpcapi \
		-lOpenCL \
		-o server.out

proto: include/api.proto
	(cd include; \
		$(PROTOC) --grpc_out=. \
			--plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) api.proto; \
		$(PROTOC) --cpp_out=. api.proto)
	(cd include; \
		g++ -std=c++11 -fPIC api.grpc.pb.cc api.pb.cc -shared -o librpcapi.so \
		-Wl,--whole-archive -Wl,--no-whole-archive)

clean:
	rm -rf include/*.pb.* include/*.so *.out
