#ifndef __simwar_battle__
#define __simwar_battle__

#include <vector>
#include <memory>
#include <cocos2d.h>
#include <utility>
#include <cstdlib>
#include <chrono>

#include "MainScene.h"
#include "Tank.h"
#include "Msg.h"
#include "SimpleAudioEngine.h"


class Battle
{
public:
    typedef enum
    {
        INIT, // 刚打开程序时
        ADDING_RED,//正在添加红军
        ADDING_BLUE, //正在添加蓝军
        SET_RED_TARGET,//正在设置红军目标
        SET_BLUE_TARGET,//正在设置蓝军目标
        WAIT_MOVING_CHECK,//等待机动的先后手判定
        WAIT_FIRST_MOVING_CLICK,//等待先手按机动按钮
        FIRST_MOVING,//先手移动中
        FIRST_MOVED,//先手已经移动
        SECOND_MOVING_ATTACKING,//无用
        WAIT_SECOND_MOVING_CLICK,//等待后手按机动按钮
        SECOND_MOVING,//后手移动中
        SECOND_MOVED,//后手移动完
        FIRST_MOVING_ATTACKING,//无用
        WAIT_ATTACK_CHECK,//等待攻击的先后手判定
        WAIT_FIRST_ATTACK_CLICK,//等待先手按攻击
        FIRST_ATTACKING,//先手攻击中
        WAIT_SECOND_ATTACK_CLICK,//等待后手按攻击
        SECOND_ATTACKING,//后手攻击中
        WAIT_END_LOOP_CLICK,//等待结束回合按钮
        RED_WIN,//红军胜
        BLUE_WIN//蓝军胜
	} GAME_STATE;/* 游戏状态定义 */

    GAME_STATE gameState = INIT;//游戏状态记录
    std::vector<std::shared_ptr<Tank>> redTeam;//红军队列
    std::vector<std::shared_ptr<Tank>> blueTeam;//蓝军队列
    bool isRedFirst = false;//是否红军先手

    static Battle* getInstance(){//单例的获取函数
        return &_instance;
    }
	//初始化battle
    void init(cocos2d::Layer* battleLayer, cocos2d::Label* label, cocos2d::Label* label2,
              cocos2d::Label* b0, cocos2d::Label* b1,cocos2d::Label* b2,cocos2d::Label* b3,cocos2d::Label* b4, cocos2d::Layer* frontLayer)
    {
		//存储一些内容
        _layer = battleLayer;
        _labelMessage = label;
        _labelWhoFirst = label2;
		statusX[0] = b0;
		statusX[1] = b1;
		statusX[2] = b2;
		statusX[3] = b3;
		statusX[4] = b4;
		_frontLayer = frontLayer;

		//设置随机种子
        auto t = std::chrono::system_clock::now();
        auto a = t.time_since_epoch().count();
        std::srand(a);
    }

	//插入一辆坦克
    void insertTank(Cord cord, Tank::TEAM team);


	//从地图坐标到地图的像素坐标
    cocos2d::Vec2 cord2pos(Cord cord);
	//从地图坐标到小地图的像素坐标
    cocos2d::Vec2 cord2mallPos(Cord cord);
	//从像素坐标到地图坐标
    Cord pos2cord(const cocos2d::Vec2& pos);

	//显示某条消息
    void message(const std::string& message ){
        CCLOG("key=%s", message.c_str());
        _labelMessage->setString(Msg::get()[message]);
    }

	//显示先后手
    void whoFirst(const std::string& message){
        CCLOG("key=%s", message.c_str());
        _labelWhoFirst->setString(Msg::get()[message]);
    }

	//开局
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
	//随机100,用来进行攻击结果判定
    int rand100(){
        return std::rand()%100;
    }

	//先后手裁决
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

	//是否所有坦克都已经移动了.
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

	//获取状态的字符串
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

	//显示某个坐标的信息
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

	//机动按钮
    void move(){
        switch (gameState) {
            case WAIT_FIRST_MOVING_CLICK:
                message("moveing");
                gameState = FIRST_MOVING;
                isSelected = false;
                canTeamMove();
                break;
            case FIRST_MOVED:
                message("second_moving");
                gameState = SECOND_MOVING;
                isSelected = false;
                canTeamMove();
                break;
                
            default:
                break;
        }
    }

    bool canTeamMoveAst(bool isRed){
        auto& team = isRed ? redTeam : blueTeam;
        for (auto& tank : team){
            if (tank->canMove() ) return true;
        }
        return false;
    }

    //检查某方是否还有可移动单位
    void canTeamMove(){
        if (gameState == FIRST_MOVING) {
            if (!canTeamMoveAst(isRedFirst)){
                gameState = FIRST_MOVED;
                message("optinal_attack");
            }
        }
        if (gameState == SECOND_MOVING) {
            if (!canTeamMoveAst(!isRedFirst)){
                if (isAllMoved()) {
                    gameState = WAIT_FIRST_ATTACK_CLICK;
                    message("to_acctack_first_attack");
                } else {
                    gameState = WAIT_FIRST_MOVING_CLICK;
                    message("click_move");
                }
            }
        }
    }

    bool camTeamAttackAst(bool isred){
        auto& team = isred? redTeam : blueTeam;
        for (auto& tank : team){
            if (tank->canFire()) return true;
        }
        return false;
    }

    void canTeamAttack(){
        if (gameState == FIRST_ATTACKING) {
            if (!camTeamAttackAst(isRedFirst)){
                gameState = WAIT_SECOND_ATTACK_CLICK;
                message("wait_second_attack");
            }
        }
        if (gameState == SECOND_ATTACKING){
            if (!camTeamAttackAst(!isRedFirst)){
                if (allShotted()) {
                    gameState = WAIT_END_LOOP_CLICK;
                    message("end_loop");
                } else {
                    gameState = WAIT_ATTACK_CHECK;
                    message("wait_attack_check");
                }
            }
        }
    }

	//攻击
    void attack(){
        switch (gameState) {
            case WAIT_FIRST_ATTACK_CLICK:
                gameState = FIRST_ATTACKING;
                message("first_attacking");
                isSelected = false;
                canTeamAttack();
                break;
            case WAIT_SECOND_ATTACK_CLICK:
                gameState = SECOND_ATTACKING;
                message("second_attacking");
                isSelected = false;
                canTeamAttack();
                break;
                
            default:
                break;
        }
    }

	//是否所有红军都灭了
    bool isRedAllGone(){
        for (auto tank :redTeam){
            if (tank ->state != Tank::ST_DEAD) return false;
        }
        return true;
    }

	//是否所有蓝军都灭了
    bool isBlueAllGone(){
        for (auto tank :blueTeam){
            if (tank->state != Tank::ST_DEAD) return false;
        }
        return true;
    }

	//是否红军胜
    bool isRedWin(){
        if (isBlueAllGone() ) return true;

        for (auto tank : redTeam)
            if (tank->cord == redTarget) return true;

        return false;
    }

	//是否蓝军胜
    bool isBlueWin(){
        if (isRedAllGone()) return true;

        for (auto tank: blueTeam)
            if (tank->cord == blueTarget) return true;

        return false;
    }

	//下一个回合
    void nextCycle(){
        for (auto tank :blueTeam) {
            tank->nextCycle();
        }
        for (auto tank : redTeam) {
            tank->nextCycle();
        }
    }

	//结束回合按钮
    void end(){
        if (isRedWin()) message("red_win");

        if (isBlueWin()) message("blue_win");

        nextCycle();
        gameState = WAIT_MOVING_CHECK;
        message("next_cycle");

    }

	//获取某个坐标的坦克,如果该位置没坦克则返回nullptr
    std::shared_ptr<Tank> getTank(Cord cord);

	//在地图上点击的响应.
    void onClick(float x, float y);

	//设置红军目标
    void setRedTarget(){
        if (redTargetSp == nullptr) {
            redTargetSp = cocos2d::Sprite::create("red_target.png");
            redTargetSp->setAnchorPoint({0.5f, 0.f});
            redTargetSp->setScale(config::target_scale);
            _layer->addChild(redTargetSp);
        }
        redTargetSp->setPosition(cord2pos(redTarget));
    }
	//设置蓝军目标
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
    cocos2d::Label* _labelMessage;//上方的消息label
    cocos2d::Label* _labelWhoFirst;//先后手的label
    cocos2d::Layer* _layer;//大地图的layer(大地图和上面的坦克\目标,都在此layer上)
    cocos2d::Layer* _frontLayer;//前面UI的layer(各种按钮及小地图 和各label均在此layer上)
    Battle(){};
    static Battle _instance;
    int getCube(Cord cord);//获取某个地图方格的属性
    int getCubeCost(Cord cord);//获取某个方格的机动小号
    bool isCubePass(Cord cord);//获取某个方格是否可以通过
    bool isCubeSee(Cord cord);//获取某个方格是否透视
    bool pathIsSee(Cord from ,Cord to);//线路是否透视
    bool pathIsPass(Cord from , Cord to);//线路是否通过
    int pathCost(Cord from, Cord to);//计算线路的机动总小号
    bool isSelected = false;
    Cord selectedCord = {0,0};
    Cord redTarget = {0,0};
    Cord blueTarget = {0,0};
    cocos2d::Sprite* redTargetSp = nullptr;
    cocos2d::Sprite* blueTargetSp = nullptr;
    cocos2d::Label* statusX[5];

	//计算直线距离
    int distance(Cord from ,Cord to){
        int xx = from.x - to.x;
        int yy = from.y - to.y;
        return sqrt(xx*xx + yy*yy);
    }
	//距离与攻击命中概率换算
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

	//攻击结果类型
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

	//计算某个距离攻击的结果
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

	//从某坦克到某坦克的射击
    void shot(Cord from ,Cord to){
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/tank_shot.mp3");
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
            dst->checkState();
        }

    }

	//是否全部射击完了
    bool allShotted(){
        for (auto tank : blueTeam){
            if (tank->canFire() && !tank->hasFired) return false;
        }
        for (auto tank : redTeam){
            if (tank->canFire() && !tank->hasFired) return false;
        }
        return true;
    }
	//处理攻击点击
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
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/tank_select.mp3");
                }
            }
        }
    }


	//处理机动的点击
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
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/tank_move.mp3");
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
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/tank_beenshot.mp3");
                    if (rand100() > 50){
                        // 敌人胜。
                        auto tank = getTank(selectedCord);
                        tank->state = Tank::ST_DEAD;
                        tank->cord = {0,0};
                        tank->sprite->setPosition(cord2pos(tank->cord));
                        tank->smallSprite->setPosition(cord2mallPos(tank->cord));
                    } else {
                        auto tank0 = getTank(cord);
                        tank0->state = Tank::ST_DEAD;

                        tank0->cord = {0,0};
                        tank0->sprite->setPosition(cord2pos(tank0->cord));
                        tank0->smallSprite->setPosition(cord2mallPos(tank0->cord));

                        auto tank2 = getTank(selectedCord);
                        tank2->cord = cord;
                        tank2->sprite->setPosition(cord2pos(tank2->cord));
                        tank2->smallSprite->setPosition(cord2mallPos(tank2->cord));
                    }
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
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/tank_select.mp3");
                }
            }
        }
    }
};
#endif