 
#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"
#include <list>
#include <string>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
  public:
    Actor(int imageID, int x, int y, Direction dir, int depth, StudentWorld* studWorld);
    virtual void doSomething() = 0;
    bool isDead();
    void setAliveStatus(bool set);
    StudentWorld* getWorld();
    virtual bool isDamageable();
    virtual bool isEdible();
    virtual bool canOverlapWithDirtPile();
    virtual bool canBlockBacteria();
    virtual void receiveDamage(int num);
    virtual ~Actor();
  private:
    bool m_alive;
    StudentWorld* m_studWorld;
};

class Socrates : public Actor
{
  public:
    Socrates(StudentWorld* studWorld);
    virtual void doSomething();
    virtual bool isDamageable();
    virtual void receiveDamage(int num);
    void changePoints(int num);
    void changeSpray(int num);
    void changeFlame(int num);
    int getSprays();
    int getFlames();
    int getHealth();
    virtual ~Socrates();
  private:
    int m_dir;
    double positionalAngle;
    int m_numSpray;
    int m_numFlame;
    int m_hp;

};

class DirtPile : public Actor
{
  public:
    DirtPile(int x, int y, StudentWorld* studWorld);
    virtual void doSomething();
    virtual bool isDamageable();
    virtual bool canBlockBacteria();
    virtual bool canOverlapWithDirtPile();
    virtual ~DirtPile();
};



class FlameSprayActor : public Actor
{
  public:
    FlameSprayActor(int imageID, int x, int y, Direction dir, int depth, StudentWorld* studWorld, int maxDist, int dmg);
    virtual void doSomething();
    virtual ~FlameSprayActor();
  private:
    virtual int damage() = 0;
    double m_maxDist;
    double distTraveled;
    int damagePoints;

};

class Spray : public FlameSprayActor
{
  public:
    Spray(int x, int y, Direction dir, StudentWorld* studWorld);
    virtual ~Spray();
  private:
    virtual int damage();
};

class Flame : public FlameSprayActor
{
  public:
    Flame(int x, int y, Direction dir, StudentWorld* studWorld);
    virtual ~Flame();
  private:
    virtual int damage();
};



class Food : public Actor
{
  public:
    Food(int x, int y, StudentWorld* studWorld);
    virtual void doSomething();
    virtual bool isEdible();
    virtual ~Food();

};



class Goodie : public Actor
{
  public:
    Goodie(int imageID, int x, int y, Direction dir, int depth, StudentWorld* studWorld);
    virtual void doSomething();
    virtual ~Goodie();
  private:
    virtual void doBoost() = 0;
    virtual int effectOnSocratesHp();
    virtual void chooseSound();
    double maxLifetime;


};

class HealthGoodie : public Goodie
{
  public:
    HealthGoodie(int x, int y, StudentWorld*);
    virtual ~HealthGoodie();
  private:
    virtual void doBoost();

};

class FlameGoodie : public Goodie
{
  public:
    FlameGoodie(int x, int y, StudentWorld*);
    virtual ~FlameGoodie();
  private:
    virtual void doBoost();

};

class LifeGoodie : public Goodie
{
  public:
    LifeGoodie(int x, int y, StudentWorld*);
    virtual ~LifeGoodie();
  private:
    virtual void doBoost();

};

class Fungus : public Goodie
{
  public:
    Fungus(int x, int y, StudentWorld* studWorld);
    virtual ~Fungus();
  private:
    virtual void doBoost();
    virtual int effectOnSocratesHp();
    virtual void chooseSound();
};

class Pit : public Actor
{
  public:
    Pit(int x, int y, StudentWorld* studWorld);
    virtual void doSomething();
    virtual ~Pit();
  private:
    int chooseBacteria();
    int m_numRegSalm;
    int m_numAggSalm;
    int m_numEcoli;
    bool emittedAllBacteria;
};

class Bacteria : public Actor
{
  public:
    Bacteria(int imageID, int x, int y, Direction dir, int depth, StudentWorld* studWorld, int hp);
    virtual void commonActions(bool doFirstPart, bool doSecondPart);
    virtual void receiveDamage(int num);
    virtual ~Bacteria();
  private:
    virtual int damage() = 0;
    virtual void divide() = 0;
    virtual void chooseSound(int status) = 0;
    int movementPlanDist;
    int m_hp;
    int numFoodEaten;
};
//
class RegularSalmonella : public Bacteria
{
  public:
    RegularSalmonella(int x, int y, StudentWorld* studWorld);
    virtual void doSomething();
    virtual ~RegularSalmonella();
private:
    virtual int damage();
    virtual void divide();
    virtual void chooseSound(int status);
};

class AggressiveSalmonella : public Bacteria
{
  public:
    AggressiveSalmonella(int x, int y, StudentWorld* studWorld);
    virtual void doSomething();
    virtual ~AggressiveSalmonella();
  private:
    virtual int damage();
    virtual void divide();
    virtual void chooseSound(int status);

};
//
class EColi : public Bacteria
{
  public:
    EColi(int x, int y, StudentWorld* studWorld);
    virtual void doSomething();
    virtual ~EColi();
  private:
    virtual int damage();
    virtual void divide();
    virtual void chooseSound(int status);

};

#endif // ACTOR_H_


