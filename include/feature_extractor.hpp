#ifndef __FACE_EXTRACTOR_H__
#define __FACE_EXTRACTOR_H__

#include <string>
#include <opencv2/opencv.hpp>

class feature_extractor
{
public:
    virtual int load_model(const std::string& model_dir)=0;

    virtual int get_feature_length(void)=0;
    
    virtual void get_input_image_size(int& height, int& width)=0;

    virtual int extract_feature(cv::Mat & img, float * feature)=0;

    virtual ~feature_extractor(void){};

    feature_extractor(const std::string&  name): name_(name){}

private:
   std::string name_;
   

};

class extractor_factory
{
public:
    typedef feature_extractor * (*creator)(const std::string& name);

    static void register_creator(const std::string& name,creator& create_func);
    static feature_extractor * create_feature_extractor(const std::string& name);
    static std::vector<std::string> list_extractor(void);
       
private:
    extractor_factory(){};
};

#endif
