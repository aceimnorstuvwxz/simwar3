//
//  Battle.cpp
//  simwar
//
//  Created by chenbingfeng on 15/5/23.
//
//

#include "Battle.h"
#include "Map.h"
USING_NS_CC;

Battle Battle::_instance;

void Battle::insertTank(Cord cord, Tank::TEAM team)
{
    auto tank = std::make_shared<Tank>();
    tank->team = team;
    tank->cord = cord;
    tank->nextCycle();
    tank->sprite = cocos2d::Sprite::create(team == Tank::T_RED ? "tank_red.png" : "tank_blue.png" );
    tank->sprite->setScale(config::tanke_scale);
    tank->sprite->setPosition(cord2pos(cord));
    if (team == Tank::T_RED) {
        redTeam.push_back(tank);
    } else {
        blueTeam.push_back(tank);
    }
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

std::shared_ptr<Tank> Battle::getTank(Cord cord)
{
    for (auto tank : blueTeam) {
        if (tank->cord == cord) {
            return tank;
        }
    }
    for (auto tank : redTeam) {
        if (tank->cord == cord) {
            return tank;
        }
    }
    return nullptr;
}

void Battle::onClick(float x, float y)
{
    /*
     INIT,
     ADDING_RED,
     ADDING_BLUE,
     WAIT_CHECK,
     WAIT_MOVE,
     MOVING,
     ATTACK,
     RED_WIN,
     BLUE_WIN
     */
    Cord cord = pos2cord({x,y});
    switch (gameState) {
        case ADDING_RED:
            if (getTank(cord) == nullptr) {
                insertTank(cord, Tank::T_RED);
                if (redTeam.size() >= 10) {
                    gameState = ADDING_BLUE;
                    message("please add blue tanks");
                }
            }
            break;
        case ADDING_BLUE:
            if (getTank(cord) == nullptr) {
                insertTank(cord, Tank::T_BLUE);
                if (blueTeam.size() >= 10) {
                    gameState = WAIT_CHECK;
                    message("wait for check");
                }
            }
            break;
        case MOVING:
            if (isSelected) {
                // move selected tank to cord
                if (getTank(cord) == nullptr) {
                    // ok to move
                    auto tank = getTank(selectedCord);
                    if (pathIsPass(selectedCord, cord) && pathCost(selectedCord, cord) <= tank->move_points) {
                        tank->cord = cord;
                        tank->move_points -= pathCost(selectedCord, cord);
                        tank->sprite->setPosition(cord2pos(cord));
                    }
                } else {
                    // judge 裁决

                }
                isSelected = false;
            } else {
                if (getTank(cord) != nullptr) {
                    isSelected = true;
                    selectedCord = cord;
                }
            }
            break;
        case ATTACK:
            if (isAttackSelected) {

                auto tankFrom = getTank(attackSelectedCord);
                auto tankTo = getTank(cord);
                if (tankTo == nullptr) {
                    // nothing
                } else {
                }
                isAttackSelected = false;
            }

        default:
            break;
    }
}

int Battle::getCube(Cord cord)
{
    int index = cord.x * 80 + cord.y;
    return map[index];
}

int Battle::getCubeCost(Cord cord)
{
    return getCube(cord)%10;
}
bool Battle::isCubePass(Cord cord)
{
    return getCube(cord)/100;
}
bool Battle::isCubeSee(Cord cord)
{
    return (getCube(cord)%100) / 10;
}

bool Battle::pathIsSee(Cord from ,Cord to)
{
    if (from == to) {
        return true;
    }
    Cord nextFrom = from;
    if (abs(from.x - to.x) >= abs(from.y - to.y)) {
        //横向移动
        if (to.x > from.x) {
            // 向右
            nextFrom.x += 1;
        } else {
            nextFrom.x -= 1;
        }
    } else {
        if (to.y > from.y) {
            // 向上
            nextFrom.y += 1;
        } else {
            nextFrom.y -= 1;
        }
    }
    if (!isCubeSee(nextFrom)) {
        return false;
    } else {
        return pathIsSee(nextFrom, to);
    }
}
bool Battle::pathIsPass(Cord from , Cord to)
{
    if (from == to) {
        return true;
    }
    Cord nextFrom = from;
    if (abs(from.x - to.x) >= abs(from.y - to.y)) {
        //横向移动
        if (to.x > from.x) {
            // 向右
            nextFrom.x += 1;
        } else {
            nextFrom.x -= 1;
        }
    } else {
        if (to.y > from.y) {
            // 向上
            nextFrom.y += 1;
        } else {
            nextFrom.y -= 1;
        }
    }
    if (!isCubePass(nextFrom)) {
        return false;
    } else {
        return pathIsPass(nextFrom, to);
    }
}
int Battle::pathCost(Cord from, Cord to)
{
    if (from == to) {
        return 0;
    }
    Cord nextFrom = from;
    if (abs(from.x - to.x) >= abs(from.y - to.y)) {
        //横向移动
        if (to.x > from.x) {
            // 向右
            nextFrom.x += 1;
        } else {
            nextFrom.x -= 1;
        }
    } else {
        if (to.y > from.y) {
            // 向上
            nextFrom.y += 1;
        } else {
            nextFrom.y -= 1;
        }
    }
    return getCubeCost(nextFrom) + pathCost(nextFrom, to);
}