#include "provided.h"
#include "ExpandableHashMap.h"
#include <string>
#include <vector>
#include <functional>
#include <iostream>
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
    ExpandableHashMap<GeoCoord, vector<StreetSegment>> hash_map;
};

StreetMapImpl::StreetMapImpl()
{
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
    //Create a file reading stream
    ifstream infile(mapFile);
    
    if (!infile)
        return false;
    
    string s;
    int i = 0;
    
    //Get each line in the file
    while (getline(infile, s))
    {
        //The first line processed by the loop each run should be the name of the street
        string strName = s;
        //The next line should be the number of street segments listed after that line
        string numSeg;
        getline(infile, numSeg);
        //Convert that string to an integer
        i = stoi(numSeg);
        GeoCoord startCoord;
        GeoCoord endCoord;

        for (int j = 0; j < i; j++)
        {
            //Get the coordinate of the street segment
            string segCoord;
            getline(infile, segCoord);
            //The latitude of the first coordinate will be the portion of the string until the first space
            int pos = segCoord.find(' ', 0);
            int startIndexOfCoord = 0;
            string startLat = segCoord.substr(startIndexOfCoord, pos - startIndexOfCoord);
            startIndexOfCoord = pos + 1;
            //The longitude of the first coordinate will be the portion of the string from the previous space
            //until the next space
            pos = segCoord.find(' ', startIndexOfCoord);
            string startLong = segCoord.substr(startIndexOfCoord, pos - startIndexOfCoord);
            startIndexOfCoord = pos + 1;
            //The latitude of the second coordinate will be the portion of the string from the previous space
            //until the next space
            pos = segCoord.find(' ', startIndexOfCoord);
            string endLat = segCoord.substr(startIndexOfCoord, pos - startIndexOfCoord);
            //The longitude of the second coordinate will be the portion of the string from the previous space
            //until the end of the string
            string endLong = segCoord.substr(pos + 1);
                        
            //Create the geo coordinates with the starting and ending coordinates gathered
            startCoord = GeoCoord(startLat, startLong);
            endCoord = GeoCoord(endLat, endLong);

            //Check whether that coordinate is already in the map
            vector<StreetSegment>* isInMap;
            isInMap = hash_map.find(startCoord);
            
            //Create the Street Segment that is in the original orientation
            StreetSegment streetSeg(startCoord, endCoord, strName);
            
            //If the coordinate is not already in the map, create a vector of street segments and add the
            //current segment and associate this pair in the map
            if (isInMap == nullptr)
            {
        
                vector<StreetSegment> streetSegs;
                streetSegs.push_back(streetSeg);
                hash_map.associate(startCoord, streetSegs);
            }
            //If the coordinate is already in the map, push the current segment onto the vector associated
            //with that coordinate in the map
            else
                (*isInMap).push_back(streetSeg);
            
            //Check whether the starting coordinate of the reverse segment is in the map
            isInMap = hash_map.find(endCoord);
            
            //Generate the reverse street segment
            StreetSegment streetSegRev(endCoord, startCoord, strName);
            //If the coordinate is not already in the map, create a vector of street segments and add the
            //reverse segment and associate this pair in the map
            if (isInMap == nullptr)
            {
                vector<StreetSegment> streetSegsRev;
                streetSegsRev.push_back(streetSegRev);
                hash_map.associate(endCoord, streetSegsRev);
            }
            //If the coordinate is already in the map, push the reverse segment onto the vector associated
            //with that coordinate in the map
            else
                (*isInMap).push_back(streetSegRev);
                
        }
        
        
    }
    
    //Return true because all the data has been read
    return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    //Initialize and set a pointer to the value associated with the key in the map
    const vector<StreetSegment>* valueptr;
    valueptr = hash_map.find(gc);
    vector<StreetSegment> segsFound;
    
    //If the returned ponter to the value is nullptr, return false because that coordinate is not in the map
    if (valueptr == nullptr)
        return false;
    //Otherwise, push each StreetSegment in the value returned onto the segsFound vector
    else
    {
        for (int i = 0; i < (*valueptr).size(); i++)
        {
            StreetSegment segFound = (*valueptr)[i];
            segsFound.push_back(segFound);
        }
    }
    
    //Swap the two vectors and return true
    segs.swap(segsFound);
    return true; 
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
