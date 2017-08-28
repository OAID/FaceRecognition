#ifndef __LIGHTENED_CNN_H__
#define __LIGHTENED_CNN_H__

#include <caffe/caffe.hpp>
#include <caffe/layers/memory_data_layer.hpp>

#include "feature_extractor.hpp"
#include <string>

/*
* from:
*   https://github.com/AlfredXiangWu/face_verification_experiment
*/

using namespace caffe;

class lightened_cnn : public feature_extractor
{

public:

     lightened_cnn(const std::string name):feature_extractor(name),net_(nullptr),feature_len_(256){};

     int get_feature_length(void) { return feature_len_;};

     void get_input_image_size(int& height, int& width) { height=128;width=128;}

     int load_model(const std::string& model_dir);

     int extract_feature(cv::Mat & img, float * feature);

     ~lightened_cnn(void);


private:

    Net<float> * net_;     
    int feature_len_;

};

#endif
