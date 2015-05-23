#ifndef __MAINSCENE_SCENE_H__
#define __MAINSCENE_SCENE_H__

#include "cocos2d.h"

#include "ui/CocosGUI.h"
class MainScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
	CREATE_FUNC(MainScene);

private:
    enum {
        TAG_CHECK,
        TAG_MOVE,
        TAG_ATTACK,
        TAG_END
    };

    cocos2d::Layer* _battleLayer;

    cocos2d::Sprite* _smallMap; // 小地图
    cocos2d::Sprite* _mainMap; // 大地图
    cocos2d::Sprite* _select; // 选择框

    cocos2d::Sprite* _lb0; //机动值
    cocos2d::Sprite* _lb1; //是否机动
    cocos2d::Sprite* _lb2; //是否射击
    cocos2d::Sprite* _lb3; //是否被射击
    cocos2d::Sprite* _lb4; //当前状态

    void decorateFunctionButton(const std::string& img0, const std::string& img1,  float x, float y, int tag);
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

};

#endif // __HELLOWORLD_SCENE_H__
