//
//  Battle.h
//  simwar
//
//  Created by chenbingfeng on 15/5/23.
//
//

#ifndef __simwar__Battle__
#define __simwar__Battle__

#include <vector>
#include <memory>
#include <cocos2d.h>
#include <utility>
#include <cstdlib>
#include <chrono>

#include "MainScene.h"
#include "Tank.h"
#include "Msg.h"



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
        WAIT_END_LOOP_CLICK,
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
    void init(cocos2d::Layer* battleLayer, cocos2d::Label* label, cocos2d::Label* label2,
              cocos2d::Label* b0, cocos2d::Label* b1,cocos2d::Label* b2,cocos2d::Label* b3,cocos2d::Label* b4, cocos2d::Layer* frontLayer)
    {
        _layer = battleLayer;
        _labelMessage = label;
        _labelWhoFirst = label2;
        auto t = std::chrono::system_clock::now();
        auto a = t.time_since_epoch().count();
        std::srand(a);
        statusX[0] = b0;
        statusX[1] = b1;
        statusX[2] = b2;
        statusX[3] = b3;
        statusX[4] = b4;
        _frontLayer = frontLayer;
    }

    void insertTank(Cord cord, Tank::TEAM team);


    cocos2d::Vec2 cord2pos(Cord cord);
    cocos2d::Vec2 cord2mallPos(Cord cord);
    Cord pos2cord(const cocos2d::Vec2& pos);

    void message(const std::string& message ){
        CCLOG("key=%s", message.c_str());
        _labelMessage->setString(Msg::get()[message]);
    }

    void whoFirst(const std::string& message){
        CCLOG("key=%s", message.c_str());
        _labelWhoFirst->setString(Msg::get()[message]);
    }

    void reset(){
        message("put_red");
        for (auto tank: blueTeam) {
            _layer->removeChild(tank->sprite);
            _frontLayer->removeChild(tank->smallSprite);
        }
        for (auto tank: redTeam) {
            _layer->removeChild(tank->sprite);
            _frontLayer->removeChild(tank->smallSprite);
        }
        redTeam.clear();
        blueTeam.clear();
        gameState = ADDING_RED;
    }
    int rand100(){
        return std::rand()%100;
    }

    void check(){
        if (std::rand()%2 == 0 ){
            whoFirst("red_first");
            isRedFirst = true;
        } else {
            whoFirst("blue_first");
            isRedFirst = false;
        }
        switch (gameState) {
            case WAIT_MOVING_CHECK:
                gameState =WAIT_FIRST_MOVING_CLICK;
                message("click_move");
                break;
            case WAIT_ATTACK_CHECK:
                gameState = WAIT_FIRST_ATTACK_CLICK;
                message("click_attack");
                break;
            case SECOND_MOVED:
                if (isAllMoved()) {
                    gameState = WAIT_FIRST_ATTACK_CLICK;
                    message("to_acctack_first_attack");
                } else {
                    gameState = WAIT_FIRST_MOVING_CLICK;
                    message("click_move");
                }
                break;
            default:
                break;
        }
    }

    bool isAllMoved(){
        for (auto tank : blueTeam) {
            if (tank->canMove()) {
                return false;
            }
        }
        for (auto tank : redTeam) {
            if (tank->canMove()) {
                return false;
            }
        }
        return true;
    }

    inline std::string getStateString(Tank::STATE st){
        switch (st) {
            case Tank::ST_ALIVE:
                return Msg::get()["st_alive"];
            case Tank::ST_DEAD:
                return Msg::get()["st_dead"];
            case Tank::ST_NOFIRE:
                return Msg::get()["st_nofire"];
            case Tank::ST_NOMOVE:
                return Msg::get()["st_nomove"];
            default:
                break;
        }
    }

    void showCord(Cord cord){
        auto tank = getTank(cord);
        if  (tank == nullptr) {
            for (int i = 0; i < 5; i++) {
                statusX[i]->setString("----n/a----");
            }
        } else {
            std::stringstream s;
            s << tank->move_points;
            statusX[0]->setString(s.str());
            statusX[1]->setString(tank->hasMoved? "Yes":"No");
            statusX[2]->setString(tank->hasFired? "Yes":"No");
            statusX[3]->setString(tank->hasBeenShotted? "Yes":"No");
            statusX[4]->setString(getStateString(tank->state));
        }
    }

    void move(){
        switch (gameState) {
            case WAIT_FIRST_MOVING_CLICK:
                message("moveing");
                gameState = FIRST_MOVING;
                isSelected = false;
                break;
            case FIRST_MOVED:
                message("second_moving");
                gameState = SECOND_MOVING;
                isSelected = false;
                break;
                
            default:
                break;
        }
    }

    void attack(){
//        gameState = ATTACK;
        switch (gameState) {
            case WAIT_FIRST_ATTACK_CLICK:
                gameState = FIRST_ATTACKING;
                message("first_attacking");
                isSelected = false;
                break;
            case WAIT_SECOND_ATTACK_CLICK:
                gameState = SECOND_ATTACKING;
                message("second_attacking");
                isSelected = false;
                break;
                
            default:
                break;
        }
    }

    bool isRedAllGone(){
        for (auto tank :redTeam){
            if (tank ->state != Tank::ST_DEAD) return false;
        }
        return true;
    }

    bool isBlueAllGone(){
        for (auto tank :blueTeam){
            if (tank->state != Tank::ST_DEAD) return false;
        }
        return true;
    }

    bool isRedWin(){
        if (isBlueAllGone() ) return true;

        for (auto tank : redTeam)
            if (tank->cord == redTarget) return true;

        return false;
    }

    bool isBlueWin(){
        if (isRedAllGone()) return true;

        for (auto tank: blueTeam)
            if (tank->cord == blueTarget) return true;

        return false;
    }

    void nextCycle(){
        for (auto tank :blueTeam) {
            tank->nextCycle();
        }
        for (auto tank : redTeam) {
            tank->nextCycle();
        }
    }

    void end(){
        if (isRedWin()) message("red_win");

        if (isBlueWin()) message("blue_win");

        nextCycle();
        gameState = WAIT_MOVING_CHECK;
        message("next_cycle");

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
    cocos2d::Layer* _frontLayer;
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
    cocos2d::Label* statusX[5];

    int distance(Cord from ,Cord to){
        int xx = from.x - to.x;
        int yy = from.y - to.y;
        return sqrt(xx*xx + yy*yy);
    }
    float distance2chance(int d){
        if (d <= 10) return 0.9f;
        if (d <= 20) return 0.85f;
        if (d <= 30) return 0.6f;
        if (d <= 40) return 0.4f;
        if (d <= 50) return 0.2f;
        if (d <= 60) return 0.1f;
        if (d <= 70) return 0.1f;
        if (d <= 80) return 0.05f;
        if (d <= 90) return 0.01f;
//        if (d <= 100) return 0.01f;
//        if (d <= 110) return 0.01f;
        return 0.f;
    }

    typedef enum {
        DESTROY,
        CANNOT_SHOT,
        CANNOT_MOVE
    } RES;

    RES resutlGen(int a, int b , int c){
        int r = rand100();
        if (r < a) return DESTROY;
        if (r < a + b ) return CANNOT_SHOT;
        return CANNOT_MOVE;
    }

    RES dist2result(int dist){
        if (dist <= 10) return resutlGen(90,5,5);
        if (dist <= 20) return resutlGen(80,10,5);
        if (dist <= 30) return resutlGen(70,15,5);
        if (dist <= 40) return resutlGen(50,25,5);
        if (dist <= 50) return resutlGen(40,30,5);
        if (dist <= 60) return resutlGen(30,35,5);
        if (dist <= 70) return resutlGen(20,40,5);
        if (dist <= 80) return resutlGen(10,45,5);
        if (dist <= 90) return resutlGen(6,47,5);
        return CANNOT_MOVE;
    }
    void shot(Cord from ,Cord to){
        CCLOG("shot");
        auto src = getTank(from);
        auto dst = getTank(to);
        int dist = distance(from, to);
        float change = distance2chance(dist);
        bool ifSuccess = (rand100() <= change * 100);
        if (ifSuccess) {
            auto result = dist2result(dist);
            dst->hasBeenShotted = true;
            switch (result) {
                case DESTROY:
                    dst->state = Tank::ST_DEAD;
                    break;
                case CANNOT_MOVE:
                    dst->state = Tank::ST_NOMOVE;
                    break;
                case CANNOT_SHOT:
                    dst->state = Tank::ST_NOFIRE;
                    break;
                default:
                    break;
            }
        }

    }

    bool allShotted(){
        for (auto tank : blueTeam){
            if (tank->canFire() && !tank->hasFired) return false;
        }
        for (auto tank : redTeam){
            if (tank->canFire() && !tank->hasFired) return false;
        }
        return true;
    }
    void dealAttack(Cord cord){
        if (isSelected) {
            auto tank = getTank(cord);
            if (tank != nullptr && !tank->hasBeenShotted && tank->team != getTank(selectedCord)->team  && pathIsSee(cord, selectedCord)) {
                getTank(selectedCord)->hasFired = true;
                shot(selectedCord, cord);
                switch (gameState) {
                    case FIRST_ATTACKING:
                        gameState = WAIT_SECOND_ATTACK_CLICK;
                        message("wait_second_attack");
                        break;
                    case SECOND_ATTACKING:
                        if (allShotted()) {
                            gameState = WAIT_END_LOOP_CLICK;
                            message("end_loop");
                        } else {
                            gameState = WAIT_ATTACK_CHECK;
                            message("wait_attack_check");
                        }
                        break;
                    default:
                        break;
                }
            }
            isSelected = false;
        } else {
            auto tank = getTank(cord);
            if (tank != nullptr && tank->canFire()) {
                if ((gameState == FIRST_ATTACKING && isRedFirst && tank->team == Tank::T_RED)||
                    (gameState == FIRST_ATTACKING && !isRedFirst && tank->team == Tank::T_BLUE)||
                    (gameState == SECOND_ATTACKING && isRedFirst && tank->team == Tank::T_BLUE)||
                    (gameState == SECOND_ATTACKING && !isRedFirst && tank->team == Tank::T_RED)
                ){
                    isSelected = true;
                    selectedCord = cord;
                }
            }
        }
    }

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
                    tank->smallSprite->setPosition(cord2mallPos(cord));
                    tank->hasMoved = true;
                    switch (gameState) {
                        case FIRST_MOVING:
                            gameState = FIRST_MOVED;
                            message("optinal_attack");
                            break;

                        case SECOND_MOVING:
                            gameState = SECOND_MOVED;
                            message("optinal_attack2");
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
            auto tank = getTank(cord);
            if (tank!= nullptr && tank->canMove()) {
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
