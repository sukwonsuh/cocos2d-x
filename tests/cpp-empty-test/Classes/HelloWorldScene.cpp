#include "HelloWorldScene.h"
#include "AppMacros.h"

USING_NS_CC;

void HelloWorld::onAcceleration(Acceleration* acc, Event* unused_event)
{
    mLabel_x += acc->x;
    mLabel_y -= acc->y;

    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto label = dynamic_cast<Label*> (this->getChildByName("label"));
    auto offset = 0;
    if (label)
    {
    	offset = label->getBoundingBox().size.width / 2;
    }

    auto left = -visibleSize.width / 2 + offset;
    auto right = visibleSize.width / 2 - offset;
    auto bottom = -visibleSize.height / 2;
    auto top = visibleSize.height / 2;

    if (mLabel_x > right)
    {
    	mLabel_x = right;
    }
    else if(mLabel_x < left)
    {
    	mLabel_x = left;
    }

    if (mLabel_y > top)
    {
    	mLabel_y = top;
    }
    else if(mLabel_y < bottom)
    {
    	mLabel_y = bottom;
    }

    if (label)
    {
    	label->setPosition(mLabel_x, mLabel_y);
    }

	if (mLabel_x * mLabel_x + mLabel_y * mLabel_y > 200)
	{
		mLabel_x *= 0.99f;
		mLabel_y *= 0.99f;
	}

	SpotLight* light = dynamic_cast<SpotLight*> (this->getChildByName("spot_light"));
	if (light)
	{
		Vec3 pos(-mLabel_x, -mLabel_y, 0.0f);
		pos.normalize();

		auto visibleSize = Director::getInstance()->getVisibleSize();
		pos.x *= visibleSize.width;
		pos.y *= visibleSize.height;
		pos.z = light->getPosition3D().z;

		Vec3 dir(-pos.x, -pos.y, 0);
		dir.normalize();

		light->setPosition3D(pos);
		light->setDirection(dir);

		if (label)
		{
	    	Size offset(dir.x * 2.0f, dir.y * 2.0f);
	    	label->enableShadow(Color4B::BLACK, offset);
		}
	}
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

void HelloWorld::Add3DObject()
{
	{
		auto sprite3d = Sprite3D::create("body.c3b");
		if (sprite3d)
		{
			this->addChild(sprite3d);
			Vec3 pos = sprite3d->getPosition3D();
			auto animation = Animation3D::create("body.c3b", "anim|01");
			if (animation)
			{
				auto animate = Animate3D::create(animation);
				if (animate)
					sprite3d->runAction(RepeatForever::create(animate));
			}
			sprite3d->setName("body");
		}
	}

	{
		auto sprite3d = Sprite3D::create("niddle_second.c3b");
		if (sprite3d)
		{
			this->addChild(sprite3d);
			sprite3d->setName("niddle_second");
		}
	}

	{
		auto sprite3d = Sprite3D::create("niddle_minute.c3b");
		if (sprite3d)
		{
			this->addChild(sprite3d);
			sprite3d->setName("niddle_minute");
		}
	}

	{
		auto sprite3d = Sprite3D::create("niddle_hour.c3b");
		if (sprite3d)
		{
			this->addChild(sprite3d);
			sprite3d->setName("niddle_hour");
		}
	}

	auto inner = CC_DEGREES_TO_RADIANS(5.0f);
	auto outer = CC_DEGREES_TO_RADIANS(20.0f);
	auto spot_light = SpotLight::create(Vec3(0, 0, 1), Vec3(0, 0, 20), Color3B(255, 255, 220), inner, outer, 1000.0f);
	spot_light->setName("spot_light");
	this->addChild(spot_light);

	auto point_light = PointLight::create(Vec3(0, 0, 180), Color3B(255, 255, 255), 1000.0f);
	point_light->setName("point_light");
	this->addChild(point_light);
}

void HelloWorld::AddCamera()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto camera = Camera::createPerspective(90.0f, visibleSize.width / visibleSize.height, 1.0f, 1000.0f);

	Vec3 pos(0.0f, 0.0f, visibleSize.width / 2);
	camera->setPosition3D(pos);
	camera->lookAt(Vec3(0, 0, 0));
	camera->setCameraFlag(CameraFlag::USER1);
	camera->setName("camera");

	this->setCameraMask((unsigned short)CameraFlag::USER1);
	this->addChild(camera);
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

    mLabel_x = 0;
    mLabel_y = 0;

    auto label = Label::createWithSystemFont("HelloWorld", "BreezeSans", 24);
    if (label)
    {
    	label->setName("label");
    	label->enableShadow();
        label->setPosition(mLabel_x, mLabel_y);
        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    this->Add3DObject();
    this->AddCamera();

    return true;
}

void HelloWorld::menuCloseCallback(Ref* sender)
{
    Director::getInstance()->end();
}

