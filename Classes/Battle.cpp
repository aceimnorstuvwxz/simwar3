//
//  Battle.cpp
//  simwar
//
//  Created by chenbingfeng on 15/5/23.
//
//

#include "Battle.h"
USING_NS_CC;

Battle Battle::_instance;

void Battle::insertTank(Cord cord, Tank::TEAM team)
{
    auto tank = std::make_shared<Tank>();
    tank->team = team;
    tank->sprite = cocos2d::Sprite::create(team == Tank::T_RED ? "tank_red.png" : "tank_blue.png" );
    tank->sprite->setScale(config::tanke_scale);
    tank->sprite->setPosition(cord2pos(cord));

    _layer->addChild(tank->sprite, 5);
}

Vec2 Battle::cord2pos(Cord cord)
{
    float x = config::cube_x + cord.x * config::cube_x * 1.5;
    float y = cord.x%2 == 0 ? (1 + 2*cord.y):(2 + 2*cord.y);
    y = config::cube_y * y;
    return {x, y};
}

Cord Battle::pos2cord(const cocos2d::Vec2& pos)
{
    int x = (pos.x - 0.25f * config::cube_x) / (1.5f * config::cube_x);
    int y = 0;
    if (x % 2 == 0) {
        y = pos.y / (2 * config::cube_y);
    } else {
        y = (pos.y - config::cube_y) / (2*config::cube_y);
    }
    return {x, y};
}