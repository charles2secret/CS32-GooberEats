//
//  PointToPointRouter.cpp
//  GooberEats
//
//  Created by Fate End on 2020/3/6.
//  Copyright © 2020 Charles Zhou. All rights reserved.
//

#include "provided.h"
#include <list>
#include <queue>
#include <map>
#include <stack>
using namespace std;

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
private:
    const StreetMap * m_streetMap;
    
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    m_streetMap=sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
    
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    route.erase(route.begin(), route.end());
    int status=0;
    stack<GeoCoord> coordStack;
    queue<GeoCoord> coordQueue;
    vector<StreetSegment> segs;
    map<GeoCoord,bool> m;
    map<GeoCoord,GeoCoord> locationOfPreviousPoints;
    if(!m_streetMap->getSegmentsThatStartWith(start, segs)||!m_streetMap->getSegmentsThatStartWith(end, segs)){
        
        return BAD_COORD;
    }
    if(start==end){
        totalDistanceTravelled=0;
        return DELIVERY_SUCCESS;
    }
    coordQueue.push(start);
    m[start]=true;
    while(!coordQueue.empty()){
        vector<StreetSegment> seg;
        GeoCoord cur=coordQueue.front();
        coordQueue.pop();
        m_streetMap->getSegmentsThatStartWith(cur, seg);
       
        for(int i=0;i<seg.size();i++){
            StreetSegment temp=seg[i];
            if(!m[temp.end]){
                coordQueue.push(temp.end);
                locationOfPreviousPoints[temp.end]=cur;
                m[temp.end]=true;

                if(temp.end==end){
                    status=1;
                    break;
                }
               
            }
        }
        
        if(status==1){
            break;
        }
        
    }
    if(status==0){
       
        return NO_ROUTE;
    }
    else{
        GeoCoord starter;
        GeoCoord ender;
        ender=end;
        coordStack.push(ender);
        while(starter!=start){
            starter=locationOfPreviousPoints[ender];
            coordStack.push(starter);
            ender=starter;
            
        }
        while(!coordStack.empty()&&coordStack.size()>=2){
            
            GeoCoord s=coordStack.top();
            coordStack.pop();
            GeoCoord e=coordStack.top();
            vector<StreetSegment> tempSegs;
            m_streetMap->getSegmentsThatStartWith(s, tempSegs);
            vector<StreetSegment>::iterator it;
            
            for(it=tempSegs.begin();it!=tempSegs.end();it++){
                if(s==it->start&&e==it->end){
                    totalDistanceTravelled+=distanceEarthMiles(s, e);
                    route.push_back(*it);
                }
            }
            
        }
        
        return DELIVERY_SUCCESS;
    }
            // Delete this line and implement this function correctly
}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}

