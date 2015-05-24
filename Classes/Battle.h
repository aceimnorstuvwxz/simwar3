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
    void init(cocos2d::Layer* battleLayer, cocos2d::Label* label, cocos2d::Label* label2)
    {
        _layer = battleLayer;
        _labelMessage = label;
        _labelWhoFirst = label2;
    }

    void insertTank(Cord cord, Tank::TEAM team);


    cocos2d::Vec2 cord2pos(Cord cord);
    Cord pos2cord(const cocos2d::Vec2& pos);

    void message(const std::string& message ){
        _labelMessage->setString(message);
    }

    void whoFirst(const std::string& message){
        _labelWhoFirst->setString(message);
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
            whoFirst("红军先手，蓝军后手。");
            isRedFirst = true;
        } else {
            whoFirst("蓝军先手，红军后手。");
            isRedFirst = false;
        }
        switch (gameState) {
            case WAIT_MOVING_CHECK:
                gameState =WAIT_FIRST_MOVING_CLICK;
                message("请先手点击’机动‘后机动。");
                break;
            case WAIT_ATTACK_CHECK:
                gameState = WAIT_FIRST_ATTACK_CLICK;
                message("请先手点击’攻击‘后攻击。");
                break;
            default:
                break;
        }
    }

    void showCord();

    void move(){
        switch (gameState) {
            case WAIT_FIRST_MOVING_CLICK:
                message("请选择坦克后点击目标位置进行机动。");
                gameState = FIRST_MOVING;
                isSelected = false;
                break;
                
            default:
                break;
        }
    }

    void attack(){
//        gameState = ATTACK;
    }

    void end(){
//        gameState = WAIT_CHECK;
    }

    std::shared_ptr<Tank> getTank(Cord cord);

    void onClick(float x, float y);

    void setRedTarget(){
        if (redTargetSp == nullptr) {
            redTargetSp = cocos2d::Sprite::create("red_target.png");
            redTargetSp->setAnchorPoint({0.5f, 0.f});
            redTargetSp->setScale(config::target_scale);
            _layer->addChild(redTargetSp);
        }
        redTargetSp->setPosition(cord2pos(redTarget));
    }
    void setBlueTarget(){
        if (blueTargetSp == nullptr) {
            blueTargetSp = cocos2d::Sprite::create("blue_target.png");
            blueTargetSp->setAnchorPoint({0.5f, 0.f});
            blueTargetSp->setScale(config::target_scale);
            _layer->addChild(blueTargetSp);
        }
        blueTargetSp->setPosition(cord2pos(blueTarget));
    }
private:
    cocos2d::Label* _labelMessage;
    cocos2d::Label* _labelWhoFirst;
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
    Cord redTarget = {0,0};
    Cord blueTarget = {0,0};
    cocos2d::Sprite* redTargetSp = nullptr;
    cocos2d::Sprite* blueTargetSp = nullptr;

    void dealMove(Cord cord){

        if (isSelected) {
            if (getTank(cord) == nullptr) {
                // ok to move
                auto tank = getTank(selectedCord);
                if (pathIsPass(selectedCord, cord)&& pathCost(selectedCord, cord) <= tank->move_points) {
                    tank->cord = cord;
                    tank->move_points -= pathCost(selectedCord, cord);
                    auto move = cocos2d::MoveTo::create(config::tank_move_time,cord2pos(cord));
                    tank->sprite->runAction(move);
                    tank->hasMoved = true;
                    switch (gameState) {
                        case FIRST_MOVING:
                            gameState = FIRST_MOVED;
                            message("后手此时可以发动机会攻击，或者点击’机动‘进行机动。");
                            break;

                        case SECOND_MOVING:
                            gameState = SECOND_MOVED;
                            message("先手此时可以发动机会攻击，或者继续下一步游戏。");
                            break;

                        default:
                            break;
                    }
                }
            } else {
                if (getTank(selectedCord)->team == getTank(cord)->team) {
                    // same team , can not move
                } else {
                    // 要移动到敌人的地方，裁决
                }
            }
            isSelected = false;
        } else {
            if (getTank(cord)!= nullptr && getTank(cord)->hasMoved == false) {
                auto tank = getTank(cord);
                if ((gameState == FIRST_MOVING && isRedFirst && tank->team == Tank::T_RED) ||
                    (gameState == FIRST_MOVING && !isRedFirst && tank->team == Tank::T_BLUE) ||
                    (gameState == SECOND_MOVING && isRedFirst && tank->team == Tank::T_BLUE) ||
                    (gameState == SECOND_MOVING && !isRedFirst && tank->team == Tank::T_RED)){
                    isSelected = true;
                    selectedCord = cord;
                }
            }
        }
    }
};

#endif /* defined(__simwar__Battle__) */
