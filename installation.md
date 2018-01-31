# Installation

## Install dependencies:

* Install Readline:
```
sudo apt-get install libreadline-dev
```

* Install telnet:
```
sudo apt-get install xinetd
sudo apt-get install telnet
```

## Compile
Edit config.mk to select caffe/mxnet/tensorflow for mtcnn
Note: Caffe is necessary to MTCNN and lightenedCNN.
```
cd face-recognition
make
```

## Run 

```
./build/bin/face-recognition
```

To get the best experience, you can run this program by a script which set some environment variables to improve performance. 
```
./run.sh
```

after you launch the program, in a new terminal using telnet to connect the shell: 
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
help/h          				show this message
list                            display info of all registered faces
maxfn -i maxFaceNum             set the max face number to verify
quit/Q/q        				quit shell. You can reconnect later
reg -i face_id -u name          register/update a face feature into system
rename -i face_id -u new_name   rename the name of face feature by id
```


