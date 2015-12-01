#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init() override;  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    // a selector callback
    void menuCloseCallback(Ref* sender);
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

    void setLabelString(char *text);

    void onAcceleration(cocos2d::Acceleration* acc, cocos2d::Event* unused_event);

    cocos2d::EventListenerAcceleration* _accelerationListener;

    cocos2d::LabelTTF * mLabel;

    float mLabel_x;
    float mLabel_y;
};

#endif // __HELLOWORLD_SCENE_H__
