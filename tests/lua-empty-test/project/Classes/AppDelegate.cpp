#include "cocos2d.h"
#include "AppDelegate.h"
#include "audio/include/SimpleAudioEngine.h"
#include "base/CCScriptSupport.h"
#include "CCLuaEngine.h"
#include "lua_module_register.h"
#include "LuaBasicConversions.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{
    // fixed me
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
}

AppDelegate::~AppDelegate()
{
    // end simple audio engine here, or it may crashed on win32
    SimpleAudioEngine::getInstance()->end();
    //CCScriptEngineManager::destroyInstance();
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    
    GLView::setGLContextAttrs(glContextAttrs);
}

NS_CC_BEGIN
class Watch:public Node
{
public:
    static int get_hour24(EventCustom* event)
    {
		int value = 0;
    	if (event)
    	{
			auto watch_time = static_cast<watch_time_h> (event->getUserData());
			watch_time_get_hour24(watch_time, &value);
    	}

    	return value;
    }

    static int get_minute(EventCustom* event)
    {
		int value = 0;
    	if (event)
    	{
			auto watch_time = static_cast<watch_time_h> (event->getUserData());
			watch_time_get_minute(watch_time, &value);
    	}

    	return value;
    }

    static int get_second(EventCustom* event)
    {
		int value = 0;
    	if (event)
    	{
			auto watch_time = static_cast<watch_time_h> (event->getUserData());
			watch_time_get_second(watch_time, &value);
    	}

    	return value;
    }
};
NS_CC_END

int lua_cocos2dx_watch_get_hour24(lua_State* L)
{
    int argc = 0;
    bool ok  = true;
    argc = lua_gettop(L) - 1;
    if (argc == 1)
    {
        cocos2d::EventCustom* arg0 = nullptr;
        ok &= luaval_to_object<cocos2d::EventCustom>(L, 2, "cc.EventCustom", &arg0);
        if(!ok)
        {
            return 0;
        }
        int ret = cocos2d::Watch::get_hour24(arg0);
        tolua_pushnumber(L,(lua_Number)ret);
        return 1;
    }
    return 0;
}

int lua_cocos2dx_watch_get_minute(lua_State* L)
{
    int argc = 0;
    bool ok  = true;
    argc = lua_gettop(L) - 1;
    if (argc == 1)
    {
        cocos2d::EventCustom* arg0 = nullptr;
        ok &= luaval_to_object<cocos2d::EventCustom>(L, 2, "cc.EventCustom", &arg0);
        if(!ok)
        {
            return 0;
        }
        int ret = cocos2d::Watch::get_minute(arg0);
        tolua_pushnumber(L,(lua_Number)ret);
        return 1;
    }
    return 0;
}

int lua_cocos2dx_watch_get_second(lua_State* L)
{
    int argc = 0;
    bool ok  = true;
    argc = lua_gettop(L) - 1;
    if (argc == 1)
    {
        cocos2d::EventCustom* arg0 = nullptr;
        ok &= luaval_to_object<cocos2d::EventCustom>(L, 2, "cc.EventCustom", &arg0);
        if(!ok)
        {
            return 0;
        }
        int ret = cocos2d::Watch::get_second(arg0);
        tolua_pushnumber(L,(lua_Number)ret);
        return 1;
    }
    return 0;
}

int lua_module_register_watch(lua_State* L)
{
    tolua_usertype(L,"cc.Watch");
    tolua_cclass(L,"Watch","cc.Watch","cc.Node",nullptr);

    tolua_beginmodule(L,"Watch");
    tolua_function(L,"get_hour24", lua_cocos2dx_watch_get_hour24);
    tolua_function(L,"get_minute", lua_cocos2dx_watch_get_minute);
    tolua_function(L,"get_second", lua_cocos2dx_watch_get_second);
    tolua_endmodule(L);

    std::string typeName = typeid(cocos2d::Watch).name();
    g_luaType[typeName] = "cc.Watch";
    g_typeCast["Watch"] = "cc.Watch";
    return 1;
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // register lua engine
    LuaEngine* engine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(engine);
    lua_State* L = engine->getLuaStack()->getLuaState();

    tolua_open(L);
    tolua_module(L, "cc", 0);
    tolua_beginmodule(L, "cc");
    lua_module_register_watch(L);
    tolua_endmodule(L);

    lua_module_register(L);


    //The call was commented because it will lead to ZeroBrane Studio can't find correct context when debugging
    //engine->executeScriptFile("src/hello.lua");
    engine->executeString("require 'src/hello.lua'");

    return true;
}

static bool need_reset = false;

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    Director::getInstance()->end();
    ScriptEngineManager::destroyInstance();
    need_reset = true;
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	if (need_reset)
	{
		need_reset = false;
		this->applicationDidFinishLaunching();
	}

    Director::getInstance()->startAnimation();
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
