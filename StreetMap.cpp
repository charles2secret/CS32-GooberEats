//
//  StreetMap.cpp
//  GooberEats
//
//  Created by Fate End on 2020/3/6.
//  Copyright © 2020 Charles Zhou. All rights reserved.
//

#include "provided.h"
#include "ExpandableHashMap.h"
#include <string>
#include <vector>
#include <functional>
#include <fstream>

using namespace std;

unsigned int hasher(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
    ExpandableHashMap<GeoCoord, vector<StreetSegment>> m_hashmap;
};

StreetMapImpl::StreetMapImpl()
{
    
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
    
    ifstream infile(mapFile);    // infile is a name of our choosing
    if ( ! infile )                // Did opening the file fail?
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }
    string line;
    while(getline(infile,line)){//repeatedly, check each line
        string streetName=line;
        getline(infile,line);
        istringstream iss(line);
        int number;
        iss>>number;
        for(int i=0;i<number;i++){
            getline(infile,line);
            istringstream iss(line);
            string start_latitude;
            string start_longitude;
            string end_latitude;
            string end_longitude;
            iss>>start_latitude>>start_longitude>>end_latitude>>end_longitude;
            GeoCoord s=GeoCoord(start_latitude, start_longitude);
            GeoCoord e=GeoCoord(end_latitude,end_longitude);
            vector<StreetSegment> temp;
            StreetSegment temp_segment=StreetSegment(s,e,streetName);
            if(m_hashmap.find(s)!=nullptr){
                vector<StreetSegment> p=*(m_hashmap.find(s));
                p.push_back(temp_segment);
                m_hashmap.associate(s,p);
            }
            else{
                temp.push_back(temp_segment);
                m_hashmap.associate(s, temp);
            }
            vector<StreetSegment> temp1;
            StreetSegment temp1_segment=StreetSegment(e,s,streetName);
            if(m_hashmap.find(e)!=nullptr){
                vector<StreetSegment> p=*(m_hashmap.find(e));
                p.push_back(temp1_segment);
                m_hashmap.associate(e,p);
            }
            else{
                temp1.push_back(temp1_segment);
                m_hashmap.associate(e, temp1);
            }
        }
    }
    return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    
    if(m_hashmap.find(gc)==nullptr){//call map function find()
        return false;
    }
    else{
        segs=*(m_hashmap.find(gc));
        return true;
    }
      // Delete this line and implement this function correctly
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
