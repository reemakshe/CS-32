#include "provided.h"
#include "ExpandableHashMap.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;


//
//bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v);
//bool parseDelivery(string line, string& lat, string& lon, string& item);
//
//int main(int argc, char *argv[])
//{
//    if (argc != 3)
//    {
//        cout << "Usage: " << argv[0] << " mapdata.txt deliveries.txt" << endl;
//        return 1;
//    }
//
//    StreetMap sm;
//
//    if (!sm.load(argv[1]))
//    {
//        cout << "Unable to load map data file " << argv[1] << endl;
//        return 1;
//    }
//
//    GeoCoord depot;
//    vector<DeliveryRequest> deliveries;
//    if (!loadDeliveryRequests(argv[2], depot, deliveries))
//    {
//        cout << "Unable to load delivery request file " << argv[2] << endl;
//        return 1;
//    }
//
//    cout << "Generating route...\n\n";
//
//    DeliveryPlanner dp(&sm);
//    vector<DeliveryCommand> dcs;
//    double totalMiles;
//    DeliveryResult result = dp.generateDeliveryPlan(depot, deliveries, dcs, totalMiles);
//    if (result == BAD_COORD)
//    {
//        cout << "One or more depot or delivery coordinates are invalid." << endl;
//        return 1;
//    }
//    if (result == NO_ROUTE)
//    {
//        cout << "No route can be found to deliver all items." << endl;
//        return 1;
//    }
//    cout << "Starting at the depot...\n";
//    for (const auto& dc : dcs)
//        cout << dc.description() << endl;
//    cout << "You are back at the depot and your deliveries are done!\n";
//    cout.setf(ios::fixed);
//    cout.precision(2);
//    cout << totalMiles << " miles travelled for all deliveries." << endl;
//}
//
//bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v)
//{
//    ifstream inf(deliveriesFile);
//    if (!inf)
//        return false;
//    string lat;
//    string lon;
//    inf >> lat >> lon;
//    inf.ignore(10000, '\n');
//    depot = GeoCoord(lat, lon);
//    string line;
//    while (getline(inf, line))
//    {
//        string item;
//        if (parseDelivery(line, lat, lon, item))
//            v.push_back(DeliveryRequest(item, GeoCoord(lat, lon)));
//    }
//    return true;
//}
//
//bool parseDelivery(string line, string& lat, string& lon, string& item)
//{
//    const size_t colon = line.find(':');
//    if (colon == string::npos)
//    {
//        cout << "Missing colon in deliveries file line: " << line << endl;
//        return false;
//    }
//    istringstream iss(line.substr(0, colon));
//    if (!(iss >> lat >> lon))
//    {
//        cout << "Bad format in deliveries file line: " << line << endl;
//        return false;
//    }
//    item = line.substr(colon + 1);
//    if (item.empty())
//    {
//        cout << "Missing item in deliveries file line: " << line << endl;
//        return false;
//    }
//    return true;
//}




int main()
{

    StreetMap sm;
    sm.load("/Users/reemakshetramade/Desktop/GooberEats/GooberEats/mapdata.txt");


    GeoCoord start("34.0724746", "-118.4923463");
    GeoCoord end("34.0736122", "-118.4927669");
    vector<StreetSegment> segs;

    list<StreetSegment> route;
    double distTravelled;

    PointToPointRouter ptr_rtr(&sm);
    ptr_rtr.generatePointToPointRoute(start, end, route, distTravelled);


//    char pt = 'A';
//
//    for (auto i = route.begin(); i != route.end(); i++)
//    {
//        cout << "Point " << pt << ": " << (*i).start.latitudeText << ", " << (*i).start.longitudeText << endl;
//        pt++;
//        cout << "Point " << pt << ": " << (*i).end.latitudeText << ", " << (*i).end.longitudeText << endl;
//    }

    vector<DeliveryRequest> delReqs;
//
//    GeoCoord geo1("34.0625329", "-118.4470263");
//    DeliveryRequest req1("Taco Bell", geo1);
//    delReqs.push_back(req1);
//
//    GeoCoord geo2("34.0706349", "-118.4492679");
//    DeliveryRequest req2("Ramen", geo2);
//    delReqs.push_back(req2);
//
//    GeoCoord geo3("34.0616291", "-118.4416199");
//    DeliveryRequest req3("Dosa", geo3);
//    delReqs.push_back(req3);
//
//    GeoCoord geo4("34.0712323", "-118.4505969");
//    DeliveryRequest req4("Drunken noodles", geo4);
//    delReqs.push_back(req4);
//
//    GeoCoord depot("34.0685657", "-118.4489289");

//    DeliveryOptimizer delOpt(&sm);
//    double oldDist;
//    double newDist;
//
//    delOpt.optimizeDeliveryOrder(depot, delReqs, oldDist, newDist);
//
//    for (int i = 0; i < delReqs.size(); i++)
//        cout << delReqs[i].item << endl;
//
//    cout << "Old Distance: " << oldDist << endl;
//    cout << "New Distance: " << newDist << endl;

    GeoCoord depot("34.0625329", "-118.4470263");
    GeoCoord geo1("34.0636671", "-118.4461842");
    DeliveryRequest req1("Sardines", geo1);
    delReqs.push_back(req1);


    DeliveryPlanner delPlanner(&sm);

    vector<DeliveryCommand> cmds;
    double dist;

    delPlanner.generateDeliveryPlan(depot, delReqs, cmds, dist);

    for (int i = 0; i < cmds.size(); i++)
        cout << cmds[i].description() << endl;




    /*
     Broxton and Weyburn: 34.0625329 -118.4470263
     Sproul Landing Intersection 34.0712323 -118.4505969
     De Neve Suites Intersection 34.0718238 -118.4525699
     Levering and Strathmore 34.0656797 -118.4505131
     Kelton and Ophir 34.0683189 -118.4536522
     Strathmore and Westwood Plaza (Eng IV) 34.0687443 -118.4449195
     Gayley and Strathmore (Beta Theta Pi frat) 34.0685657 -118.4489289
     De Neve Plaza @ Charles E Young Drive 34.0706349 -118.4492679
     Le Conte and Westwood Blvd 34.0636860 -118.4453568
     Broxton and Weyburn 34.0625329 -118.4470263
     Hilgard and Weyburn 34.0616291 -118.4416199
     Charles E Young and De Neve 34.0711774 -118.4495120
     Hilgard and Manning 34.0660665 -118.4385079
     */



}
