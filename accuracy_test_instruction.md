# Accuracy test instructions

## Download test dataset:
```
wget ftp://ftp.openailab.net/tools/face_benchmark/benchmark.tar.gz
tar -xzvf benchmark.tar.gz
mv benchmark /home/firefly/benchmark
```

## Notes:
The default test directory is /home/firefly/benchmark.
If you want to custom dataset address(Not Recommend!), you should change the dataset directory in the following source file:
* ./test/test_detectAccuracy
* ./test/test_detect_for_lightenedcnn
* ./test/test_lightenedCNN

## Install dependencies:

Install Matplotlib for Python:
```
sudo apt-get install python-matplotlib
```

## Run accuracy test program

### Test MTCNN:
```
./build/test/detectAccuracy
```
After processing all the test images, it will display the accuracy result and output detected faces in benchmark/mtcnn.txt

### Test LightenedCNN:
```
./build/test/test_detect_for_lightenedCNN
./build/test/test_lightenedCNN
cd /home/firefly/benchmark
python draw_roc.py
```

## Customize your own test dataset.
```
1) Detection data is a subset of CelebA. So new data should be the same format of CelebA.
2) Detection images should place in benchmark/img, label file is benchmark/list_bbox.txt.
3) Recognition data uses LFW dataset, locate in benchmark/LFW. 
4) benchmark/lfw_face stores the test_detect_for_lightenedCNN results, which is all the features that the LFW dataset's faces, using MTCNN to detect and align.
   So if you want to use your own detector and test lightenedCNN, you should clear this directory first.
5) Read benchmark/README.txt for more information.
```

