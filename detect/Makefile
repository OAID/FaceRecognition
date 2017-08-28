
detect-obj-y+=mtcnn.o
detect-obj-y+=comm_lib.o

detect-obj-$(CONFIG_CAFFE_MTCNN)+=caffe_mtcnn.o

# caffe related 

ifeq ($(CONFIG_CAFFE_MTCNN),y)
   CAFFE_INCS += -I$(CAFFE_ROOT)/include
   CAFFE_INCS += -I$(CAFFE_ROOT)/build/src
   CXXFLAGS += $(CAFFE_INCS) -DCPU_ONLY=1 -Wno-sign-compare
   LIBS+=-Wl,-rpath,$(CAFFE_ROOT)/build/lib -L$(CAFFE_ROOT)/build/lib -lcaffe
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

