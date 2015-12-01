#include "HelloWorldScene.h"
#include "AppMacros.h"

USING_NS_CC;

void HelloWorld::onAcceleration(Acceleration* acc, Event* unused_event)
{
    mLabel_x -= acc->y;
    mLabel_y -= acc->x;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto offset = mLabel->getBoundingBox().size.width / 2;
    if (mLabel_x > visibleSize.width + offset)
    {
    	mLabel_x = 0 + offset;
    }
    else if(mLabel_x < 0.0f + offset)
    {
    	mLabel_x = visibleSize.width + offset;
    }

    if (mLabel_y > visibleSize.height)
    {
    	mLabel_y = 0.0f;
    }
    else if(mLabel_y < 0.0f)
    {
    	mLabel_y = visibleSize.height;
    }

    mLabel->setPosition(mLabel_x, mLabel_y);
}

Scene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();
    layer->setName("HelloWorld");

    Device::setAccelerometerEnabled(true);

    layer->_accelerationListener = EventListenerAcceleration::create(CC_CALLBACK_2(Layer::onAcceleration, layer));
    layer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(layer->_accelerationListener, layer);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
   
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", TITLE_FONT_SIZE);
    
    mLabel = label;
    label->setFontSize(50.0);

    // position the label on the center of the screen
    mLabel_x = origin.x + visibleSize.width / 2;
    mLabel_y = origin.y + visibleSize.height - label->getContentSize().height;
    label->setPosition(mLabel_x, mLabel_y);

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize / 2) + origin);

    sprite->setName("bg");

    // add the sprite as a child to this layer
    this->addChild(sprite);
    
    return true;
}

void HelloWorld::menuCloseCallback(Ref* sender)
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

void HelloWorld::setLabelString(char *text)
{
	mLabel->setString(text);
}
