# Accuracy Test Instructions

## Download test dataset

```
wget -c ftp://ftp.openailab.net/dataset/benchmark.tar.gz --ftp-user=*** --ftp-password=******* --no-passive-ftp
tar -xzvf benchmark.tar.gz
mv benchmark /home/firefly/benchmark
```

## Notes
You may need to [contact us](support@openailab.com) to access the dataset. 
The default test directory is /home/firefly/benchmark.
If you want to customize your own dataset address, you can run test program with arguments, or change the dataset directory in the following source file.

* ./test/test_detectAccuracy
* ./test/test_detect_for_lightenedCNN
* ./test/test_lightenedCNN

## Install dependencies

Install Matplotlib for Python

```
sudo apt-get install python-matplotlib
```

## Run accuracy test program

### Test MTCNN

* Using default directory

```
./build/test/test_detectAccuracy
```

* Customize dataset directory

```
./build/test/test_detectAccuracy -dir $benchmark/img/ -label $benchmark/list_bbox.txt -output $benchmark/mtcnn.txt
```

After processing all the test images, it will display the accuracy result and output detected faces in benchmark/mtcnn.txt

### Test LightenedCNN

* Using default directory
```
./build/test/test_detect_for_lightenedCNN
./build/test/test_lightenedCNN
cd /home/firefly/benchmark
python draw_roc.py
```

* Customize dataset directory

```
./build/test/test_detect_for_lightenedCNN -dir $benchmark/LFW/ -out $benchmark/lfw_face/ -img $benchmark/Path_lfw2.txt
./build/test/test_lightenedCNN -dir $benchmark/lfw_face/ -out $benchmark/ -pair $benchmark/pairs.txt
cd $benchmark
python draw_roc.py
```

### Test performance
Run the following command to evaluate performance. Where 100 means calculate 100 frames average time then stop.
```
./build/bin/face-recognition -t 100
```

## Customize your own test dataset
```
1) Detection data is a subset of CelebA. So new data should be the same format of CelebA.
2) Detection images should place in benchmark/img, label file is benchmark/list_bbox.txt.
3) Recognition data uses LFW dataset, locate in benchmark/LFW. 
4) benchmark/lfw_face stores the test_detect_for_lightenedCNN results, which is all the features that the LFW dataset's faces, using MTCNN to detect and align.
   So if you want to use your own detector and test lightenedCNN, you should clear this directory first.
5) Read benchmark/README.txt for more information.
```
