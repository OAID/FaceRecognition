#include "feature_extractor.hpp"
#include "lightened_cnn.hpp"

#include <exception>

int lightened_cnn::load_model(const std::string& proto_model_dir)
{
	Caffe::set_mode(Caffe::CPU);

	try{
		net_=new Net<float>((proto_model_dir + "/LightenedCNN_B_deploy.prototxt"), caffe::TEST);
		net_->CopyTrainedLayersFrom(proto_model_dir + "/LightenedCNN_B.caffemodel");
	}

	catch(std::exception&e)
	{
		if(net_)
			delete net_;

		return -1;
	}

	return 0;

}

lightened_cnn:: ~lightened_cnn(void)
{
	if(net_)
		delete net_;
}

int lightened_cnn::extract_feature(cv::Mat & img, float * feature)
{
	cv::Mat gray;

	cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);


	Blob<float>* input_blob = net_->input_blobs()[0];
	float * input_data=input_blob->mutable_cpu_data();


	if(gray.isContinuous())
	{
		unsigned char * p_pixel=gray.ptr(0);

		for(int i=0;i<128*128;i++)
		{
			input_data[i]=((float)p_pixel[i])/256;
		}
	}
	else
	{
		for(int i=0;i<128;i++)
		{
			unsigned char * p_row=gray.ptr(i);

			for(int j=0;j<128;j++)
			{
				unsigned char p_pixel=p_row[j];

				input_data[0]=((float)p_pixel)/255;
				input_data++;
			}
		}
	}
	
	net_->Forward();

	/* get output*/

	const Blob<float> * feature_blob=net_->blob_by_name("eltwise_fc1").get();

	if(feature_blob->shape(1)!= feature_len_)
	{
		return -1;
	}

	const float * output_data=feature_blob->cpu_data();

	for(int i=0;i<feature_len_;i++)
	{
		feature[i]=output_data[i];
	} 


	return 0;

}

/******************/



class only_for_auto_register
{
public:
   only_for_auto_register(const std::string& name, extractor_factory::creator creator)
   {
      extractor_factory::register_creator(name,creator);
   } 
      
};

static feature_extractor * lightened_cnn_creator(const std::string& name)
{
      return new lightened_cnn(name);
}

static only_for_auto_register dummy_instance("lightened_cnn",lightened_cnn_creator);
