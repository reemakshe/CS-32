
#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
#include <list>
#include <string>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//Sets the private variables of Actor and calls the constructor the Actor's parent class GraphObject
Actor::Actor(int imageID, int x, int y, Direction dir, int depth, StudentWorld* studWorld)
: GraphObject(imageID, x, y, dir, depth)
{
    m_alive = true;
    m_studWorld = studWorld;
}

//Returns whether the Actor is dead
bool Actor::isDead()
{
    return !m_alive;
}

//Sets the alive status of the Actor
void Actor::setAliveStatus(bool set)
{
    m_alive = set;
}

//Returns the pointer to the StudentWorld object
StudentWorld* Actor::getWorld()
{
    return m_studWorld;
}

//Returns whether an Actor is damageable (by projectiles)
bool Actor::isDamageable()
{
    return false;
}

//Returns whether an Actor is edible
bool Actor::isEdible()
{
    return false;
}

//Returns whether an Actor can block the movement of a bacterium
bool Actor::canBlockBacteria()
{
    return false;
}

//Returns whether the position of an object can overlap with dirt piles (used when initializing the screen)
bool Actor::canOverlapWithDirtPile()
{
    return false;
}

//The actor will recieve damage in a certain way, usually just becoming dead if they have no health points
void Actor::receiveDamage(int num)
{
    setAliveStatus(false);
}

Actor::~Actor()
{}

//Sets the private variables of Socrates and calls the constructor of its base class Actor
Socrates::Socrates(StudentWorld* studWorld)
: Actor(IID_PLAYER, 0, 128, 0, 0, studWorld)
{
    m_numSpray = 20;
    m_numFlame = 5;
    m_hp = 100;
    //Positional angle around the circumference of the game screen
    positionalAngle = 180;
}

//Changes Socrates health points by num
void Socrates::changePoints(int num)
{
    m_hp = num;
}

//Changes Socrates number of sprays by num
void Socrates::changeSpray(int num)
{
    m_numSpray = num;
}

//Changes Socrates number of flames by num
void Socrates::changeFlame(int num)
{
    m_numFlame = num;
}

//Returns Socrates number of sprays
int Socrates::getSprays()
{
    return m_numSpray;
}

//Returns Socrates number of flames
int Socrates::getFlames()
{
    return m_numFlame;
}

//Returns Socrates health points
int Socrates::getHealth()
{
    return m_hp;
}

void Socrates::doSomething()
{
    //Return immediately if Socrates is dead
    if (isDead())
        return;
    
    //Get the input from the user
    int ch;
    if (getWorld()->getKey(ch))
    {
        double x = getX();
        double y = getY();
        double PI = 3.14159;
        double newAngle;

        switch (ch)
        {
            //If the user pressed the left key, move 5 degrees counterclockwise around the circumference
            case KEY_PRESS_LEFT:
                newAngle = (positionalAngle + 5) * (PI / 180);
                positionalAngle += 5;
                x = VIEW_RADIUS * (cos(newAngle));
                y = VIEW_RADIUS * (sin(newAngle));
                x = (VIEW_WIDTH / 2) + x;
                y = (VIEW_WIDTH / 2) + y;
                moveTo(x, y);
                setDirection(positionalAngle + 185);
                break;
            //If the user pressed the right key, move 5 degrees clockwise around the circumference
            case KEY_PRESS_RIGHT:
                newAngle = (positionalAngle - 5) * (PI / 180);
                positionalAngle -= 5;
                x = VIEW_RADIUS * (cos(newAngle));
                y = VIEW_RADIUS * (sin(newAngle));
                x = (VIEW_HEIGHT / 2) + x;
                y = (VIEW_HEIGHT / 2) + y;
                moveTo(x, y);
                setDirection(positionalAngle + 185);
                break;
            //If the user pressed the space key, generate a spray projectile and reduce the number of sprays
            //Socrates has and play the sound indicating a Spray projectile has been launched
            case KEY_PRESS_SPACE:
                if (m_numSpray >= 1)
                {
                    double sprayX;
                    double sprayY;
                    Direction angle = getDirection();
                    getPositionInThisDirection(angle, SPRITE_RADIUS, sprayX, sprayY);
                    Actor* act = new Spray(sprayX, sprayY, angle, getWorld());
                    getWorld()->addActor(act);
                    m_numSpray--;
                    getWorld()->playSound(SOUND_PLAYER_SPRAY);
                }
                break;
            //If the user pressed the enter key, generate flame projectiles and reduce the number of flames
            //Socrates has and play the sound indicating a Flame projectile has been launched
            case KEY_PRESS_ENTER:
                if (m_numFlame >= 1)
                {
                    for (int i = 0; i < 16; i++)
                       {
                           //The flames will be oriented in a circle around Socrates
                           double flameX;
                           double flameY;
                           Direction angle = getDirection() + (i * 22);
                           getPositionInThisDirection(angle, SPRITE_RADIUS * 2, flameX, flameY);
                           Actor* act = new Flame(flameX, flameY, angle, getWorld());
                           getWorld()->addActor(act);
                       }
                    m_numFlame--;
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                }
                break;
        }

    }

}

//Return true because Socrates is damageable
bool Socrates::isDamageable()
{
    return true;
}


void Socrates::receiveDamage(int num)
{
    //If num does not equal 0, Socrates will reduce his health points by num
    if (num != 0)
    {
        //If the health points go below 0, Socrates is dead
        if (m_hp - num <= 0)
        {
            m_hp = 0;
            setAliveStatus(false);
            getWorld()->playSound(SOUND_PLAYER_DIE);
        }
        else
        {
            m_hp -= num;
            getWorld()->playSound(SOUND_PLAYER_HURT);
        }
    }
}

Socrates::~Socrates()
{}

//Calls the constructor of its parent class Actor
DirtPile::DirtPile(int x, int y, StudentWorld* studWorld)
: Actor(IID_DIRT, x, y, 90, 1, studWorld)
{}

//DirtPile does nothing
void DirtPile::doSomething()
{}

//DirtPile is damageable by projectile
bool DirtPile::isDamageable()
{
    return true;
}

//Dirtpile can block movement of bacteria
bool DirtPile::canBlockBacteria()
{
    return true;
}

//Dirt pile can overlap with other dirt piles
bool DirtPile::canOverlapWithDirtPile()
{
    return true;
}

DirtPile::~DirtPile()
{}

//Sets private variable of FlameSprayActor and calls parent class constructor of Actor
FlameSprayActor::FlameSprayActor(int imageID, int x, int y, Direction dir, int depth, StudentWorld* studWorld, int dist, int dmg)
: Actor(imageID, x, y, dir, depth, studWorld)
{
    m_maxDist = dist;
    damagePoints = dmg;
    distTraveled = 0;
}

void FlameSprayActor::doSomething()
{
    if (isDead())
        return;
    
    //If it overlaps with a damageable actor, it will do damage to that actor through StudentWorld and
    //it will become dead and return immediately
    if (getWorld()->checkOverlap(getX(), getY(), damage(), false))
    {
        setAliveStatus(false);
        return;
    }
    
    //Otherwise it will move in its current direction and die when it reaches its max distance
    double x = getX();
    double y = getY();
    
    getPositionInThisDirection(getDirection(), SPRITE_RADIUS * 2, x, y);
    moveTo(x, y);
    
    distTraveled += SPRITE_RADIUS * 2;
    
    if (distTraveled >= m_maxDist)
        setAliveStatus(false);
    
}

FlameSprayActor::~FlameSprayActor()
{}

//Calls parent constructor
Flame::Flame(int x, int y, Direction dir, StudentWorld* studWorld)
: FlameSprayActor(IID_FLAME, x, y, dir, 1, studWorld, 32, 5)
{}

//Returns the amount of damage it does
int Flame::damage()
{
    return 5;
}

Flame::~Flame()
{}

//Calls parent constructor
Spray::Spray(int x, int y, Direction dir, StudentWorld* studWorld)
: FlameSprayActor(IID_SPRAY, x, y, dir, 1, studWorld, 112, 2)
{}

//Returns the amount of damage it does
int Spray::damage()
{
    return 2;
}

Spray::~Spray()
{}

//Calls parent constructor
Food::Food(int x, int y, StudentWorld* studWorld)
: Actor(IID_FOOD, x, y, 90, 1, studWorld)
{}

//Does nothing
void Food::doSomething()
{}

//Food is edible
bool Food::isEdible()
{
    return true;
}

Food::~Food()
{}

//Calls parent constructor of Actor and sets the maxlifetime
Goodie::Goodie(int imageID, int x, int y, Direction dir, int depth, StudentWorld* studWorld)
: Actor(imageID, x, y, dir, depth, studWorld)
{
    maxLifetime = std::max(rand() % (300 - 10 * studWorld->getLevel()), 50);
}


void Goodie::doSomething()
{
    if (isDead())
        return;

    //Check whether it overlaps with Socrates and apply boost if it does and play appropriate sound
    if (getWorld()->checkOverlapSocrates(getX(), getY(), effectOnSocratesHp()))
    {
        doBoost();
        setAliveStatus(false);
        chooseSound();
        return;
    }

    //Decrement lifetime and if lifetime is 0, actor is dead
    maxLifetime--;

    if (maxLifetime == 0)
        setAliveStatus(false);

}

//Goodies usually have 0 negative impact on Socrates, except for fungus, so parent goodie will return 0
int Goodie::effectOnSocratesHp()
{
    return 0;
}

//Goodies usually need to play the sound of getting a goodie, except for fungus
void Goodie::chooseSound()
{
    getWorld()->playSound(SOUND_GOT_GOODIE);
}

Goodie::~Goodie()
{}

//Call parent constructor
HealthGoodie::HealthGoodie(int x, int y, StudentWorld* studWorld)
: Goodie(IID_RESTORE_HEALTH_GOODIE, x, y, 0, 1, studWorld)
{}

//Boost increases player score by 250 and restores Socrates health to full
void HealthGoodie::doBoost()
{
    getWorld()->increaseScore(250);
    getWorld()->restoreSocratesHealth();
}

HealthGoodie::~HealthGoodie()
{}

//Calls parent constructor
FlameGoodie::FlameGoodie(int x, int y, StudentWorld* studWorld)
: Goodie(IID_FLAME_THROWER_GOODIE, x, y, 0, 1, studWorld)
{}

//Increases player score by 300 and increases Socrates number of flames by 5
void FlameGoodie::doBoost()
{
    getWorld()->increaseScore(300);
    getWorld()->increaseSocratesFlame(5);
}

FlameGoodie::~FlameGoodie()
{}

//Calls parent constructor
LifeGoodie::LifeGoodie(int x, int y, StudentWorld* studWorld)
: Goodie(IID_EXTRA_LIFE_GOODIE, x, y, 0, 1, studWorld)
{}

//Increase player score by 500 and increase player lives
void LifeGoodie::doBoost()
{
    getWorld()->increaseScore(500);
    getWorld()->incLives();
}

LifeGoodie::~LifeGoodie()
{}

//Calls parent constructor
Fungus::Fungus(int x, int y, StudentWorld* studWorld)
: Goodie(IID_FUNGUS, x, y, 0, 1, studWorld)
{}

//Decreases player score by 50
void Fungus::doBoost()
{
    getWorld()->increaseScore(-50);
}

//Has an effect of 20 points of damage on Socrates
int Fungus::effectOnSocratesHp()
{
    return 20;
}

//Plays no sound because when Socrates is damaged it will play a sound that it is damaged
void Fungus::chooseSound()
{}

Fungus::~Fungus()
{}

//Sets private variables and calls Actor constructor
Pit::Pit(int x, int y, StudentWorld* studWorld)
: Actor(IID_PIT, x, y, 0, 1, studWorld)
{
    m_numRegSalm = 5;
    m_numAggSalm = 3;
    m_numEcoli = 2;
    emittedAllBacteria = false;
}

void Pit::doSomething()
{
    //If it has no more bacteria to emit, sets its status to dead and reduce the number of pits in the game
    if (m_numEcoli == 0 && m_numAggSalm == 0 && m_numRegSalm == 0)
    {
        getWorld()->decNumPits();
        setAliveStatus(false);
        return;
    }

    //Based on the chance from these random numbers, the pit will emit a bacteria and reduce its count
    //of that bacteria
    int num = chooseBacteria();
    int chance = randInt(1, 50);

    if (chance == 1)
    {
        switch (num)
        {
            case 1:
                getWorld()->addActor(new RegularSalmonella(getX(), getY(), getWorld()));
                m_numRegSalm--;
                break;
            case 2:
                getWorld()->addActor(new AggressiveSalmonella(getX(), getY(), getWorld()));
                m_numAggSalm--;
                break;
            case 3:
                getWorld()->addActor(new EColi(getX(), getY(), getWorld()));
                m_numEcoli--;
                break;
        }
        
        //Increase the number of bacteria alive in the game and play a sound indicating bacteria birth
        getWorld()->changeNumBacteriaAliveBy(1);
        getWorld()->playSound(SOUND_BACTERIUM_BORN);
    }



}

//Randomly choose a bacteria to emit, if the Pit has any of those bacteria left
int Pit::chooseBacteria()
{
    bool found = false;
    int num;

    for (;;)
    {
        num = randInt(1, 3);

        switch (num)
        {
            case 1:
                if (m_numRegSalm != 0)
                    found = true;
                break;
            case 2:
                if (m_numAggSalm != 0)
                    found = true;
                break;
            case 3:
                if (m_numEcoli != 0)
                    found = true;
                break;
        }

        if (found)
            break;
    }

    return num;
}


Pit::~Pit()
{}

//Sets private variabes and calls parent class constructor
Bacteria::Bacteria(int imageID, int x, int y, Direction dir, int depth, StudentWorld* studWorld, int hp)
: Actor(imageID, x, y, dir, depth, studWorld)
{
    movementPlanDist = 0;
    m_hp = hp;
    numFoodEaten = 0;
}

void Bacteria::receiveDamage(int num)
{
    m_hp = num;
    if (num != 0)
    {
        //if the bacteria becomes dead from the damage, set its status to dead and increase player's score
        //and decrease total number of bacteria alive
        if (m_hp <= 0)
        {
            chooseSound(-1);
            setAliveStatus(false);
            getWorld()->increaseScore(100);
            getWorld()->changeNumBacteriaAliveBy(-1);
            //The bacteria has 50/50 chance of becoming a food
            int chance = randInt(1, 2);
            if (chance == 1)
                getWorld()->addActor(new Food(getX(), getY(), getWorld()));
        }
        else
            chooseSound(1);
    }
    
}

//Common actions done by all bacteria
void Bacteria::commonActions(bool doFirstPart, bool doSecondPart)
{
    if (doFirstPart)
    {
        //Check whether it has eaten 3 food and divide if so
        if (numFoodEaten == 3)
        {
            divide();
            numFoodEaten = 0;
        }

        //Check if overlaps with food and increase its number of food eaten if so
        else if (getWorld()->checkOverlap(getX(), getY(), -1, true))
        {
             numFoodEaten++;
        }
    }

    if (doSecondPart)
    {
        //If it has a movementPlanDist attempt to move in that Distance, otherwise change its direction
        //to a random direction and restore its movement plan
        if (movementPlanDist > 0)
        {
            movementPlanDist--;
            if (getWorld()->checkOverlapBacteria(getX(), getY(), getDirection(), 3))
            {
                setDirection(randInt(0, 359));
                movementPlanDist = 10;
                return;
            }

            else
            {
                double x = getX() + (3 * cos(getDirection() * (3.14159 / 180)));
                double y = getY() + (3 * sin(getDirection() * (3.14159 / 180)));
                moveTo(x, y);
            }
        }

        //otherwise find the nearest food and attempt to move towards it
        else
        {
            Direction foodDir;

            if (getWorld()->findFood(getX(), getY(), foodDir))
            {
                int newX = getX() + (3 * cos(foodDir * (3.14159 / 180)));
                int newY = getY() + (3 * sin(foodDir * (3.14159 / 180)));

                if (!getWorld()->checkOverlapBacteria(getX(), getY(), foodDir, 3))
                {
                    setDirection(foodDir);
                    moveTo(newX, newY);
                    return;
                }
            }
            
            setDirection(randInt(0, 359));
            movementPlanDist = 10;
        }
    }

}
//

Bacteria::~Bacteria()
{}

//Call parent constructor
RegularSalmonella::RegularSalmonella(int x, int y, StudentWorld* studWorld)
: Bacteria(IID_SALMONELLA, x, y, 90, 0, studWorld, 4)
{}

//Does common actions depending on whether it overlaps with Socrates
void RegularSalmonella::doSomething()
{
    if (isDead())
        return;

    bool doesOverlapSocrates = getWorld()->checkOverlapSocrates(getX(), getY(), damage());

    commonActions(!doesOverlapSocrates, true);
}

//To divide, adds a new RegularSalmonella at the specified position
void RegularSalmonella::divide()
{
    int x;
    int y;

    if (getX() < VIEW_WIDTH / 2)
        x = getX() + SPRITE_RADIUS;
    else
        x = getX() - SPRITE_RADIUS;

    if (getY() < VIEW_HEIGHT / 2)
        y = getY() + SPRITE_RADIUS;
    else
        y = getY() - SPRITE_RADIUS;

    getWorld()->addActor(new RegularSalmonella(x, y, getWorld()));
    getWorld()->changeNumBacteriaAliveBy(1);

}

//Does 1 point of damage to Socrates
int RegularSalmonella::damage()
{
    return 1;
}

//Plays sounds corresponding to salmonella
void RegularSalmonella::chooseSound(int status)
{
    if (status == -1)
        getWorld()->playSound(SOUND_SALMONELLA_DIE);
    else
        getWorld()->playSound(SOUND_SALMONELLA_DIE);
}

RegularSalmonella::~RegularSalmonella()
{}

//Calls parent constructor
AggressiveSalmonella::AggressiveSalmonella(int x, int y, StudentWorld* studWorld)
: Bacteria(IID_SALMONELLA, x, y, 90, 0, studWorld, 10)
{}


void AggressiveSalmonella::doSomething()
{
    if (isDead())
        return;

    int x;
    int y;
    bool doSecondPart;
    bool doFirstPart;

    //Attempts to find Socrates and move towards him, which changes what aspects of the common actions it does
    Direction dir;
    if (getWorld()->findSocrates(getX(), getY(), x, y, dir, 72))
    {
        setDirection(dir);
        moveTo(x, y);
        doSecondPart = false;
    }

    else
        doSecondPart = true;
    
    //Check overlap with Socrates which again may change what aspects of the common actions it does
    bool doesOverlapSocrates = getWorld()->checkOverlapSocrates(getX(), getY(), damage());

    if (doesOverlapSocrates)
        doFirstPart = false;
    else
        doFirstPart = true;

    //Do common bacteria actions
    commonActions(doFirstPart, doSecondPart);
    
}

//Does 2 points of damage to Socrates
int AggressiveSalmonella::damage()
{
    return 2;
}

//To divide, adds a new AggressiveSalmonella at the specified position
void AggressiveSalmonella::divide()
{
    int x;
    int y;

    if (getX() < VIEW_WIDTH / 2)
        x = getX() + SPRITE_RADIUS;
    else
        x = getX() - SPRITE_RADIUS;

    if (getY() < VIEW_HEIGHT / 2)
        y = getY() + SPRITE_RADIUS;
    else
        y = getY() - SPRITE_RADIUS;

    getWorld()->addActor(new AggressiveSalmonella(x, y, getWorld()));
    getWorld()->changeNumBacteriaAliveBy(1);
}

//Plays sounds corresponding to salmonella
void AggressiveSalmonella::chooseSound(int status)
{
    if (status == -1)
        getWorld()->playSound(SOUND_SALMONELLA_DIE);
    else
        getWorld()->playSound(SOUND_SALMONELLA_DIE);
}

AggressiveSalmonella::~AggressiveSalmonella()
{}

//Calls parent constructor
EColi::EColi(int x, int y, StudentWorld* studWorld)
: Bacteria(IID_ECOLI, x, y, 90, 0, studWorld, 5)
{}

//Does 4 points of damage to Socrates
int EColi::damage()
{
    return 4;
}

//
void EColi::doSomething()
{
    if (isDead())
        return;

    //Checks whether it overlaps with Socrates which may change what aspects of the common actions it does
    bool doesOverlapSocrates = getWorld()->checkOverlapSocrates(getX(), getY(), damage());
    bool doFirstPart;
    bool doSecondPart = false;

    if (doesOverlapSocrates)
        doFirstPart = false;
    else
        doFirstPart = true;

    //Do specified portions of common bacteria actions
    commonActions(doFirstPart, doSecondPart);
    
    int x;
    int y;
    int count = 0;

    //Attempt to find Socrates and move in his direction
    Direction dir;
    if (getWorld()->findSocrates(getX(), getY(), x, y, dir, 256))
    {
        double movex;
        double movey;
        
        while (count < 10)
        {
            if (getWorld()->checkOverlapBacteria(getX(), getY(), dir, 2))
            {
                dir += 10;
                setDirection(dir);
                count++;
            }
            else
            {
                getPositionInThisDirection(dir, 2, movex, movey);
                moveTo(movex, movey);
                return;
            }
        }
    }
}

//Plays sounds corresponding to EColi
void EColi::chooseSound(int status)
{
    if (status == -1)
        getWorld()->playSound(SOUND_ECOLI_DIE);
    else
        getWorld()->playSound(SOUND_ECOLI_HURT);
}

//To divide, adds a new EColi at the specified position
void EColi::divide()
{
    int x;
    int y;

    if (getX() < VIEW_WIDTH / 2)
        x = getX() + SPRITE_RADIUS;
    else
        x = getX() - SPRITE_RADIUS;

    if (getY() < VIEW_HEIGHT / 2)
        y = getY() + SPRITE_RADIUS;
    else
        y = getY() - SPRITE_RADIUS;

    getWorld()->addActor(new EColi(x, y, getWorld()));
    getWorld()->changeNumBacteriaAliveBy(1);
}

EColi::~EColi()
{}
