#include "provided.h"
#include "ExpandableHashMap.h"
#include <list>
#include <queue>
#include <set>
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
    const StreetMap* sm;
    void quicksort(vector<StreetSegment>& segs, GeoCoord end, int first, int last) const;
    int partition(vector<StreetSegment>& segs, GeoCoord end, int first, int last) const;

};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    this->sm = sm;
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
    //If the starting or ending coordinate is not found in the street map data, then return BAD_COORD
    vector<StreetSegment> possSegs;
    if (!(sm->getSegmentsThatStartWith(start, possSegs)) || !(sm->getSegmentsThatStartWith(end, possSegs)))
    {
        cerr << "Bad Coord" << endl;
        return BAD_COORD;
    }
      
    //Initialize a queue to hold the coordinates to be visited
    queue<GeoCoord> coordQ;
    //Initialize a map to hold the point to point route
    ExpandableHashMap<GeoCoord, GeoCoord> routeMap;
    //Initialize a set to hold the coordinates that have already been visited so they are not revisited
    set<GeoCoord> visitedCoords;
    set<GeoCoord>::iterator setIt;
    bool found = false;
    
    //Push the starting coordinate onto the queue and mark it as discovered
    coordQ.push(start);
    visitedCoords.insert(start);
    
    while (!coordQ.empty())
    {
        GeoCoord currCoord = coordQ.front();
        coordQ.pop();
        //If the current coordinate is the end, break from the loop and set the found status to true
        if (currCoord == end)
        {
            found = true;
            break;
        }
        
        //Find the segments that start with this point
        vector<StreetSegment> possibleSegs;
        bool anyPossibleSegs = sm->getSegmentsThatStartWith(currCoord, possibleSegs);
        
        //If there are any possible segments
        if (anyPossibleSegs)
        {
            //Order the segments based on their distance from the end point, least distance first
            //so that the segments least far away from the end point are explored first
            quicksort(possibleSegs, end, 0, possibleSegs.size() - 1);
            //For each possible street segment
            for (int i = 0; i < possibleSegs.size(); i++)
            {
                //Check whether the end coordinate has already been visited
                setIt = visitedCoords.find((possibleSegs[i]).end);
                
                //If it has not been visited, push the coordinate onto the queue and add the association
                //between this end point and the current point to the map
                if (setIt == visitedCoords.end())
                {
                    coordQ.push((possibleSegs[i]).end);
                    routeMap.associate(possibleSegs[i].end, currCoord);
                }
                
                //Insert it into the set holding already visited coords
                visitedCoords.insert((possibleSegs[i]).end);
            }
        }

    }
    
    //If the route has been found
    if (found)
    {
        totalDistanceTravelled = 0;
        //Find the route by following the reverse of the route from the end to the start and push the
        //segments created by these points onto route
        GeoCoord* startOfCurrSeg = routeMap.find(end);
        const GeoCoord* endOfCurrSeg = &end;
        while (*startOfCurrSeg != start)
        {
            startOfCurrSeg = routeMap.find(*endOfCurrSeg);
            sm->getSegmentsThatStartWith(*startOfCurrSeg, possSegs);
            string strName;
            //For the possible segments that start with this coordinate, if the end of the segment is
            //equal to the current end of the segment, the street name is the name of this segment
            for (int i = 0; i < possSegs.size(); i++)
            {
                if (possSegs[i].end == *endOfCurrSeg)
                {
                    strName = possSegs[i].name;
                    break;
                }
            }
            route.push_front(StreetSegment(*startOfCurrSeg, *endOfCurrSeg, strName));
            totalDistanceTravelled += distanceEarthMiles(*startOfCurrSeg, *endOfCurrSeg);
            endOfCurrSeg = startOfCurrSeg;
        }
        //Since a route was found, return delivery success
        return DELIVERY_SUCCESS;
    }
    
    //No route was found
    return NO_ROUTE;
}

//Sort street segments in ascending order of distance from end using quicksort
void PointToPointRouterImpl::quicksort(vector<StreetSegment> &segs, GeoCoord end, int first, int last) const
{
    if (last - first >= 1)
    {
        int pivotIndex;
        pivotIndex = partition(segs, end, first, last);
        
        quicksort(segs, end, first, pivotIndex - 1);
        quicksort(segs, end, pivotIndex + 1, last);
    }
        
}

//Partition everything around a pivot value (function used in quicksort)
int PointToPointRouterImpl::partition(vector<StreetSegment> &segs, GeoCoord end, int first, int last) const
{
    int pi = first;
    StreetSegment pivot = segs[first];
    
    do
    {
        //Sort based on the distance from the end of the segment to the end location of the route
        while (first <= last && (distanceEarthMiles(segs[first].end, end) <= distanceEarthMiles(pivot.end, end)))
            first++;
        
        while (distanceEarthMiles(segs[last].end, end) > distanceEarthMiles(pivot.end, end))
            last--;
        
        if (first < last)
        {
            StreetSegment temp = segs[first];
            segs[first] = segs[last];
            segs[last] = temp;
        }
        
    } while (first < last);
    
    StreetSegment temp = segs[pi];
    segs[pi] = segs[last];
    segs[last] = temp;
    
    pi = last;
    return pi;
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
