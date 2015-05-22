#include "MainScene.h"
#include "Common.h"

USING_NS_CC;

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
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

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(MainScene::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

	// 大地图
	_mainMap = Sprite::create("map.jpg");
    _mainMap->setAnchorPoint({ 0, 0 });
    _mainMap->setPosition({ 0, 1 });
    this->addChild(_mainMap, 0);

    // 设置区域的背景
    auto bg = Sprite::create("bg2.jpg");
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

    // 小地图选择框
    _select = Sprite::create("select.png");
    float scale_x = config::map_space_width * 1.f / _mainMap->getContentSize().width * config::minimap_width / _select->getContentSize().width;
    _select->setScaleX(scale_x);
    float scale_y = config::map_space_height * 1.f / _mainMap->getContentSize().height * config::minimap_height / _select->getContentSize().height;
    _select->setScaleY(scale_y);
    _select->setPosition(config::minimap_x + 0.5f*config::minimap_width, config::minimap_y + 0.5f * config::minimap_height);
    this->addChild(_select, 3);

    // 小地图事件处理
    auto smallMap_listener = EventListenerMouse::create();
    smallMap_listener->onMouseDown = [this](Event * event){
        EventMouse* e = reinterpret_cast<EventMouse*>(event);
        this->_select->setPosition(e->getCursorX(), e->getCursorY());
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _smallMap);


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
