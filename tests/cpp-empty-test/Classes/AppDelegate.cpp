#include "AppDelegate.h"

#include <vector>
#include <string>

#include "HelloWorldScene.h"
#include "AppMacros.h"

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Cpp Empty Test");
        director->setOpenGLView(glview);
    }

    director->setOpenGLView(glview);

    // Set the design resolution
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    // a bug in DirectX 11 level9-x on the device prevents ResolutionPolicy::NO_BORDER from working correctly
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
#else
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
#endif

	Size frameSize = glview->getFrameSize();
    
    vector<string> searchPath;

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.

    // if the frame's height is larger than the height of medium resource size, select large resource.
	if (frameSize.height > mediumResource.size.height)
	{
        searchPath.push_back(largeResource.directory);

        director->setContentScaleFactor(MIN(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
	}
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > smallResource.size.height)
    {
        searchPath.push_back(mediumResource.directory);
        
        director->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
	else
    {
        searchPath.push_back(smallResource.directory);

        director->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
    }
    
    // set searching path
    FileUtils::getInstance()->setSearchPaths(searchPath);
	
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 30);

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::scene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void AppDelegate::applicationTimeTick(watch_time_h watch_time, void *data)
{
	const int TEXT_BUF_SIZE = 100;
	char watch_text[TEXT_BUF_SIZE];
	int hour24, minute, second;

	if (watch_time == NULL)
		return;

	watch_time_get_hour24(watch_time, &hour24);
	watch_time_get_minute(watch_time, &minute);
	watch_time_get_second(watch_time, &second);
	snprintf(watch_text, TEXT_BUF_SIZE, "%02d:%02d:%02d", hour24, minute, second);

    auto scene = Director::getInstance()->getRunningScene();
    HelloWorld* helloWorld = dynamic_cast<HelloWorld*> (scene->getChildByName("HelloWorld"));
	if (helloWorld)
	{
	    auto label = dynamic_cast<Label*> (helloWorld->getChildByName("label"));
	    if (label)
	    {
	    	label->setString(watch_text);
	    }

		{
			Sprite3D* sprite3D = dynamic_cast<Sprite3D*> (helloWorld->getChildByName("niddle_second"));
			if (sprite3D)
			{
				auto rot = sprite3D->getRotation3D();
				rot.z = 6.0f * second;
				sprite3D->setRotation3D(rot);
			}
		}

		{
			Sprite3D* sprite3D = dynamic_cast<Sprite3D*> (helloWorld->getChildByName("niddle_minute"));
			if (sprite3D)
			{
				auto rot = sprite3D->getRotation3D();
				rot.z = 6.0f * minute;
				rot.z += 6.0f * (second / 60.0f);
				sprite3D->setRotation3D(rot);
			}
		}

		{
			Sprite3D* sprite3D = dynamic_cast<Sprite3D*> (helloWorld->getChildByName("niddle_hour"));
			if (sprite3D)
			{
				auto rot = sprite3D->getRotation3D();
				rot.z = 30.0f * (hour24 % 12);
				rot.z += 30.0f * (minute / 60.0f);
				rot.z += 0.5f * (second / 60.0f);
				sprite3D->setRotation3D(rot);
			}
		}
	}
}

void AppDelegate::applicationAmbientTick(watch_time_h watch_time, void *data)
{
	const int TEXT_BUF_SIZE = 100;
	char watch_text[TEXT_BUF_SIZE];
	int hour24, minute, second;

	if (watch_time == NULL)
		return;

	watch_time_get_hour24(watch_time, &hour24);
	watch_time_get_minute(watch_time, &minute);
	watch_time_get_second(watch_time, &second);
	snprintf(watch_text, TEXT_BUF_SIZE, "%02d:%02d", hour24, minute);

    auto scene = Director::getInstance()->getRunningScene();
    if (!scene)
    	return;

    HelloWorld* helloWorld = dynamic_cast<HelloWorld*> (scene->getChildByName("HelloWorld"));
	if (helloWorld)
	{
	    auto label = dynamic_cast<Label*> (helloWorld->getChildByName("label"));
	    if (label)
	    {
	    	label->setString(watch_text);
	    }

		{
			Sprite3D* sprite3D = dynamic_cast<Sprite3D*> (helloWorld->getChildByName("niddle_minute"));
			if (sprite3D)
			{
				auto rot = sprite3D->getRotation3D();
				rot.z = 6.0f * minute;
				rot.z += 6.0f * (second / 60.0f);
				sprite3D->setRotation3D(rot);
			}
		}

		{
			Sprite3D* sprite3D = dynamic_cast<Sprite3D*> (helloWorld->getChildByName("niddle_hour"));
			if (sprite3D)
			{
				auto rot = sprite3D->getRotation3D();
				rot.z = 30.0f * (hour24 % 12);
				rot.z += 30.0f * (minute / 60.0f);
				rot.z += 0.5f * (second / 60.0f);
				sprite3D->setRotation3D(rot);
			}
		}
	}
}

void AppDelegate::applicationAmbientChanged(bool ambient_mode, void *data)
{
    auto scene = Director::getInstance()->getRunningScene();
    if (!scene)
    	return;

    HelloWorld* helloWorld = dynamic_cast<HelloWorld*> (scene->getChildByName("HelloWorld"));
    if (!helloWorld)
    	return;

	if(ambient_mode)
	{
	    auto label = dynamic_cast<Label*> (helloWorld->getChildByName("label"));
	    if (label)
	    {
	    	label->disableEffect();
	    }

		{
			Sprite3D* sprite3D = dynamic_cast<Sprite3D*> (helloWorld->getChildByName("body"));
			if (sprite3D)
			{
				sprite3D->setVisible(false);
			}
		}

		{
			Sprite3D* sprite3D = dynamic_cast<Sprite3D*> (helloWorld->getChildByName("niddle_second"));
			if (sprite3D)
			{
				sprite3D->setVisible(false);
			}
		}
	}
	else
	{
	    auto label = dynamic_cast<Label*> (helloWorld->getChildByName("label"));
	    if (label)
	    {
	    	label->enableShadow();
	    }

		{
			Sprite3D* sprite3D = dynamic_cast<Sprite3D*> (helloWorld->getChildByName("body"));
			if (sprite3D)
			{
				sprite3D->setVisible(true);
			}
		}

		{
			Sprite3D* sprite3D = dynamic_cast<Sprite3D*> (helloWorld->getChildByName("niddle_second"));
			if (sprite3D)
			{
				sprite3D->setVisible(true);
			}
		}
	}
}
