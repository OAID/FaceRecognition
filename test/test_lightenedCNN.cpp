#pragma GCC diagnostic error "-std=c++11"  

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <memory.h>
#include <unistd.h>
#include <cmath>
#include <algorithm>


#include "utils.hpp"

const char * feature_dir="/home/firefly/benchmark/lfw_face/";   /* where your feature is. */
const char * out="/home/firefly/benchmark/lightened_CNN_LFW.txt";   /* where to put output file. */
const char * pair_in="/home/firefly/benchmark/pairs.txt"; /* where your bbox file. */

std::vector<double> FPR,TPR,PRE;
unsigned int FN=0,FP=0,TN=0,TP=0;

float compare(float * f0, float * f1)
{
	float  product=0;
	float l2_sum0=0;
	float  l2_sum1=0;

	float score;

	for(int i=0;i<256;i++)
	{
		product+=f0[i]*f1[i];
		l2_sum0+=f0[i]*f0[i];
		l2_sum1+=f1[i]*f1[i];
	}

	score=product/sqrt(l2_sum0)/sqrt(l2_sum1);
	if(score<0)
		score = -score;

	return score;
}

class same_person
{
	public:
		std::string name;
		int img_n1,img_n2;
};

class diff_person
{
	public:
		std::string name1,name2;
		int img_n1,img_n2;
};

std::string get_name(std::string& name, int num)
{
	if(num<10)
		return (std::string)feature_dir + name + "_000" + std::to_string(num) + ".jpg.txt";
	else
		return (std::string)feature_dir + name + "_00" + std::to_string(num) + ".jpg.txt";
}


int main(int argc, char * argv[])
{

	int res;

	while((res=getopt(argc,argv,"f:o:d"))!=-1)
	{
		switch(res)
		{
			case 'f':
				pair_in=optarg;
				break;
			case 'o':
				out=optarg;
				break;
			case 'd':
				feature_dir=optarg;
				break;
			default:
				break;
		}
	}

	std::ifstream pair(pair_in,std::ios::in);
	if(!pair.is_open())
	{
    	std::cerr << "Can not open pair from " << pair_in << std::endl;
    	std::cerr << "Set pair using -f option. See usage: ." << std::endl;
    	return -1;
  	}

	same_person sp;
	diff_person dp;
	
	int switch_num = 300;
	int line_count = 0;
	int line_num = 0;
	bool is_sp = true;

	std::vector<float> same_face_score, diff_face_score;

	while(!pair.eof())
	{
		line_count++;
		line_num++;

		if(line_count > switch_num)
		{
			if(is_sp)
				is_sp = false;
			else
				is_sp=true;

			line_count = 1;
		}

		std::cout << "line_num: " << line_num;

		if(is_sp)   /* same person data  */
		{
			pair >> sp.name >> sp.img_n1 >> sp.img_n2;

			/* for test */
			std::cout << " same person: name: " << sp.name << " img_n1: " << sp.img_n1 << " img_n2: " << sp.img_n2 << std::endl;

			float feature1[256],feature2[256];

			std::string img_name1 = get_name(sp.name,sp.img_n1);
			std::string img_name2 = get_name(sp.name,sp.img_n2);
			
			std::ifstream f1(img_name1,std::ios::in);
			std::ifstream f2(img_name2,std::ios::in);
			
			for(int i=0;i<256;i++)
			{
				f1>>feature1[i];
				f2>>feature2[i];
			}

			f1.close();
			f2.close();

			float score_s = compare(feature1, feature2);
			same_face_score.push_back(score_s);

		}
		else
		{
			pair >> dp.name1 >> dp.img_n1 >> dp.name2 >> dp.img_n2;

			/* for test */
			std::cout << " diff person: name1: " << dp.name1 << " img_n1: " << dp.img_n1 << " name2: " << dp.name2 << " img_n2: " << dp.img_n2 << std::endl;

			float feature1[256],feature2[256];

			std::string img_name1 = get_name(dp.name1,dp.img_n1);
			std::string img_name2 = get_name(dp.name2,dp.img_n2);
			
			std::ifstream f1(img_name1,std::ios::in);
			std::ifstream f2(img_name2,std::ios::in);
			
			for(int i=0;i<256;i++)
			{
				f1 >> feature1[i];
				f2 >> feature2[i];
			}

			f1.close();
			f2.close();

			float score_d = compare(feature1, feature2);
			diff_face_score.push_back(score_d);

			
		}

		if (line_num >6000)
			break;


	}

	std::sort(same_face_score.begin(),same_face_score.end());
	std::ofstream score_s("/home/firefly/benchmark/same_face_score.txt",std::ios::app);
	for(int i=0;i<same_face_score.size();i++)
		score_s << std::setiosflags(std::ios::fixed) << std::setprecision(4) << same_face_score[i] << std::endl;
	score_s.close();

	std::sort(diff_face_score.begin(),diff_face_score.end());
	std::ofstream score_d("/home/firefly/benchmark/diff_face_score.txt",std::ios::app);
	for(int i=0;i<diff_face_score.size();i++)
		score_d << std::setiosflags(std::ios::fixed) << std::setprecision(4) << diff_face_score[i] << std::endl;
	score_d.close();

	for(int i=0;i<=1000;i++)
	{
		float th = i * 0.001;
		for (int i=0;i<same_face_score.size();i++)
    	{
        	if(same_face_score[i] >= th)
				TP++;
			else
				FN++;
    	}
		for (int i=0;i<diff_face_score.size();i++)
    	{
        	if(diff_face_score[i]  >= th)
				FP++;
			else
				TN++;
    	}

		float tpr = TP * 1.0 / (TP + FN);
		float fpr = FP * 1.0 / (FP + TN);
		float pre = TP * 1.0 / (TP + FP);

		TPR.push_back(tpr);
		FPR.push_back(fpr);
		PRE.push_back(pre);
		
	}

	std::ofstream roc("/home/firefly/benchmark/ROC_LightenedCNN.txt",std::ios::out);
	for(int i=0;i<TPR.size();i++)
		roc << std::setiosflags(std::ios::fixed) << std::setprecision(4) << FPR[i] << " " << TPR[i]<< std::endl;
	roc.close();

	std::ofstream roc_d("/home/firefly/benchmark/PR_LightenedCNN.txt",std::ios::out);
	for(int i=0;i<TPR.size();i++)
		roc_d << std::setiosflags(std::ios::fixed) << std::setprecision(4) << TPR[i] << " " << PRE[i] << std::endl;
	roc_d.close();
	
	return 0;
}


