#ifndef __FACE_STORE_HPP__
#define __FACE_STORE_HPP__


#include <malloc.h>
#include <string.h>

#include <string>
#include <vector>

#define DEFUALT_MAX_RECORD_NUMBER 100

struct face_info
{
	int face_id;
	std::string name;
	float * p_feature;
	int feature_len;

	face_info() 
	{
		p_feature=nullptr;
	}

	~face_info()
	{
		if(p_feature)
			free(p_feature);
	}

	face_info & operator =(const face_info& r)
	{
		face_id=r.face_id;
		name=r.name;
		feature_len=r.feature_len;

		p_feature=(float *)malloc(sizeof(float)*feature_len);

		memcpy(p_feature,r.p_feature,feature_len*sizeof(float));

		return *this;
	}
};

class face_info_store
{
	public: 
		face_info_store(int feature_len):feature_len_(feature_len),max_record_num_(DEFUALT_MAX_RECORD_NUMBER){};

		virtual ~face_info_store(){};

		virtual int get_record_number()=0;
		virtual int get_all_records(std::vector<face_info*>& list)=0;

		virtual int insert_new_record(face_info& new_record)=0;
		virtual int remove_record(int face_id)=0;
		virtual int remove_record(const std::string& name)=0;
		virtual face_info * find_record(int face_id)=0;
		virtual int find_record(const std::string& name, std::vector<face_info *>& list)=0;


		void set_max_record_number(int n) {max_record_num_=n;};
		int  get_max_record_number(void) { return max_record_num_;};

	protected:

		int feature_len_;
		int max_record_num_;
};


#endif

