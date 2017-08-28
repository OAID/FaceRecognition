#setting this variable to y to enable building caffe_mtcnn

#if you want to build caffe_mtcnn, you should set :
#CONFIG_CAFFE_MTCNN=y, CONFIG_MXNET_MTCNN=n, CONFIG_TENSORFLOW_MTCNN=n

CONFIG_CAFFE_MTCNN=y

CAFFE_ROOT=/home/haitao/github/caffe

#if you want to build mxnet_mtcnn, you should set :
#CONFIG_CAFFE_MTCNN=y, CONFIG_MXNET_MTCNN=y, CONFIG_TENSORFLOW_MTCNN=n

CONFIG_MXNET_MTCNN=y

MXNET_ROOT=/home/haitao/github/mxnet


CONFIG_TENSORFLOW_MTCNN=y

TENSORFLOW_ROOT=./tf_dist
