#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <unistd.h>

#include "mtcnn.hpp"

#include "utils.hpp"

#ifndef MODEL_DIR
#define MODEL_DIR "./models"
#endif

unsigned int FN=0,FP=0,TN=0,TP=0;
double total_iou;

class bbbox
{
	public:
		std::string img_name;
		cv::Rect rect;
		double score;
};

cv::Rect info2rect(face_box& box)
{
	cv::Rect rec;
	rec.x=box.x0;
	rec.y=box.y0;
	rec.width=box.x1-box.x0;
	rec.height=box.y1-box.y0;
	return rec;
}

double cal_iou(cv::Rect r1, cv::Rect r2)
{
	int x0 = r1.x > r2.x ? r1.x : r2.x;
	int y0=r1.y>r2.y?r1.y:r2.y;
	int x1=r1.x+r1.width;
	int y1=r1.y+r1.height;
	int x2=r2.x+r2.width;
	int y2=r2.y+r2.height;
	int x3=x1<x2?x1:x2;
	int y3=y1<y2?y1:y2;

	
	if(x3<x0||y3<y0)
	{
		return 0.0;
	}
	else
	{
		int intersection = std::abs((x3-x0+1)*(y3-y0+1));
		int area_r1 = r1.width * r1.height;
		int area_r2 = r2.width * r2.height;
		double iou = intersection*1.0 / (area_r1 + area_r2 - intersection);
		std::cout << "iou: " << iou <<  std::endl;
		return iou;
	}
}

void evaluate(bbbox groundtrue, std::vector<bbbox> detect_rets)
{
	if(detect_rets.size()==0)
	{
		FN++;
		return;
	}
	else if(detect_rets.size()==1)
	{
		double iou = cal_iou(groundtrue.rect, detect_rets[0].rect);
		if(iou>=0.5)
		{
			TP++;
			total_iou += iou;
		}
		else
		{
			FN++;
		}
		
	}
	else
	{
		for(int i=0;i<detect_rets.size();i++)
		{
			double iou = cal_iou(groundtrue.rect, detect_rets[i].rect);
			if(iou>0.5)
			{
				TP++;
				total_iou += iou;
				return;
			}
		}
		FN++;
	}
}

int main(int argc, char * argv[])
{
	const char * type="caffe";
	const char * dir="/home/firefly/benchmark/img/";   /* where your imgs are. */
	const char * out="/home/firefly/benchmark/mtcnn.txt";   /* where to put output file. */
	const char * in="/home/firefly/benchmark/list_bbox.txt"; /* where your bbox file. */

	total_iou = 0;

	if( argc == 1 )
    {
        std::cout << "Usage: " << argv[0] << std::endl;
        std::cout << "  -dir <test images dir>" << std::endl;
        std::cout << "  -label <label box file name>" << std::endl;
        std::cout << "  -output <output label file name>" << std::endl;
    }

	for( int i = 1; i < argc; i++ )
    {
        if( !strcmp( argv[i], "-dir" ) )
        {
            dir = argv[++i];
        }
		else if( !strcmp( argv[i], "-label" ) )
        {
            in = argv[++i];
        }
		else if( !strcmp( argv[i], "-output" ) )
        {
            out = argv[++i];
        }
	}

	std::ifstream fbox(in,std::ios::in);
	if(!fbox.is_open())
	{
    	std::cerr << "Can not open fbox from " << in << std::endl;
    	std::cerr << "Set fbox using -f option. See usage: ." << std::endl;
    	return -1;
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

	std::ofstream output(out);
	output.close();

	unsigned int total_num=0,total_detect_num=0;
    while(!fbox.eof())
    {
    	bbbox label;
		std::vector<bbbox> d_rets;
		fbox >> label.img_name >> label.rect.x >> label.rect.y >> label.rect.width >> label.rect.height;

		std::cout << "get one label: image_name: " << label.img_name <<  " rect: " << label.rect.x << " "  << label.rect.y << " " << label.rect.width << " " << label.rect.height << std::endl;
	
		std::vector<face_box> face_info;
		
    	std::string full_filename = (std::string)dir + label.img_name;
		std::cout << "Processing: " << label.img_name << " full_filename: " << full_filename << std::endl;
		
    	cv::Mat frame = cv::imread(full_filename);

		while(frame.rows > 1000 || frame.cols > 1000)
			{
				cv::resize(frame,frame,cv::Size(),0.7,0.7);
				cv::imwrite(full_filename,frame);
			}
		
    	if(!frame.data)
    	{
    		std::cerr<<"imgfile open error: " << label.img_name << std::endl;
    		return 1;
    	}

    	p_mtcnn->detect(frame,face_info);
		
		std::cout << "total detected: " << face_info.size() << " faces." << std::endl;

		total_detect_num+=face_info.size();

		for(unsigned int i=0;i<face_info.size();i++)
    	{
    		bbbox d_ret;
    		d_ret.rect = info2rect(face_info[i]);
			d_ret.img_name = label.img_name;
			d_ret.score = face_info[i].score;
			d_rets.push_back(d_ret);
			
    		std::ofstream output(out,std::ios::app);
			output << d_ret.img_name <<" " << d_ret.rect.x << " " << d_ret.rect.y << " " << d_ret.rect.width << " " << d_ret.rect.height << std::endl;
    		output.close();
    	}
		evaluate(label,d_rets);

		total_num++;

    }
	fbox.close();

	std::cout << "Test done.\n" << "TP=" << TP << " FP=" << FP << " FN=" << FN << " TN=" << TN << std::endl;
	std::cout << "Recal: " << TP * 1.0 / total_num << " Precision: " << TP * 1.0 / total_detect_num << " Stability: " << total_iou / TP << std::endl;

	return 0;
}


