#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

//Set private variables and call parent class constructor
StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    numBacteriaAlive = 0;
    numPits = 0;
}

int StudentWorld::init()
{
    //Initialize Socrates actor
    m_socrates = new Socrates(this);
    
    double angle;
    double radius;
    double x;
    double y;

    //Initialize Pits at random positions as specified and they are not allowed to overlap with
    //previously constructed Pits
    for (int i = 0; i < getLevel(); i++)
    {
        angle = randInt(0, 360) * (3.14159 / 180);
        radius = randInt(0, 120);

        x = radius * (cos(angle));
        y = radius * (sin(angle));
        x = (VIEW_WIDTH / 2) + x;
        y = (VIEW_WIDTH / 2) + y;


        std::list<Actor*>::iterator it = m_actors.begin();

        while (it != m_actors.end())
        {
            double dist = sqrt(pow(x - ((*it)->getX()), 2) + pow(y - (*it)->getY(), 2));

            if (dist < 2 * SPRITE_RADIUS)
                break;

            it++;
        }

        if (it != m_actors.end())
        {
            i--;
            continue;
        }
        else
        {
            m_actors.push_back(new Pit(x, y, this));
            numBacteriaAlive += 10;
            numPits++;
        }

    }
    
    //Initialize Food at random positions as specified and they are not allowed to overlap with
    //previously constructed Pits and Food
    for (int i = 0; i < min(5 * getLevel(), 25); i++)
    {
        angle = randInt(0, 360) * (3.14159 / 180);
        radius = randInt(0, 120);

        x = radius * (cos(angle));
        y = radius * (sin(angle));
        x = (VIEW_WIDTH / 2) + x;
        y = (VIEW_WIDTH / 2) + y;

         std::list<Actor*>::iterator it = m_actors.begin();

         while (it != m_actors.end())
         {
             double dist = sqrt(pow(x - ((*it)->getX()), 2) + pow(y - (*it)->getY(), 2));

             if (dist < 2 * SPRITE_RADIUS)
                 break;

             it++;
         }

         if (it != m_actors.end())
         {
             i--;
             continue;
         }
         else
             m_actors.push_back(new Food(x, y, this));
    }
    
    double xDirt;
    double yDirt;
    double radiusDirt;
    
    //Initialize Dirt Piles at random positions as specified and they are not allowed to overlap with
    //previously constructed Pits and Food, but can overlap with DirtPiles
    for (int i = 0; i < max(180 - 20 * getLevel(), 20); i++)
    {
        
        xDirt = randInt(0, VIEW_WIDTH);
        yDirt = randInt(0, VIEW_HEIGHT);
        
        radiusDirt = sqrt(pow(xDirt - (VIEW_WIDTH / 2), 2) + pow(yDirt - (VIEW_HEIGHT / 2), 2));
        
        if (!(radiusDirt <= 120))
        {
            i--;
            continue;
        }
        
        std::list<Actor*>::iterator it = m_actors.begin();
        bool doesOverlap = false;
        
        while (it != m_actors.end())
        {
            double dist = sqrt(pow(xDirt - ((*it)->getX()), 2) + pow(yDirt - (*it)->getY(), 2));
            
            if (dist < 2 * SPRITE_RADIUS && !((*it)->canOverlapWithDirtPile()))
            {
                doesOverlap = true;
                break;
            }
            it++;
        }
        
        if (doesOverlap)
            i--;
        else
            
            m_actors.push_back(new DirtPile(xDirt, yDirt, this));
        
    }
    
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    //Allow Socrates to do something
    m_socrates->doSomething();

    list<Actor*>:: iterator it = m_actors.begin();
    
    //Allow all the actors to do something
    while (it != m_actors.end())
    {
        (*it)->doSomething();

        //if socrates dies, return that status and decrement his number of lives
        if (m_socrates->isDead())
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        
        //if all the bacteria are dead and all the pits are empty, socrates has finished the level
        if (numBacteriaAlive == 0 && numPits == 0)
            return GWSTATUS_FINISHED_LEVEL;
        
        it++;
    }

    //Remove all the dead actors
    it = m_actors.begin();

    while (it != m_actors.end())
    {
        if ((*it)->isDead())
        {
            delete *it;
            list<Actor*>::iterator tempIt = it;
            it--;
            m_actors.erase(tempIt);
        }
        else
            it++;
    }

    //Randomly generate fungus with this chance at specified position
    int chanceFungus = min(510 - getLevel() * 10, 200);
    int randFungus = randInt(0, chanceFungus - 1);
    
    if (randFungus == 0)
    {
        int fungusAngle = randInt(0, 359);
        double fungusX = VIEW_RADIUS * (cos(fungusAngle));
        double fungusY = VIEW_RADIUS * (sin(fungusAngle));
        fungusX = (VIEW_WIDTH / 2) + fungusX;
        fungusY = (VIEW_WIDTH / 2) + fungusY;
        m_actors.push_back(new Fungus(fungusX, fungusY, this));
    }

    //Randomly generate goodies with this chance at specified position
    int chanceGoodie = min(510 - getLevel() * 10, 250);
    double randGoodie = randInt(0, chanceGoodie);

    if (randGoodie == 0)
    {
        int num = randInt(1, 10);
        
        int goodieAngle = randInt(0, 359);
        double goodieX = VIEW_RADIUS * (cos(goodieAngle));
        double goodieY = VIEW_RADIUS * (sin(goodieAngle));
        goodieX = (VIEW_WIDTH / 2) + goodieX;
        goodieY = (VIEW_WIDTH / 2) + goodieY;

        if (num >= 1 && num <= 6)
            m_actors.push_back(new HealthGoodie(goodieX, goodieY, this));
        else if (num <= 9)
            m_actors.push_back(new FlameGoodie(goodieX, goodieY, this));
        else
            m_actors.push_back(new LifeGoodie(goodieX, goodieY, this));
    }

    //Set the status string
    std::ostringstream oss;
    oss.fill('0');
    oss << "Score: " << setw(6) << getScore();
    oss.fill(' ');
    oss << setw(9) << "Level: " << getLevel() << setw(9) << "Lives: " << getLives();
    oss << setw(10) << "Health: " << m_socrates->getHealth();
    oss << setw(10) << "Sprays: " << 20 << setw(10) << "Flames: " << 3;
    string status = oss.str();
    
    setGameStatText(status);

    //Return status that the game is being continued
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //Delete socrates and all the remaining actors in the game
    delete m_socrates;
    
    std::list<Actor*>::iterator it = m_actors.begin();

    while (it != m_actors.end())
    {
        delete *it;
        m_actors.erase(it);
        it++;
    }
}

bool StudentWorld::checkOverlap(double x, double y, int toDamage, bool toEat)
{
    //Check whether any actors overlap with the position x, y and act on those actors depending on the
    //parameters of toDamage and toEat which specify what action to take
    double dist;
    
    std::list<Actor*>::iterator it = m_actors.begin();

    while (it != m_actors.end())
    {
        dist = sqrt(pow(x - (*it)->getX(), 2) + pow(y - (*it)->getY(), 2));


        if (dist < 2 * SPRITE_RADIUS)
        {
            if ((*it)->isDamageable() && toDamage != -1)
            {
                (*it)->receiveDamage(toDamage);
                return true;
            }
            
            else if ((*it)->isEdible() && toEat)
            {
                (*it)->receiveDamage(0);
                return true;
            }

        }
        
        it++;

    }

    return false;
}

bool StudentWorld::checkOverlapSocrates(double x, double y, int damage)
{
    //checks whether the actor at position x, y overlaps with Socrates and applies the specified amount of
    //damage to Socrates
    double dist = sqrt(pow(x - m_socrates->getX(), 2) + pow(y - m_socrates->getY(), 2));

    if (dist < 2 * SPRITE_RADIUS)
    {
        m_socrates->receiveDamage(damage);
        return true;
    }

    return false;
}

bool StudentWorld::checkOverlapBacteria(double x, double y, int angle, int distOfOverlap)
{
    //checks whether a bacteria at position x, y will overlap with an actor in the game that can block
    //bacteria a distance of distOfOverlap away in the direction of angle
    list<Actor*>::iterator it = m_actors.begin();

    int newX = x + (distOfOverlap * cos(angle * (3.14159 / 180)));
    int newY = y + (distOfOverlap * sin(angle * (3.14159 / 180)));

    double radius = sqrt(pow((VIEW_WIDTH / 2) - newX, 2) + pow((VIEW_HEIGHT / 2) - newY, 2));

    if (radius >= VIEW_RADIUS)
        return true;

    while (it != m_actors.end())
    {
        if ((*it)->canBlockBacteria())
        {
            int actX = (*it)->getX();
            int actY = (*it)->getY();

            double dist = sqrt(pow(newX - actX, 2) + pow(newY - actY, 2));

            if (dist <= (SPRITE_RADIUS / 2))
                return true;
        }

        it++;
    }

    return false;

}

bool StudentWorld::findFood(int x, int y, Direction& dir)
{
    //returns whether a Food object can be found within 128 pixels of the position x, y and changes dir to
    //the direction to that Food object if that is true
    list<Actor*>::iterator it = m_actors.begin();

    double dist;

    while (it != m_actors.end())
    {
        if ((*it)->isEdible())
        {
            int actX = (*it)->getX();
            int actY = (*it)->getY();

            dist = sqrt(pow(x - actX, 2) + pow(y - actY, 2));

            if (dist <= 128)
            {
                int dirX = x - m_socrates->getX();
                int dirY = y - m_socrates->getY();
                Direction dir = atan2(dirY, dirX) * (180 / 3.14159);
                dir = (*it)->getDirection();
                
                return true;
            }
        }

        it++;
    }

    return false;
}
//
bool StudentWorld::findSocrates(int x, int y, int& finalX, int& finalY, Direction& dirSoc, int maxDist)
{
    //returns whether Socrates can be found within maxDist pixels of position x, y
    double dist = sqrt(pow(x - m_socrates->getX(), 2) + pow(y - m_socrates->getY(), 2));
    
    //dirSoc is set to the direction needed to move in to move to Socrates
    int dirX = x - m_socrates->getX();
    int dirY = y - m_socrates->getY();
    Direction dir = atan2(dirY, dirX) * (180 / 3.14159);

    if (dirX < 0 && dirY < 0)
        dir += 180;
    
    dirSoc = dir;

    if (dist <= maxDist)
    {
        //finalX and finalY are set to 3 pixels in the direction that Socrates is if the bacteria is not
        //blocked by a dirt pile on this path
        dirSoc = dirSoc + 180;
        if (!checkOverlapBacteria(x, y, dirSoc, 3))
        {
            finalX = x + 3 * cos(dirSoc * (3.14159 / 180));
            finalY = y + 3 * sin(dirSoc * (3.14159 / 180));
        }
        
        return true;
    }

    return false;
}

//Adds Actor a to list containing all actors
void StudentWorld::addActor(Actor* a)
{
    m_actors.push_back(a);
}

//Changes the number of bacteria alive by num
void StudentWorld::changeNumBacteriaAliveBy(int num)
{
    numBacteriaAlive += num;
}

//Decrements number of pits
void StudentWorld::decNumPits()
{
    numPits--;
}

//Increase socrates number of flames
void StudentWorld::increaseSocratesFlame(int num)
{
    m_socrates->changeFlame(num);
}

//Restore socrates health to full
void StudentWorld::restoreSocratesHealth()
{
    m_socrates->changePoints(100);
}

//Call cleanup once again to remove all the remaining actors in the game
StudentWorld::~StudentWorld()
{
    cleanUp();
}

