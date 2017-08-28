#include <map>
#include <string>

#include "face_verify.hpp"


typedef std::map<std::string,face_verifier_creator>  creator_map;

static creator_map& get_registry(void)
{
	static creator_map * instance_ptr=new creator_map();

	return *instance_ptr;
}

face_verifier * get_face_verifier(const std::string& name)
{
     creator_map& registry=get_registry();

     if(registry.find(name)== registry.end())
		return nullptr;

     face_verifier_creator& func=registry[name];

    return func(name);
}

int register_face_verifier(const std::string& name, face_verifier_creator create_func)
{
     creator_map& registry=get_registry();

     registry[name]=create_func;

     return 0;
}
