#ifndef __FACE_MEM_STORE_HPP__
#define __FACE_MEM_STORE_HPP__

#include "face_store.hpp"

#include <vector>


class face_mem_store :public face_info_store 
{
	public:
		face_mem_store(int feature_len):face_info_store(feature_len),cur_record_num_(0){max_record_num_=10;};
		face_mem_store(int feature_len,int max_num):face_info_store(feature_len),cur_record_num_(0){max_record_num_=max_num;};
		~face_mem_store()
		{
			for(int i=0;i<data_list_.size();i++)
			{
				delete data_list_[i];
			}
		}

		int get_record_number() {return cur_record_num_;};

		int insert_new_record(face_info& new_record);

		int get_all_records(std::vector<face_info*>& list);

		int remove_record(int face_id);
		int remove_record(const std::string& name);
		face_info * find_record(int face_id);
		int find_record(const std::string& name, std::vector<face_info *>& list);


	private:
		int cur_record_num_;
		std::vector<face_info *> data_list_;
};


#endif
