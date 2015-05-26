//
//  Tank.h
//  simwar
//
//  Created by chenbingfeng on 15/5/23.
//
//

#ifndef __simwar__Tank__
#define __simwar__Tank__

#include <stdio.h>
#include "Common.h"
#include <math.h>
#include <memory>
#include <cocos2d.h>

class Tank
{
public:
    typedef enum {
        ST_ALIVE, //正常模式
        ST_NOFIRE, //失火了
        ST_NOMOVE, //失去动力
        ST_DEAD, //已被灭
    } STATE;

    typedef enum {
        T_BLUE,
        T_RED
    } TEAM;

    STATE state = ST_ALIVE;
    TEAM team = T_RED;
    bool hasMoved = false;
    bool hasFired = false;
    bool hasBeenShotted = false;
    std::string name;
    Cord cord = {0,0};
    int move_points = config::init_move_point;//本回合剩余机动点数
    cocos2d::Sprite* sprite;
    cocos2d::Sprite* smallSprite;

    int distance(int x, int y){
        return sqrt((cord.x - x)*(cord.x - x) + (cord.y - y)*(cord.y - y));
    }

    bool canFire(){
        return hasFired == false && (state == ST_ALIVE || state == ST_NOMOVE);
    }

    bool canMove(){
        return hasMoved == false && (state == ST_ALIVE || state == ST_NOFIRE);
    }

    void nextCycle(){
        // 进入下一个回合，更新相关状态
        move_points = config::init_move_point;
        hasFired = hasMoved = hasBeenShotted = false;
    }


};


#endif /* defined(__simwar__Tank__) */
