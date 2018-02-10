# FaceRecognition
[![GitHub license](http://dmlc.github.io/img/apache2.svg)](./LICENSE)

FaceRecognition is an implementation project of face detection and  recognition. The face detection using MTCNN algorithm, and recognition using LightenedCNN algorithm. 

The release version is 0.1.3, is based on ROCK960 Platform, target OS is Ubuntu 16.04.

* MTCNN is a deep cascaded multi-task framework to boost up face detection performance. See also [OAID/FaceDetection](https://github.com/OAID/FaceDetection).
* Lightened CNN is a light CNN framework to learn a compact embedding on the large-scale face data with massive noisy labels. See also [LightenedCNN](https://github.com/AlfredXiangWu/face_verification_experiment).

### Documents
* [Installation Instructions](installation.md)
* [Accuracy Test Instructions](accuracy_test_instruction.md)
* [Performance Report PDF](performance_report.pdf)

# Release History

### Version 0.1.3 - 2018-2-1
   
  Fix bugs in accuracy test.
  Add FaceRecognition test control.

### Version 0.1.2 - 2018-1-26
   
  Add support to test MTCNN and lightenedCNN.
  Update log and time control.

### Version 0.1.1 - 2017-12-29 
   
  Add support to set the max number of faces.

### Version 0.1.0 - 2017-10-23 
   
  Initial version supports face register, face detection, and face recognization. 

# Issue Report
Encounter any issue, please report on [issue report](https://github.com/OAID/FaceRecognition/issues). Issue report should contain the following information:

* The exact description of the steps that are needed to reproduce the issue. 
* The exact description of what happens and what you think is wrong. 
