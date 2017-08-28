#ifndef __FACE_VERIFY_HPP__
#define __FACE_VERIFY_HPP_


#include <string>


class  face_verifier
{
public:
   virtual float compare(float * f0, float * f1, int len)=0;

   virtual int search(float * f, int * p_idx, float * p_score)=0;
   virtual int insert_feature(float * feature, unsigned int face_id)=0;
   virtual void set_feature_len(int feature_len)=0;
   virtual void remove_feature(unsigned int face_id)=0;

protected:

   int feature_len_;
   std::string name_;
};

typedef face_verifier * (*face_verifier_creator)(const std::string& name);

face_verifier * get_face_verifier(const std::string& name);

int register_face_verifier(const std::string& name, face_verifier_creator verifier);


#endif
