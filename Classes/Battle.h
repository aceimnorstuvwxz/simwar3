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
#include <cstdlib>
#include <ctime>

#include "MainScene.h"
#include "Tank.h"



class Battle
{
public:
    typedef enum
    {
        INIT,
        ADDING_RED,
        ADDING_BLUE,
        SET_RED_TARGET,
        SET_BLUE_TARGET,
        WAIT_MOVING_CHECK,
        WAIT_FIRST_MOVING_CLICK,
        FIRST_MOVING,
        FIRST_MOVED,
        SECOND_MOVING_ATTACKING,
        WAIT_SECOND_MOVING_CLICK,
        SECOND_MOVING,
        SECOND_MOVED,
        FIRST_MOVING_ATTACKING,
        WAIT_ATTACK_CHECK,
        WAIT_FIRST_ATTACK_CLICK,
        FIRST_ATTACKING,
        WAIT_SECOND_ATTACK_CLICK,
        SECOND_ATTACKING,
        RED_WIN,
        BLUE_WIN
    } GAME_STATE;

    GAME_STATE gameState = INIT;
    std::vector<std::shared_ptr<Tank>> redTeam;
    std::vector<std::shared_ptr<Tank>> blueTeam;
    bool isRedFirst = false;

    static Battle* getInstance(){
        return &_instance;
    }
    void init(cocos2d::Layer* battleLayer, cocos2d::Label* label)
    {
        _layer = battleLayer;
        _labelMessage = label;
    }

    void insertTank(Cord cord, Tank::TEAM team);


    cocos2d::Vec2 cord2pos(Cord cord);
    Cord pos2cord(const cocos2d::Vec2& pos);

    void message(const std::string& message ){
        _labelMessage->setString(message);
    }

    void reset(){
        message("新的战争开始了，请先放置10辆红军坦克。");
        for (auto tank: blueTeam) {
            _layer->removeChild(tank->sprite);
        }
        for (auto tank: redTeam) {
            _layer->removeChild(tank->sprite);
        }
        redTeam.clear();
        blueTeam.clear();
        gameState = ADDING_RED;
    }

    void check(){
        std::time_t t;
        std::ctime(&t);
        CCLOG("%ld", t);
        std::srand(t);
        if (std::rand()%2 == 0 ){
            message("red first");
            isRedFirst = true;
        } else {
            message("blue first");
            isRedFirst = false;
        }
        switch (gameState) {
            case WAIT_MOVING_CHECK:
                gameState =WAIT_FIRST_MOVING_CLICK;
                break;
            case WAIT_ATTACK_CHECK:
                gameState = WAIT_FIRST_ATTACK_CLICK;
                break;
            default:
                break;
        }
    }

    void showCord();

    void move(){
//        gameState = MOVING;
    }

    void attack(){
//        gameState = ATTACK;
    }

    void end(){
//        gameState = WAIT_CHECK;
    }

    std::shared_ptr<Tank> getTank(Cord cord);

    void onClick(float x, float y);
private:
    cocos2d::Label* _labelMessage;
    cocos2d::Layer* _layer;
    Battle(){};
    static Battle _instance;
    int getCube(Cord cord);
    int getCubeCost(Cord cord);
    bool isCubePass(Cord cord);
    bool isCubeSee(Cord cord);
    bool pathIsSee(Cord from ,Cord to);
    bool pathIsPass(Cord from , Cord to);
    int pathCost(Cord from, Cord to);
    bool isSelected = false;
    Cord selectedCord = {0,0};
    bool isAttackSelected = false;
    Cord attackSelectedCord = {0,0};
    Cord redTarget = {0,0};
    Cord blueTarget = {0,0};
};

#endif /* defined(__simwar__Battle__) */
