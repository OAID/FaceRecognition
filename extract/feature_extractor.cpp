#include <map>
#include <string>

#include "feature_extractor.hpp"

typedef std::map<std::string,extractor_factory::creator> creator_map;

static creator_map & get_registry(void)
{
   static creator_map * instance_ptr=new creator_map;

   return (*instance_ptr);
}


void extractor_factory::register_creator(const std::string&name, extractor_factory::creator& create_func)
{
   creator_map& registry=get_registry();

   registry[name]=create_func;
}


feature_extractor * extractor_factory::create_feature_extractor(const std::string& name)
{
    std::vector<std::string> ret;

    creator_map& registry=get_registry();

    if(registry.find(name)==registry.end())
        return nullptr;

     creator func=registry[name];

    return func(name);
}

std::vector<std::string> extractor_factory::list_extractor(void)
{
 
    std::vector<std::string> ret;

    creator_map& registry=get_registry();

    creator_map::iterator it=registry.begin();
 
    while(it!=registry.end())
    {
        ret.push_back(it->first);
        it++;
    }

    return ret;
}


