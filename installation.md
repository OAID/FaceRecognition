# Installation Instructions

## Install dependencies

* Install AID-tools(Caffe-HRT,OpenCV3.3.0)
```
wget ftp://ftp.openailab.net/tools/package/AID-tools.tar.gz
sudo tar -xvf AID-tools.tar.gz -C /usr/local 
sudo /usr/local/AID/gen-pkg-config-pc.sh /usr/local/AID 
```

* Install Readline
```
sudo apt-get install libreadline-dev
```

* Install telnet
```
sudo apt-get install xinetd
sudo apt-get install telnet
```

## Compile
```
cd face-recognition
make
```
Note: Caffe-HRT is necessary to MTCNN and LightenedCNN.

## Run 

```
./build/bin/face-recognition
```

To gain better performance, you can run this script. 
```
./run.sh
```

After launch the program, in a new terminal using telnet to connect the shell. 
```
telnet 127.0.0.1 8080
```

Use this command to add an entry into database
```
reg -i face_id -u name
```

## Shell command list
```
del {-i face_id|-u name}        delete face features by face id or by name
exit                            exit the program
help/h                          show this message
list                            display info of all registered faces
maxfn -i maxFaceNum             set the max number of faces to verify
quit/Q/q                        quit shell. You can reconnect later
reg -i face_id -u name          register/update a face feature into system
rename -i face_id -u new_name   rename the name of face feature by face id
```
