# Face Demo

This is a demo to show how to do face detection and recoginiton.
This also will work as a testbed about how to define a framework API for different algorthms to be plugged-in/plugged out

# Build

Edit config.mk to select caffe/mxnet/tensorflow for mtcnn

Then 'make', 

all the objects and binaries are generated under directory $BUILD_DIR


# Run


launch the program: 

    ./build/bin/face_demo

after you launch the progtam, in a new terminal
using telnet to connect the shell:  
   
    telnet 127.0.0.1 8080

using this command to add an entry into database

    reg -i face_id -u name

## shell command list

    del {-i face_id|-u name}        delete face features by face id or by name
    exit                            exit the demo
    help/h                          show this message
    list                            display info of all registered faces
    quit/Q/q                        quit shell. You can reconnect later
    reg -i face_id -u name          register/update a face feature into system
    rename -i face_id -u new_name   rename the name of face feature by id
