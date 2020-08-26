#include "provided.h"
#include <vector>
#include <list>
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
    void quicksort(vector<DeliveryRequest>& deliveries, GeoCoord depot, int first, int last) const;
    int partition(vector<DeliveryRequest>& deliveries, GeoCoord depot, int first, int last) const;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
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
    //Calculate the old crow distance for the old order of deliveries
    oldCrowDistance = 0;
    GeoCoord start = depot;
    GeoCoord end;
    for (int i = 0; i < deliveries.size(); i++)
    {
        end = deliveries[i].location;
        oldCrowDistance += distanceEarthMiles(start, end);
        start = end;
    }
    
    //Sort the deliveries, in increasing order of distance from the depot
    vector<DeliveryRequest> newDelOrder;
    list<double> dists;
    
    quicksort(deliveries, depot, 0, deliveries.size() - 1);
    
    //Calculate the new crow distance
    GeoCoord startNew = depot;
    GeoCoord endNew;
    for (int i = 0; i < deliveries.size(); i++)
    {
        endNew = deliveries[i].location;
        newCrowDistance += distanceEarthMiles(startNew, endNew);
        startNew = endNew;
    }
}

void DeliveryOptimizerImpl::quicksort(vector<DeliveryRequest> &deliveries, GeoCoord depot, int first, int last) const
{
    if (last - first >= 1)
    {
        int pivotIndex;
        pivotIndex = partition(deliveries, depot, first, last);
        
        quicksort(deliveries, depot, first, pivotIndex - 1);
        quicksort(deliveries, depot, pivotIndex + 1, last);
    }
        
}

int DeliveryOptimizerImpl::partition(vector<DeliveryRequest> &deliveries, GeoCoord depot, int first, int last) const
{
    int pi = first;
    DeliveryRequest pivot = deliveries[first];
    
    do
    {
        while (first <= last && (distanceEarthMiles(deliveries[first].location, depot) <= distanceEarthMiles(pivot.location, depot)))
            first++;
        
        while (distanceEarthMiles(deliveries[last].location, depot) > distanceEarthMiles(pivot.location, depot))
            last--;
        
        if (first < last)
        {
            DeliveryRequest temp = deliveries[first];
            deliveries[first] = deliveries[last];
            deliveries[last] = temp;
        }
        
    } while (first < last);
    
    DeliveryRequest temp = deliveries[pi];
    deliveries[pi] = deliveries[last];
    deliveries[last] = temp;
    
    pi = last;
    return pi;
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
