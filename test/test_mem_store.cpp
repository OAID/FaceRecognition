#include <iostream>

#include "face_mem_store.hpp"




int main(int argc, char * argv[])
{
	int i;
	int f_len=256;
	int max_num=1000;


	face_mem_store new_store(f_len,max_num);

	face_info info;
	info.feature_len=f_len;
	info.p_feature=(float *)malloc(sizeof(float)*f_len);


	for(i=0;i<new_store.get_max_record_number()+2;i++)
	{

		info.face_id=i;
		info.name=std::string("test")+std::to_string(i/2);
		info.p_feature[0]=i;

		if(new_store.insert_new_record(info)<0)
		{
			std::cout<<"insert record failed at "<<i<<std::endl;
		}
	}

	std::cout<<"total stored "<<new_store.get_record_number()<<" records"<<std::endl;

	/* test search*/
	int face_id=100;

	face_info * p=new_store.find_record(face_id);

	std::cout<<"first float of (id: " <<p->face_id <<" name: "<<p->name<<") is: "<<p->p_feature[0]<<std::endl;

	/*erease  */

	new_store.remove_record(face_id);

	/* find again */

	p=new_store.find_record(face_id);

	if(p==nullptr)
	{
		std::cout<<face_id<<" is deleted"<<std::endl;
	}

	/* find by name */
	std::vector<face_info * > result;
	int n=new_store.find_record(std::string("test")+std::to_string(face_id+1),result);

	std::cout<<"total found "<<n<<" records"<<std::endl;

	/* remove by name */

	n=new_store.remove_record(std::string("test")+std::to_string(face_id+2));

	std::cout<<"total removed " <<n<<" records"<<std::endl;

	return 0;

}
