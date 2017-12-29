# Installation

## Install dependencies:

* Install CMake:
Skip this step if OpenCV3.3 has been installed
```
sudo apt-get install cmake
```

* Install Readline:
```
sudo apt-get install libreadline-dev
```

* Install talnet:
```
sudo apt-get install xinetd
sudo apt-get install telnet
```

* Install OpenCV3.3
```
wget https://github.com/opencv/opencv/archive/3.3.0.zip --no-check-certificate
unzip 3.30.zip
cd opencv-3.3.0
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local/AID/opencv3.3.0 ..
make -j4
sudo make install
sudo ldconfig
wget https://github.com/OAID/AID-tools/tree/master/script/gen-pkg-config-pc.sh
sudo ~/gen-pkg-config-pc.sh /usr/local/AID
```

* Install CaffeOnACL and compile using the CMake build instructions:
```
Referenced by https://github.com/OAID/CaffeOnACL
Modify the Makefile "LIBRARIES += glog gflags protobuf leveldb snappy lmdb boost_system hdf5_hl hdf5 m opencv_core opencv_highgui opencv_imgproc opencv_imgcodecs",if errors ocurred.
```

## Compile
Edit config.mk to select caffe/mxnet/tensorflow for mtcnn
Note: Caffe is necessary to build lightened CNN face recognition algorithm
```
cd face-demo
make
```

## Run 
Add the directory of FaceVerify/libFaceCV and libcaffe.so which in caffeOnACL to LD_LIBRARY_PATH.
```
./build/bin/face-demo
```

after you launch the progtam, in a new terminal using telnet to connect the shell: 
```
telnet 127.0.0.1 8080
```

using this command to add an entry into database
```
reg -i face_id -u name
```

## shell command list
```
del {-i face_id|-u name}        delete face features by face id or by name
exit                            exit the demo
help/h          		show this message
list                            display info of all registered faces
maxfn -i maxFaceNum             set the max face number to verify
quit/Q/q        		quit shell. You can reconnect later
reg -i face_id -u name          register/update a face feature into system
rename -i face_id -u new_name   rename the name of face feature by id
```


