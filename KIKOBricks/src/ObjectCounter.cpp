#include "ObjectCounter.hpp"

std::vector<ObjectCounter *> ObjectCounter::objects;
std::map<std::string, std::vector<ObjectCounter *>> ObjectCounter::objects_map;
