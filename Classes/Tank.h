// 表示一个坦克

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

    STATE lastState = ST_ALIVE; //上个回合的坦克状态
    STATE state = ST_ALIVE; //坦克状态
    TEAM team = T_RED; //坦克队伍
    bool hasMoved = false; //本回合是否动过
    bool hasFired = false;
    bool hasBeenShotted = false;
    std::string name;//无用
    Cord cord = {0,0};//坐标
    int move_points = config::init_move_point;//本回合剩余机动点数
    cocos2d::Sprite* sprite;//大地图上的图片
    cocos2d::Sprite* smallSprite;//小地图上的点

    int distance(int x, int y){
        return sqrt((cord.x - x)*(cord.x - x) + (cord.y - y)*(cord.y - y));
    }

    bool canFire(){
        return hasFired == false && (lastState != ST_DEAD && lastState != ST_NOFIRE && state != ST_DEAD && state != ST_NOFIRE);
    }

    bool canMove(){
        return hasMoved == false && (lastState != ST_DEAD && lastState != ST_NOMOVE && state != ST_DEAD && state != ST_NOMOVE);
    }

    void nextCycle(){
        // 进入下一个回合，更新相关状态
        move_points = config::init_move_point;
        hasFired = hasMoved = hasBeenShotted = false;
        // 状态记录，部分状态需要持续一个回合（NOMOVE, NOFIRE)
        STATE newNowState = state == ST_DEAD ? ST_DEAD : ST_ALIVE;
        lastState = state;
        state = newNowState;
    }

    void checkState(){
        // 状态归一化，如果既NOFIRE又NOMOVE则也是DEAD。
        if (state != ST_DEAD){
            if ((lastState == ST_NOMOVE || state == ST_NOMOVE) && (lastState == ST_NOFIRE || state == ST_NOFIRE)) {
                state = ST_DEAD;
            }
        }
    }
};


#endif /* defined(__simwar__Tank__) */
