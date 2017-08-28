#include "face_mem_store.hpp"

int face_mem_store::insert_new_record(face_info& new_record)
{
	if(cur_record_num_==max_record_num_)
		return -1;

//	if(find_record(new_record.face_id))
//		return -1;

        remove_record(new_record.face_id);

	face_info * p_info=new face_info();

	*p_info=new_record;

	data_list_.push_back(p_info);

	cur_record_num_++;

	return 0;
};

int face_mem_store::get_all_records(std::vector<face_info*>& list)
{
	list.insert(list.end(),data_list_.begin(),data_list_.end());

	return list.size();
}


int face_mem_store::remove_record(int face_id)
{
	std::vector<face_info *>::iterator it=data_list_.begin();

	while(it!=data_list_.end())
	{
		if((*it)->face_id == face_id)
			break;
		it++;
	}

	if(it==data_list_.end())
		return -1;

	delete *it;

	data_list_.erase(it);

	cur_record_num_--;

	return 0; 
}

int face_mem_store::remove_record(const std::string& name)
{
	std::vector<face_info *>::iterator it=data_list_.begin();
	int count=0;

	while(it!=data_list_.end())
	{

		if((*it)->name == name)
		{
			cur_record_num_--;
			delete(*it);
			data_list_.erase(it);
			count++;
		}
		else
			it++;
	}

	return count;
}

face_info * face_mem_store::find_record(int face_id)
{
	std::vector<face_info *>::iterator it=data_list_.begin();

	while(it!=data_list_.end())
	{
		if((*it)->face_id == face_id)
			break;
		it++;
	} 

	if(it==data_list_.end())
		return nullptr;

	return *it;
}

int face_mem_store::find_record(const std::string& name, std::vector<face_info *>& list)
{

	std::vector<face_info *>::iterator it=data_list_.begin();

	while(it!=data_list_.end())
	{
		if((*it)->name == name)
			list.push_back(*it); 
		it++;
	}

	return list.size();
}


