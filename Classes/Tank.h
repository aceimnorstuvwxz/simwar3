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
        ST_FIRE, //失火了
        ST_DEAD, //已被灭
    } STATE;

    typedef enum {
        T_BLUE,
        T_RED
    } TEAM;

    STATE state = ST_ALIVE;
    TEAM team = T_RED;
    std::string name;
    int pos_x = 0;
    int pos_y = 0;
    int move_points = config::init_move_point;//本回合剩余机动点数
    cocos2d::Sprite* sprite;

    int distance(int x, int y){
        return sqrt((pos_x - x)*(pos_x - x) + (pos_y - y)*(pos_y - y));
    }

    bool moveTo(int x, int y){
        assert(x >= 0);
        assert(y >= 0);
        auto dis = distance(x, y);
        if (dis > move_points) {
            CCLOG("too far, move failed.");
            return false;
        }

        pos_x = x;
        pos_y = y;
        move_points -= dis;
        CCLOG("%s move to %d, %d", name.c_str(), pos_x, pos_y);
        return true;
    }

    void nextCycle(){
        // 进入下一个回合，更新相关状态
        move_points = config::init_move_point;
    }

};


#endif /* defined(__simwar__Tank__) */
