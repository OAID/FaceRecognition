
detect-obj-y+=mtcnn.o
detect-obj-y+=comm_lib.o

detect-obj-$(CONFIG_CAFFE_MTCNN)+=caffe_mtcnn.o

# caffe related 

ifeq ($(CONFIG_CAFFE_MTCNN),y)
   COMPUTELIBRARY_INCS += $(shell pkg-config computelibrary --cflags)
   CAFFE_INCS += $(shell pkg-config caffeonacl --cflags)
   CXXFLAGS += $(CAFFE_INCS) -DCPU_ONLY=1 -Wno-sign-compare
   LIBS+= $(shell pkg-config caffeonacl --libs) $(shell pkg-config computelibrary --libs)
   LIBS+= -lprotobuf -lboost_system -lglog
endif


#mxnet related
detect-obj-$(CONFIG_MXNET_MTCNN)+=mxnet_mtcnn.o

ifeq ($(CONFIG_MXNET_MTCNN),y)

   MXNET_INCS+=-I$(MXNET_ROOT)/include
   MXNET_INCS+=-I$(MXNET_ROOT)/dmlc-core/include
   MXNET_INCS+=-I$(MXNET_ROOT)/nnvm/include

   CXXFLAGS += $(MXNET_INCS) -Wno-sign-compare
   LIBS+=$(MXNET_ROOT)/lib/libmxnet.so

endif

#tensorlfow related

detect-obj-$(CONFIG_TENSORFLOW_MTCNN)+=tensorflow_mtcnn.o

ifeq ($(CONFIG_TENSORFLOW_MTCNN),y)

   TENSORFLOW_INCS += -I$(TENSORFLOW_ROOT)/include
   CXXFLAGS +=$(TENSORFLOW_INCS)

   LIBS+=-Wl,-rpath,$(TENSORFLOW_ROOT)/lib -L$(TENSORFLOW_ROOT)/lib -ltensorflow

endif

