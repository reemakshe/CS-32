//
//  Game.h
//  cs32Project1
//
//  Created by Reema Kshetramade on 1/8/20.
//  Copyright © 2020 Reema Kshetramade. All rights reserved.
//

#ifndef Game_h
#define Game_h

#include <string>

class Arena;

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nVampires);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    std::string takePlayerTurn();
};

bool decodeDirection(char ch, int& dir);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
int computeDanger(const Arena& a, int r, int c);


#endif /* Game_h */

