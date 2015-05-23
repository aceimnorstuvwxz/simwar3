//
//  Battle.h
//  simwar
//
//  Created by chenbingfeng on 15/5/23.
//
//

#ifndef __simwar__Battle__
#define __simwar__Battle__

#include <stdio.h>
#include <vector>
#include <memory>
#include <cocos2d.h>
#include <utility>

#include "MainScene.h"
#include "Tank.h"

typedef
struct _cord
{
    int x,y;
    _cord(int xx, int yy):x(xx), y(yy){};
} Cord;

class Battle
{
public:
    std::vector<std::shared_ptr<Tank>> redTeam;
    std::vector<std::shared_ptr<Tank>> blueTeam;

    static Battle* getInstance(){
        return &_instance;
    }
    void init(cocos2d::Layer* battleLayer)
    {
        _layer = battleLayer;
    }

    void insertTank(Cord cord, Tank::TEAM team);


    cocos2d::Vec2 cord2pos(Cord cord);
    Cord pos2cord(const cocos2d::Vec2& pos);
private:
    cocos2d::Layer* _layer;
    Battle(){};
    static Battle _instance;

};

#endif /* defined(__simwar__Battle__) */
