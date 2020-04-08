//
//  DeliveryPlanner.cpp
//  GooberEats
//
//  Created by Fate End on 2020/3/6.
//  Copyright © 2020 Charles Zhou. All rights reserved.
//

#include "provided.h"
#include <vector>
#include <map>
using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
private:
    const StreetMap *m_streetMap;
    string calculateDir(const StreetSegment& line)const{
        double dir=angleOfLine(line);
        if(dir>=0&&dir<22.5){
            return "east";
        }
        else if(dir>=22.5&&dir<67.5){
            return "northeast";
        }
        else if(dir>=67.5&&dir<112.5){
            return "north";
        }
        else if(dir>=112.5&&dir<157.5){
            return "northwest";
        }
        else if(dir>=157.5&&dir<202.5){
            return "west";
        }
        else if(dir>=202.5&&dir<247.5){
            return "southwest";
        }
        else if(dir>=247.5&&dir<292.5){
            return "south";
        }
        else if(dir>=292.5&&dir<337.5){
            return "southeast";
        }
        else {
            return "east";
        }
    }
    string calculateTurn(const StreetSegment& line1, const StreetSegment& line2) const{
        double turn=angleBetween2Lines(line1, line2);
        if(turn<1||turn>359){
            return "No";
        }
        else if(turn>=1&&turn<180){
            return "left";
        }
        else{
            return "right";
        }
    }
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
{
    m_streetMap=sm;
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
    
}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    GeoCoord end1=depot;
    vector<StreetSegment> temp0;
    if(!m_streetMap->getSegmentsThatStartWith(depot, temp0)){
        return BAD_COORD;
    }
    DeliveryOptimizer m(m_streetMap);
    PointToPointRouter p(m_streetMap);
    vector<DeliveryRequest> temp=deliveries;
    double old,New;
    m.optimizeDeliveryOrder(depot, temp, old, New);
    DeliveryRequest end("true",end1);
    temp.push_back(end);
    size_t loop=temp.size();
    
    GeoCoord start=depot;
    for(int i=0;i<loop;i++){
        string item=temp[i].item;
        GeoCoord location=temp[i].location;
        list<StreetSegment> route;
        double travelDistance;
        if(p.generatePointToPointRoute(start, location, route, travelDistance)!=DELIVERY_SUCCESS){
            return p.generatePointToPointRoute(start, location, route, travelDistance);
        }
        
        StreetSegment first_segs=route.front();
        StreetSegment start_segs=first_segs;
        string s_dir=calculateDir(start_segs);
        string s_name=start_segs.name;
        double totalDis=distanceEarthMiles(start_segs.start, start_segs.end);
        StreetSegment end_segs;
        route.pop_front();
        while(!route.empty()){
            end_segs=route.front();
            if(start_segs.name==end_segs.name){
                totalDis+=distanceEarthMiles(end_segs.start, end_segs.end);
            }
            else{
                DeliveryCommand DC1;
                DC1.initAsProceedCommand(s_dir, start_segs.name, totalDis);
                totalDistanceTravelled+=totalDis;
                commands.push_back(DC1);
                string turn=calculateTurn(start_segs, end_segs);
                if(turn !="No"){
                    DeliveryCommand DC2;
                    DC2.initAsTurnCommand(turn, end_segs.name);
                    commands.push_back(DC2);
                }
                totalDis=distanceEarthMiles(end_segs.start, end_segs.end);
                s_dir=calculateDir(end_segs);
                
            }
            start_segs=end_segs;
            route.pop_front();
        }
        DeliveryCommand DC3;
        DC3.initAsProceedCommand(s_dir, end_segs.name, totalDis);
        totalDistanceTravelled+=totalDis;
        commands.push_back(DC3);
        if(location!=depot){
            DeliveryCommand DC4;
            DC4.initAsDeliverCommand(item);
            commands.push_back(DC4);
        }
        start=location;
        
        
    }
    return DELIVERY_SUCCESS;  // Delete this line and implement this function correctly
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}

