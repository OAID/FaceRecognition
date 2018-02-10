#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <memory.h>

#include <unistd.h>
#include <signal.h>

#include "mtcnn.hpp"
#include "face_align.hpp"
#include "feature_extractor.hpp"
#include "face_verify.hpp"
#include "face_mem_store.hpp"

#include "utils.hpp"

#ifndef MODEL_DIR
#define MODEL_DIR "./models"
#endif

bool GreaterSort (face_box a,face_box b)
{ return (abs(a.x1-a.x0)*(a.y1-a.y0) > abs((b.x1-b.x0)*(b.y1-b.y0))); }

void split_dir_name(std::string& s, std::vector<std::string>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
  	pos2 = s.find(c);
  	pos1 = 0;
  	while(std::string::npos != pos2)
  	{
		v.push_back(s.substr(pos1, pos2-pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
  	if(pos1 != s.length())
    	v.push_back(s.substr(pos1));
}

int main(int argc, char * argv[])
{
	const char * type="caffe";
	const char * dir="/home/firefly/benchmark/LFW/";   /* where your imgs are. */
	const char * outdir="/home/firefly/benchmark/lfw_face/";   /* where to put output feature. */
	const char * in="/home/firefly/benchmark/Path_lfw2.txt"; /* where your imglist file. */
	
	if( argc == 1 )
	{
		std::cout << "Usage: " << argv[0] << std::endl;
		std::cout << "	-dir <test images dir>" << std::endl;
		std::cout << "	-out <output feature dir>" << std::endl;
		std::cout << "	-img <images list file name>" << std::endl;
	}
	
	for( int i = 1; i < argc; i++ )
	{
		if( !strcmp( argv[i], "-dir" ) )
		{
			dir = argv[++i];
		}
		else if( !strcmp( argv[i], "-out" ) )
		{
			outdir = argv[++i];
		}
		else if( !strcmp( argv[i], "-img" ) )
		{
			in = argv[++i];
		}
	}

	std::string model_dir=MODEL_DIR;
	
	mtcnn * p_mtcnn=mtcnn_factory::create_detector(type);

    if(p_mtcnn==nullptr)
    {
        std::cerr<<type<<" is not supported"<<std::endl;
        std::cerr<<"supported types: ";
        std::vector<std::string> type_list=mtcnn_factory::list();
            
        for(int i=0;i<type_list.size();i++)
            std::cerr<<" "<<type_list[i];

        std::cerr<<std::endl;
            
        return 1;
    }

	p_mtcnn->load_model(model_dir);
	p_mtcnn->load_model(model_dir);
    p_mtcnn->set_threshold(0.8,0.8,0.8);
    p_mtcnn->set_factor_min_size(0.6,48);
	
	/* extractor */
	const std::string extractor_name("lightened_cnn");
	feature_extractor * p_extractor;
	p_extractor=extractor_factory::create_feature_extractor(extractor_name);

	if(p_extractor==nullptr)
	{
		return 2;
	}

	p_extractor->load_model(model_dir);

	std::ifstream filelist(in,std::ios::in);
	if(!filelist.is_open())
	{
    	std::cerr << "Can not open filelist from " << in << std::endl;
    	std::cerr << "Set filelist using -f option. See usage: ." << std::endl;
    	return -1;
  	}
	
	std::string filename;
	
     while(!filelist.eof())
    {
		filelist >> filename;
		std::vector<face_box> face_info;

    	std::string full_filename = (std::string)dir + filename;
		std::cout << "Processing: " << filename << " full_filename: " << full_filename << std::endl;
		
    	cv::Mat frame = cv::imread(full_filename);

		std::vector<std::string> dir_name;
		const std::string seperater = "/";
		split_dir_name(filename,dir_name,seperater);
		std::string img_dir = dir_name[0];
		std::string img_name = dir_name[1];
		
		while(frame.rows > 1000 || frame.cols > 1000)
		{
			cv::resize(frame,frame,cv::Size(),0.7,0.7);
			cv::imwrite(full_filename,frame);
			std::cout << "Resize image: " << filename << " to: width=" << frame.rows << " height=" << frame.cols << std::endl; 
		}
		
    	if(!frame.data)
    	{
			std::cerr<<"imgfile open error: " << filename << std::endl;;
    		return 1;
    	}

    	p_mtcnn->detect(frame,face_info);
		std::cout << "total detected: " << face_info.size() << " faces." << std::endl;

		std::string out_feature = outdir + img_name + ".txt";
		std::ofstream output(out_feature);
		output.close();
		
		if(face_info.size()==0)
		{
			std::ofstream output(out_feature,std::ios::app);
			output << 0 << std::endl;
			output.close();
		}
		else
		{
			if(face_info.size()>1)
			{
				std::sort(face_info.begin(),face_info.end(),GreaterSort);
            	face_info.erase(face_info.begin()+1,face_info.end());
			}

			float * d_feature;
			d_feature = new float[256];
			
			cv::Mat aligned;
			/* align face */
			get_aligned_face(frame,(float *)&(face_info[0].landmark),5,128,aligned);

			/* get feature */
			p_extractor->extract_feature(aligned,d_feature); 

			std::ofstream output(out_feature,std::ios::app);
			output << 1 << std::endl;
			for(int i=0;i<256;i++)
			{
				output << d_feature[i] << " ";
			}
			output << std::endl;
			output.close();		
		}
		
    }
	filelist.close();

	return 0;
}
