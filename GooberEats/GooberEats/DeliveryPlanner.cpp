#include "provided.h"
#include <vector>
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
    const StreetMap* sm;
    PointToPointRouter ptr_rtr;
    DeliveryOptimizer del_opt;
    string chooseDir(const StreetSegment& line) const;
    
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
: ptr_rtr(sm), del_opt(sm)
{
    this->sm = sm;
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
    double oldCrowDist;
    double newCrowDist;
    
    vector<DeliveryRequest> newDeliveries;
    newDeliveries = deliveries;
    
    //Optimize the order of the deliveries
    del_opt.optimizeDeliveryOrder(depot, newDeliveries, oldCrowDist, newCrowDist);
    
    GeoCoord start = depot;
    GeoCoord end;
    list<StreetSegment> route;
    double distTravelled = 0;
    string currStreet;
    
    //For each delivery and then once more to go back to the depot
    for (int i = 0; i < newDeliveries.size() + 1; i++)
    {
        list<StreetSegment> route;
        
        DeliveryResult isPossible;
        
        //If this is for a delivery, the ending point of the route is the location of the delivery
        if (i < newDeliveries.size())
            isPossible = ptr_rtr.generatePointToPointRoute(start, newDeliveries[i].location, route, distTravelled);
        //If this is to return back to the depot, the ending point is the depot
        else
            isPossible = ptr_rtr.generatePointToPointRoute(start, depot, route, distTravelled);
        
        //Return based on issue, if there is one
        if (isPossible == NO_ROUTE)
            return NO_ROUTE;
        
        if (isPossible == BAD_COORD)
            return BAD_COORD;
        
        double distOfProceed = 0;
        auto k = route.begin();
        string currStreet = (*k).name;
        string dir;
        
        //For each segment in the route
        for ( ; k != route.end(); k++)
        {
            //If the name of the street is not equal to the name of the street we have been proceeding on
            //then you must turn
            if ((*k).name != currStreet)
            {
                //Generate the proceed command for the street we have been proceeding on before generating
                //the turn command
                DeliveryCommand delComProceed;
                delComProceed.initAsProceedCommand(dir, currStreet, distOfProceed);
                commands.push_back(delComProceed);
                //Reset the distance we have been proceeding on this street to 0 bc we just started on a new
                //street
                distOfProceed = 0;

                //Decide the angle of the turn and generate the turn command
                DeliveryCommand delComTurn;
                StreetSegment prevSeg = *(--k);
                k++;
                double angleOfTurn = angleBetween2Lines(prevSeg, (*k));

                if (angleOfTurn < 1 || angleOfTurn > 359)
                {
                    distOfProceed += distanceEarthMiles((*k).start, (*k).end);
                    dir = chooseDir((*k));
                    continue;
                }

                string turnDir;
                if (angleOfTurn >= 1 && angleOfTurn < 180)
                    turnDir = "left";
                else
                    turnDir = "right";

                delComTurn.initAsTurnCommand(turnDir, (*k).name);
                commands.push_back(delComTurn);
                currStreet = (*k).name;
            }

            //Add to the distance we have been proceeding and the distance we have travelled total the length
            //of the current segment and choose the direction that we are proceeding in
            distOfProceed += distanceEarthMiles((*k).start, (*k).end);
            dir = chooseDir((*k));
            distTravelled += distanceEarthMiles((*k).start, (*k).end);

            //If this is for a delivery and the end point of the current segment is equal to the location of
            //the delivery
            if ((i < newDeliveries.size() && (*k).end == newDeliveries[i].location))
            {
                //Generate a proceed command for the previous proceeding
                DeliveryCommand delComProceed;
                delComProceed.initAsProceedCommand(dir, currStreet, distOfProceed);
                commands.push_back(delComProceed);
                distOfProceed = 0;

                //Generate a delivery command
                DeliveryCommand deliv;
                deliv.initAsDeliverCommand(newDeliveries[i].item);
                commands.push_back(deliv);
            }
            
            //If this is not for a delivery and the end location of the current segment is the depot
            //generate a proceed command only for the previous proceeding only this segment
            else if  (i == newDeliveries.size() && (*k).end == depot)
            {
                DeliveryCommand delComProceed;
                delComProceed.initAsProceedCommand(dir, currStreet, distOfProceed);
                commands.push_back(delComProceed);
                distOfProceed = 0;
            }

        }
        
        //If there are still deliveries to be done, set the new start to the location of the delivery
        if (i != newDeliveries.size())
            start = newDeliveries[i].location;
    }
    
    //Set the total distance travelled and return success
    totalDistanceTravelled = distTravelled;
    return DELIVERY_SUCCESS;

}

string DeliveryPlannerImpl::chooseDir(const StreetSegment& line) const
{
    //Based on the angle of the street segment return a direction to move in
    double angle = angleOfLine(line);
    
    if (angle >= 0 && angle < 22.5)
        return "east";
    if (angle < 67.5)
        return "northeast";
    if (angle < 112.5)
        return "north";
    if (angle < 157.7)
        return "northwest";
    if (angle < 202.5)
        return "west";
    if (angle < 247.5)
        return "southwest";
    if (angle < 292.5)
        return "south";
    if (angle < 337.5)
        return "southeast";
    
    return "east";
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
