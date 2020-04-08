//
//  DeliveryOptimizer.cpp
//  GooberEats
//
//  Created by Fate End on 2020/3/6.
//  Copyright © 2020 Charles Zhou. All rights reserved.
//

#include "provided.h"
#include <vector>
#include <random>
using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
private:
    const StreetMap* m_streetMap;
    void calculate(const GeoCoord& depot,
                vector<DeliveryRequest>& deliveries,double &oldCrowDistance)const{
        size_t loop=deliveries.size();
        oldCrowDistance+=distanceEarthMiles(depot, deliveries[0].location);
        
        if(loop==1){
            oldCrowDistance+=distanceEarthMiles(deliveries[0].location, depot);
        }
        else{
            for(int i=0;i<loop-1;i++){
                oldCrowDistance+=distanceEarthMiles(deliveries[i].location, deliveries[i+1].location);
            }
            oldCrowDistance+=distanceEarthMiles(deliveries[loop-1].location, depot);
        }
        
    }
    int randInt(size_t min, size_t max)const
    {
        if (max < min)
            std::swap(max, min);
        static std::random_device rd;
        static std::default_random_engine generator(rd());
        std::uniform_int_distribution<> distro(min, max);
        return distro(generator);
    }
        
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
    m_streetMap=sm;
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
    
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
    
    oldCrowDistance=0;
    if(deliveries.empty()){
        newCrowDistance=0;
        return;
    }
    else{
        calculate(depot, deliveries, oldCrowDistance);
    }//compute the oldCrowDistance
    vector<DeliveryRequest> temp=deliveries;
    double old=oldCrowDistance;
    double temperature=10000;
    double coolingRate=0.003;
    double neW;
    size_t turn=temp.size()*temp.size()*temp.size()*temp.size();
    int check=0;
    while(temperature>1&&check<=turn){
        size_t one=randInt(0, temp.size()-1);
        size_t two=randInt(0, temp.size()-1);
        swap(temp[one], temp[two]);
        calculate(depot, temp, neW);
        if(neW<old){
            old=neW;
            deliveries=temp;
        }
        temperature*=(1-coolingRate);
        check++;
    }
    newCrowDistance=old;
    
}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}

