#pragma once
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <QDebug>

class ObjectCounter;

#define DBG_COUNT(x) ObjectCounter objectCounter = ObjectCounter(x)

class ObjectCounter
{
public:
    ObjectCounter(const ObjectCounter & ref)
    {
        //type = 2;
        name = ref.name;
        objects.push_back(this);
        objects_map[name].push_back(this);
    }
    ObjectCounter(std::string obj_name)
    {
        //type = 1;
        name = obj_name;
        objects.push_back(this);
        objects_map[name].push_back(this);
    }

    ~ObjectCounter()
    {
        auto it = std::find(objects.begin(), objects.end(),this);
        if(it != objects.end())
        {
            objects.erase(it);
        }

        auto itm = std::find(objects_map[this->name].begin(), objects_map[this->name].end(), this);
        if(itm != objects_map[this->name].end())
        {
            objects_map[this->name].erase(itm);
        }
    }

    static void print(std::stringstream &ss){
        ss << "***********************" << std::endl;
        //printVectorObjects();
        printMapObjects(ss);
        ss << "***********************" << std::endl;
    }

    static void printQt(){
        qDebug() << "***********************";
        //printVectorObjects();
        printMapObjectsQt();
        qDebug() << "***********************";
    }

    static void print()
    {
        std::cout << "***********************" << std::endl;
        //printVectorObjects();
        printMapObjects();
        std::cout << "***********************" << std::endl;
    }


private:
    static void printVectorObjects()
    {

        std::cout << "VECTOR: size = " << objects.size() << std::endl;
        std::vector<ObjectCounter *>::iterator pos;
        for(pos = objects.begin(); pos != objects.end(); ++pos)
        {

            std::cout << "   Object Name = " << (*pos)->name << std::endl;
            //cout << "           Type = " << (*pos)->type << endl;

        }
    }
    static void printMapObjects()
    {
        std::cout << "MAP:" << std::endl;
        std::map<std::string, std::vector<ObjectCounter *> >::iterator positon;
        for( positon = objects_map.begin(); positon != objects_map.end(); ++positon)
        {
            if(positon->second.size() > 0)
                std::cout << "   " << positon->first << " " << positon->second.size() << std::endl;
        }
    }

    static void printMapObjectsQt()
    {
        qDebug() << "MAP:";
        std::map<std::string, std::vector<ObjectCounter *> >::iterator positon;
        for( positon = objects_map.begin(); positon != objects_map.end(); ++positon)
        {
            if(positon->second.size() > 0)
                qDebug() << "   " << positon->first << " " << positon->second.size();
        }
    }


    static void printMapObjects(std::stringstream &ss)
    {
        ss << "MAP:" << std::endl;
        std::map<std::string, std::vector<ObjectCounter *> >::iterator positon;
        for( positon = objects_map.begin(); positon != objects_map.end(); ++positon)
        {
            if(positon->second.size() > 0)
                ss << "   " << positon->first << " " << positon->second.size() << std::endl;
        }
    }


private:
    std::string name;
    // int type = 0;
    static std::vector<ObjectCounter *> objects;
    static std::map<std::string, std::vector<ObjectCounter *> > objects_map;
};
