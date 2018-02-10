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

int main(int argc, char * argv[])
{
	const char * type="caffe";
	const char * fname="./test.jpg";
	int save_crop=0;

	int res;

	while((res=getopt(argc,argv,"f:t:s"))!=-1)
	{
		switch(res)
		{
			case 'f':
				fname=optarg;
				break;
			case 't':
				type=optarg;
				break;
			case 's':
				save_crop=1;
				break;
			default:
				break;
		}
	}



	cv::Mat frame = cv::imread(fname);

	if(!frame.data)
	{
		std::cerr<<"failed to read image file: "<<fname<<std::endl;
		return 1;
	}

	std::string model_dir=MODEL_DIR;
	std::vector<face_box> face_info;


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

        unsigned long start_time=get_cur_time();

	p_mtcnn->detect(frame,face_info);

        unsigned long end_time=get_cur_time();


	for(unsigned int i=0;i<face_info.size();i++)
	{
		face_box& box=face_info[i];

		printf("face %d: x0,y0 %2.5f %2.5f  x1,y1 %2.5f  %2.5f conf: %2.5f\n",i,
				box.x0,box.y0,box.x1,box.y1, box.score);
		printf("landmark: ");

		for(unsigned int j=0;j<5;j++)
			printf(" (%2.5f %2.5f)",box.landmark.x[j], box.landmark.y[j]);

		printf("\n");


		if(save_crop)
		{

			cv::Mat crop_img=frame(cv::Range(box.y0,box.y1),
					cv::Range(box.x0,box.x1));

			char title[128];

			sprintf(title,"id%d.jpg",i);

			cv::imwrite(title,crop_img);
		}

		/*draw box */

		cv::rectangle(frame, cv::Point(box.x0, box.y0), cv::Point(box.x1, box.y1), cv::Scalar(0, 255, 0), 1);


		/* draw landmark */

		for(int l=0;l<5;l++)
		{
			cv::circle(frame,cv::Point(box.landmark.x[l],box.landmark.y[l]),1,cv::Scalar(0, 0, 255),1.8);
		}

	}

	cv::imwrite("new.jpg",frame);

	std::cout<<"total detected: "<<face_info.size()<<" faces. used "<<(end_time-start_time)<<" us"<<std::endl;

	return 0;
}


