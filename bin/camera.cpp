#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <unistd.h>
#include <signal.h>

#include "mtcnn.hpp"

#include "utils.hpp"

#ifndef MODEL_DIR
#define MODEL_DIR "./models"
#endif

volatile int quit_flag=0;

void sig_user_interrupt(int sig, siginfo_t * info, void * arg)
{
	std::cout<<"User interrupt the program ...\n"<<std::endl;
	quit_flag=1;
}


int main(int argc, char * argv[])
{
	const char * type="mxnet";
        struct  sigaction sa;

	int res;

	while((res=getopt(argc,argv,"f:t:s"))!=-1)
	{
		switch(res)
		{
			case 't':
				type=optarg;
				break;
			default:
				break;
		}
	}

	sa.sa_sigaction=sig_user_interrupt;
	sa.sa_flags=SA_SIGINFO;
	sigemptyset(&sa.sa_mask);

	sigaction(SIGTERM,&sa,NULL);
	sigaction(SIGINT,&sa,NULL);


	cv::VideoCapture camera;

	camera.open(0);

	if(!camera.isOpened())
	{
		std::cerr<<"failed to open camera"<<std::endl;
		return 1;  
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



	cv::Mat frame;

	while(!quit_flag)
	{
	        std::vector<face_box> face_info;

		camera.read(frame);


		unsigned long start_time=get_cur_time();

		p_mtcnn->detect(frame,face_info);

		unsigned long end_time=get_cur_time();


		for(unsigned int i=0;i<face_info.size();i++)
		{
			face_box& box=face_info[i];

			/*draw box */

			cv::rectangle(frame, cv::Point(box.x0, box.y0), cv::Point(box.x1, box.y1), cv::Scalar(0, 255, 0), 1);


			/* draw landmark */

			for(int l=0;l<5;l++)
			{
				cv::circle(frame,cv::Point(box.landmark.x[l],box.landmark.y[l]),1,cv::Scalar(0, 0, 255),2);
			}

		}



		std::cout<<"total detected: "<<face_info.size()<<" faces. used "<<(end_time-start_time)<<" us"<<std::endl;

		cv::imshow("camera",frame);

		cv::waitKey(1000);

	}

	return 0;
}


