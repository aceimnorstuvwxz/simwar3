#include "MainScene.h"
#include "Common.h"
#include "Battle.h"
#include "Msg.h"
USING_NS_CC;

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();

    scene->addChild(layer->_battleLayer);
    scene->addChild(layer);

    // return the scene
    return scene;
}

void MainScene::decorateFunctionButton(const std::string& img0, const std::string& img1,  float x, float y, int tag)
{
    auto button = ui::Button::create(img0, img1);
    button->setPressedActionEnabled(true);
    button->setAnchorPoint({0,0});
    button->setPosition({x,y});
    button->setTag(tag);
    button->addTouchEventListener(CC_CALLBACK_2(MainScene::touchEvent, this));
    button->setScale(config::btn_scale);
    this->addChild(button, 11);
}

void MainScene::touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type){
    auto button = static_cast<cocos2d::ui::Button*>(pSender);
    int tag = button->getTag();
    if (type != cocos2d::ui::Widget::TouchEventType::ENDED) {
        return;
    }
    switch (tag) {
        case TAG_CHECK:
            CCLOG("btn check");
            Battle::getInstance()->check();
            break;
        case TAG_MOVE:
            CCLOG("btn move");
            Battle::getInstance()->move();
            break;
        case TAG_ATTACK:
            CCLOG("btn attack");
            Battle::getInstance()->attack();
            break;
        case TAG_END:
            CCLOG("btn end");
            Battle::getInstance()->end();
            break;
        case TAG_RESET:
            Battle::getInstance()->reset();
            CCLOG("reset");
            break;
        default:
            break;
    }
}


// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 消息
    auto label = Label::createWithTTF(Msg::get()["open"], "fonts/myfont.ttf", 24);
    label->setColor({0,1,0});
    // position the label on the center of the screen
    label->setPosition({config::map_space_width / 2, config::window_height - 50});

    // add the label as a child to this layer
    this->addChild(label, 100);

    auto label2 = Label::createWithTTF("", "fonts/myfont.ttf", 20);
    label2->setAnchorPoint({0,0});
    label2->setPosition({config::btn_x + 100, config::btn_y_0});
    label2->setColor({0,0,0});
    this->addChild(label2, 100);


    // 状态区
    auto status = Sprite::create("status.png");
    status->setPosition({config::status_x, config::status_y});
    status->setScale(config::status_scale);
    this->addChild(status, 10);

    auto l0 = Sprite::create("l0.png");
    l0->setAnchorPoint({0,0});
    l0->setScale(config::status_scale);
    l0->setPosition({config::status_l_x, config::status_y_0, });
    this->addChild(l0, 10);

    auto makeBx = [&](float x, float y){
        auto b0 = Label::createWithTTF("", "fonts/myfont.ttf", 20);
        b0->setAnchorPoint({0,0});
        b0->setPosition({x, y-5});
        b0->setColor({0,0,0});
        this->addChild(b0, 100);
        return b0;
    };

    auto b0 = makeBx(config::status_l_x2, config::status_y_0);
    auto b1 = makeBx(config::status_l_x2, config::status_y_1);
    auto b2 = makeBx(config::status_l_x2, config::status_y_2);
    auto b3 = makeBx(config::status_l_x2, config::status_y_3);
    auto b4 = makeBx(config::status_l_x2, config::status_y_4);

    auto l1 = Sprite::create("l1.png");
    l1->setAnchorPoint({0,0});
    l1->setScale(config::status_scale);
    l1->setPosition({config::status_l_x, config::status_y_1, });
    this->addChild(l1, 10);

    auto l2 = Sprite::create("l2.png");
    l2->setAnchorPoint({0,0});
    l2->setScale(config::status_scale);
    l2->setPosition({config::status_l_x, config::status_y_2, });
    this->addChild(l2, 10);

    auto l3 = Sprite::create("l3.png");
    l3->setAnchorPoint({0,0});
    l3->setScale(config::status_scale);
    l3->setPosition({config::status_l_x, config::status_y_3, });
    this->addChild(l3, 10);

    auto l4 = Sprite::create("l4.png");
    l4->setAnchorPoint({0,0});
    l4->setScale(config::status_scale);
    l4->setPosition({config::status_l_x, config::status_y_4, });
    this->addChild(l4, 10);

    // 功能区
    decorateFunctionButton("btn_check.png", "btn_check_p.png", config::btn_x, config::btn_y_0, TAG_CHECK);
    decorateFunctionButton("btn_move.png", "btn_move_p.png", config::btn_x, config::btn_y_1, TAG_MOVE);
    decorateFunctionButton("btn_attack.png", "btn_attack_p.png", config::btn_x, config::btn_y_2, TAG_ATTACK);
    decorateFunctionButton("btn_end.png", "btn_end_p.png", config::btn_x, config::btn_y_3, TAG_END);
    decorateFunctionButton("btn_reset.png", "btn_reset_p.png", config::btn_x, config::btn_y_m1, TAG_RESET);


    _battleLayer = Layer::create();
    auto battle = Battle::getInstance();
    battle->init(_battleLayer, label, label2, b0, b1, b2, b3, b4, this);

	// 大地图
	_mainMap = Sprite::create("map.jpg");
    _mainMap->setAnchorPoint({0,0});
    _mainMap->setPosition({0,0});
    _battleLayer->addChild(_mainMap, 0);

    _battleLayer->setPosition(Vec2{ config::map_space_width *0.5f, config::map_space_height*0.5f } - 0.5f * _mainMap->getContentSize());

    // 小背景
    auto bg = Sprite::create("setting_bg.png");
    bg->setAnchorPoint({0,0}); 
    bg->setScaleX(config::setting_area_w / bg->getContentSize().width);
    bg->setScaleY(config::setting_area_h / bg->getContentSize().height);
    bg->setPosition(config::setting_area_x, config::setting_area_y);
    this->addChild(bg, 1);

	// 小地图
	_smallMap = Sprite::create("map.jpg");
	_smallMap->setAnchorPoint({ 0, 0 });
    _smallMap->setScaleX(config::minimap_width / _smallMap->getContentSize().width);
    _smallMap->setScaleY(config::minimap_height / _smallMap->getContentSize().height);
	_smallMap->setPosition({ config::minimap_x, config::minimap_y });
	this->addChild(_smallMap, 2);

    // 小地图内的选择框
    _select = Sprite::create("select.png");
    float scale_x = config::map_space_width * 1.f / _mainMap->getContentSize().width * config::minimap_width / _select->getContentSize().width;
    _select->setScaleX(scale_x);
    float scale_y = config::map_space_height * 1.f / _mainMap->getContentSize().height * config::minimap_height / _select->getContentSize().height;
    _select->setScaleY(scale_y);
    _select->setPosition(config::minimap_x + 0.5f*config::minimap_width, config::minimap_y + 0.5f * config::minimap_height);
    this->addChild(_select, 3);

    // 选择框移动响应
    auto smallMap_listener = EventListenerMouse::create();
    smallMap_listener->onMouseDown = [this](Event * event){
        EventMouse* e = reinterpret_cast<EventMouse*>(event);
        float x = e->getCursorX();
        float y = config::window_height + e->getCursorY();
        CCLOG("smallMap_listener %f %f", x, y);
        if (x >= config::minimap_x && x <= config::minimap_x + config::minimap_width &&
            y >= 0 && y <= config::minimap_y + config::minimap_height) {

            // 移动选择框
            float rx = x - config::minimap_x;
            float ry = y - config::minimap_y;
            CCLOG("smallMap_listener relative %f %f", rx, ry);

            float half_w = this->_select->getScaleX() * this->_select->getContentSize().width * 0.5f;
            float half_h = this->_select->getScaleY() * this->_select->getContentSize().height * 0.5f;
            if (rx < half_w) rx = half_w;
            if (config::minimap_width - rx  < half_w) rx = config::minimap_width - half_w;
            if (ry < half_h) ry = half_h;
            if (config::minimap_height - ry < half_h) ry = config::minimap_height - half_h;

            this->_select->setPosition(config::minimap_x + rx, config::minimap_y + ry);

            // 移动大地图
            auto& csize = this->_mainMap->getContentSize();
            auto move = MoveTo::create(config::bigmap_move_time,
                                       Vec2{
                                           0.5f*csize.width - rx/ config::minimap_width * csize.width + 0.5f * config::map_space_width,
                                           0.5f*csize.height - ry/ config::minimap_height * csize.height + 0.5f * config::map_space_height,
                                       } - 0.5f * _mainMap->getContentSize());
            _battleLayer->stopAllActions();
            _battleLayer->runAction(move);
        }

        if (x >= 0 && x <= config::map_space_width && y >=0 && y <=config::map_space_height) {
            // 大地图上的点击
            auto& pos = this->_battleLayer->getPosition();
            Battle::getInstance()->onClick(x - pos.x,y-pos.y);
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(smallMap_listener, _smallMap);

    return true;
}


void MainScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
