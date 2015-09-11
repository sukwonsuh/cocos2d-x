/****************************************************************************
 Copyright (c) 2014 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "platform/CCPlatformConfig.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_TIZEN

#include "CCApplication-tizen.h"
#include <unistd.h>
#include <sys/time.h>
#include <string>

#include <app.h>
#include <dlog.h>
#include <Elementary.h>
#include <Elementary_GL_Helpers.h>
#include <efl_extension.h>
#include <Evas_GL.h>

#include "CCGLViewImpl-tizen.h"
#include "base/CCDirector.h"
#include "base/CCEventKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "platform/CCFileUtils.h"

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "cocos2d-x"

ELEMENTARY_GLVIEW_GLOBAL_DEFINE();

NS_CC_BEGIN

void stopAccelerometerSensor();
void pauseAccelerometerSensor();
void resumeAccelerometerSensor();

static void win_back_cb(void *data, Evas_Object *obj, void *event_info) {
    //Application *ad = (Application *)data;
    /* Let window go to hidden state. */
    //elm_win_lower(ad->_win);
    cocos2d::EventKeyboard event(cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE, false);
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

static void win_more_cb(void *data, Evas_Object *obj, void *event_info) {
    cocos2d::EventKeyboard event(cocos2d::EventKeyboard::KeyCode::KEY_MENU, false);
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

static void makeCurrent(void)
{
	Application* app = Application::getInstance();
	evas_gl_make_current(app->_evasGL, app->_sfc, app->_ctx);
}

static void draw_gl(Evas_Object *obj)
{
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    director->mainLoop();
    glview->pollEvents();
}

static void del_gl(Evas_Object *obj) {

}

// sharedApplication pointer
Application* Application::__instance = nullptr;

Application::Application()
: _win(nullptr)
, _conform(nullptr)
, _animationInterval(1.0f/60.0f*1000.0f)
{
    _orientation = APP_DEVICE_ORIENTATION_0;
    CC_ASSERT(! __instance);
    __instance = this;
}

Application::~Application()
{
    CC_ASSERT(this == __instance);
    __instance = nullptr;
}

static void del_anim(void *data, Evas *evas, Evas_Object *obj, void *event_info)
{
    Ecore_Animator *ani = (Ecore_Animator *)evas_object_data_get(obj, "ani");
    ecore_animator_del(ani);
}

static Eina_Bool anim(void *data) {
    elm_glview_changed_set((Evas_Object*)data);
    return EINA_TRUE;
}

static void init_gl(Evas_Object *obj) {
    Application *ad = Application::getInstance();

    //save current ctx
    ad->_evasGL = elm_glview_evas_gl_get(obj);
    ad->_ctx = evas_gl_current_context_get(ad->_evasGL);
    ad->_sfc = evas_gl_current_surface_get(ad->_evasGL);

    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (glview == nullptr) {
        glview = GLViewImpl::create("Cocos2dxTizen");

        int w, h;
        elm_glview_size_get(obj, &w, &h);
        glview->setFrameSize(w, h);
        director->setOpenGLView(glview);
    }

    ad->initGLContextAttrs();
    // Initialize instance and cocos2d.
    if (! ad->applicationDidFinishLaunching())
    {
        return;
    }
}

static void create_indicator(Application *ad) {
    elm_win_conformant_set(ad->_win, EINA_TRUE);

    elm_win_indicator_mode_set(ad->_win, ELM_WIN_INDICATOR_HIDE);

    ad->_conform = elm_conformant_add(ad->_win);
    evas_object_size_hint_weight_set(ad->_conform, EVAS_HINT_EXPAND,
            EVAS_HINT_EXPAND);
    elm_win_resize_object_add(ad->_win, ad->_conform);
    evas_object_show(ad->_conform);
}

static Evas_Object* add_win(const char *name) {
    Evas_Object *win;

    win = elm_win_util_standard_add(name, "tizen");

    if (!win)
        return nullptr;
    //fix openURL tests not goes to another program issue
   // elm_win_fullscreen_set(win, EINA_TRUE);
    evas_object_show(win);

    return win;
}

//touch call back
static void touches_down_cb(void *data, Evas *e , Evas_Object *obj , void *event_info)
{
    auto ev = (Evas_Event_Multi_Down *)event_info;
    int id = ev->device;
    float x = ev->canvas.x;
    float y = ev->canvas.y;

    makeCurrent();
    cocos2d::Director::getInstance()->getOpenGLView()->handleTouchesBegin(1, &id, &x, &y);
}

static void touches_move_cb(void *data, Evas *e , Evas_Object *obj , void *event_info)
{
    auto ev = (Evas_Event_Multi_Move *)event_info;
    int id = ev->device;
    float x = ev->cur.canvas.x;
    float y = ev->cur.canvas.y;

    makeCurrent();
    cocos2d::Director::getInstance()->getOpenGLView()->handleTouchesMove(1, &id, &x, &y);
}

static void touches_up_cb(void *data, Evas *e , Evas_Object *obj , void *event_info)
{
    auto ev = (Evas_Event_Multi_Up *)event_info;
    int id = ev->device;
    float x = ev->canvas.x;
    float y = ev->canvas.y;

    makeCurrent();
    cocos2d::Director::getInstance()->getOpenGLView()->handleTouchesEnd(1, &id, &x, &y);
}

static void touch_down_cb(void *data, Evas *e , Evas_Object *obj , void *event_info)
{
    auto ev = (Evas_Event_Mouse_Down *)event_info;
    int id = ev->button;
    float x = ev->canvas.x;
    float y = ev->canvas.y;

    makeCurrent();
    cocos2d::Director::getInstance()->getOpenGLView()->handleTouchesBegin(1, &id, &x, &y);
}

static void touch_move_cb(void *data, Evas *e , Evas_Object *obj , void *event_info)
{
    auto ev = (Evas_Event_Mouse_Move *)event_info;
    int id = ev->buttons;
    float x = ev->cur.canvas.x;
    float y = ev->cur.canvas.y;

    makeCurrent();
    cocos2d::Director::getInstance()->getOpenGLView()->handleTouchesMove(1, &id, &x, &y);
}

static void touch_up_cb(void *data, Evas *e , Evas_Object *obj , void *event_info)
{
    auto ev = (Evas_Event_Mouse_Up *)event_info;
    int id = ev->button;
    float x = ev->canvas.x;
    float y = ev->canvas.y;

    makeCurrent();
    cocos2d::Director::getInstance()->getOpenGLView()->handleTouchesEnd(1, &id, &x, &y);
}

static Elm_GLView_Mode get_glview_mode(const GLContextAttrs &attrs)
{
	/* for performance */
	Elm_GLView_Mode mode = ELM_GLVIEW_DIRECT;

	/* alpha */
	if (attrs.alphaBits > 0)
	{
		mode = (Elm_GLView_Mode)(mode | ELM_GLVIEW_ALPHA);
	}

	/* depth */
	if (attrs.depthBits > 24)
	{
		mode = (Elm_GLView_Mode)(mode | ELM_GLVIEW_DEPTH_32);
	}
	else if (attrs.depthBits > 16)
	{
		mode = (Elm_GLView_Mode)(mode | ELM_GLVIEW_DEPTH_24);
	}
	else if (attrs.depthBits > 8)
	{
		mode = (Elm_GLView_Mode)(mode | ELM_GLVIEW_DEPTH_16);
	}
	else if (attrs.depthBits > 0)
	{
		mode = (Elm_GLView_Mode)(mode | ELM_GLVIEW_DEPTH_8);
	}

	/* stencil */
	if (attrs.stencilBits > 8)
	{
		mode = (Elm_GLView_Mode)(mode | ELM_GLVIEW_STENCIL_16);
	}
	else if (attrs.stencilBits > 4)
	{
		mode = (Elm_GLView_Mode)(mode | ELM_GLVIEW_STENCIL_8);
	}
	else if (attrs.stencilBits > 2)
	{
		mode = (Elm_GLView_Mode)(mode | ELM_GLVIEW_STENCIL_4);
	}
	else if (attrs.stencilBits > 1)
	{
		mode = (Elm_GLView_Mode)(mode | ELM_GLVIEW_STENCIL_2);
	}
	else if (attrs.stencilBits > 0)
	{
		mode = (Elm_GLView_Mode)(mode | ELM_GLVIEW_STENCIL_1);
	}

	return mode;
}

static bool app_create(void *data) {
    /* Hook to take necessary actions before main event loop starts
     * Initialize UI resources and application's data
     * If this function returns true, the main loop of application starts
     * If this function returns false, the application is terminated. */

    Evas_Object *bx, *gl;
    Ecore_Animator *ani;
    Application *ad = (Application *)data;

    if (!data)
        return false;

    /* Create and initialize GLView */
    elm_config_accel_preference_set("opengl");
    /* Create the window */
    ad->_win = add_win("cocos2d-x");

    if (!ad->_win)
        return false;

    int rots[2];
    rots[0] = ad->_orientation;
    rots[1] = rots[0] + 180 % 360;
    elm_win_wm_rotation_available_rotations_set(ad->_win, rots, 2);

    eext_object_event_callback_add(ad->_win, EEXT_CALLBACK_BACK, win_back_cb, ad);
    eext_object_event_callback_add(ad->_win, EEXT_CALLBACK_MORE, win_more_cb, ad);

    /* Add a box to contain our GLView */
    bx = elm_box_add(ad->_win);
    evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(ad->_win, bx);
    evas_object_show(bx);

    gl = elm_glview_add(ad->_win);
    ELEMENTARY_GLVIEW_GLOBAL_USE(gl);
    evas_object_size_hint_align_set(gl, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(gl, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

    /* Create and initialize GLView */
    ad->initGLContextAttrs();
    auto attrs = GLView::getGLContextAttrs();
    auto mode = get_glview_mode(attrs);
    elm_glview_mode_set(gl, mode);

    /* The resize policy tells GLView what to do with the surface when it
     * resizes. ELM_GLVIEW_RESIZE_POLICY_RECREATE will tell it to
     * destroy the current surface and recreate it to the new size.
     */
    //elm_glview_resize_policy_set(gl, ELM_GLVIEW_RESIZE_POLICY_RECREATE);

    /* The render policy sets how GLView should render GL code.
     * ELM_GLVIEW_RENDER_POLICY_ON_DEMAND will have the GL callback
     * called only when the object is visible.
     * ELM_GLVIEW_RENDER_POLICY_ALWAYS would cause the callback to be
     * called even if the object were hidden.
     */
    elm_glview_render_policy_set(gl, ELM_GLVIEW_RENDER_POLICY_ON_DEMAND);

    /* The initialize callback function gets registered here */
    elm_glview_init_func_set(gl, init_gl);

    /* The delete callback function gets registered here */
    elm_glview_del_func_set(gl, del_gl);

    /* The resize callback function gets registered here */
    // Cocos2d-x doesn't support to change orientation from portrait to landscape.
    // So comment next line.
//  elm_glview_resize_func_set(gl, resize_gl);

    /* The render callback function gets registered here */
    elm_glview_render_func_set(gl, draw_gl);

    /* Add the GLView to the box and show it */
    elm_box_pack_end(bx, gl);
    evas_object_show(gl);

    elm_object_focus_set(gl, EINA_TRUE);

    /* This adds an animator so that the app will regularly
     * trigger updates of the GLView using elm_glview_changed_set().
     *
     * NOTE: If you delete GL, this animator will keep running trying to access
     * GL so this animator needs to be deleted with ecore_animator_del().
     */
    ecore_animator_source_set(ECORE_ANIMATOR_SOURCE_TIMER);
    ani = ecore_animator_add(anim, gl);
    evas_object_data_set(gl, "ani", ani);
    //evas_object_data_set(gl, "ad", ad);
    evas_object_event_callback_add(gl, EVAS_CALLBACK_DEL, del_anim, gl);

    /* Add Mouse Event Callbacks */
    evas_object_event_callback_add(gl, EVAS_CALLBACK_MOUSE_DOWN, touch_down_cb, ad);
    evas_object_event_callback_add(gl, EVAS_CALLBACK_MOUSE_MOVE, touch_move_cb, ad);
    evas_object_event_callback_add(gl, EVAS_CALLBACK_MOUSE_UP, touch_up_cb, ad);

    evas_object_event_callback_add(gl, EVAS_CALLBACK_MULTI_DOWN, touches_down_cb, ad);
    evas_object_event_callback_add(gl, EVAS_CALLBACK_MULTI_MOVE, touches_move_cb, ad);
    evas_object_event_callback_add(gl, EVAS_CALLBACK_MULTI_UP, touches_up_cb, ad);

    evas_object_show(ad->_win);

    create_indicator(ad);

    return true;
}

static void app_pause(void *data)
{
    /* Take necessary actions when application becomes invisible. */
    if(!data)
    {
        return;
    }
    pauseAccelerometerSensor();

    Application* app = ((Application *)data);
    app->applicationDidEnterBackground();
}

static void app_resume(void *data)
{
    /* Take necessary actions when application becomes visible. */
    if(!data)
    {
        return;
    }

    Application* app = ((Application *)data);
    app->applicationWillEnterForeground();
    resumeAccelerometerSensor();
}

static void app_terminate(void *data)
{
    /* Release all resources. */
    if(!data)
    {
        return;
    }
    stopAccelerometerSensor();
    Application* app = ((Application *)data);
    delete app;
}

static void app_control(app_control_h app_control, void *data)
{
    /* Handle the launch request. */
}

int Application::run(int argc, char *argv[])
{
    app_event_callback_s event_callback = { nullptr, };

    event_callback.create = app_create;
    event_callback.terminate = app_terminate;
    event_callback.pause = app_pause;
    event_callback.resume = app_resume;
    event_callback.app_control = app_control;

    //setenv("EVAS_GL_WIN_PREROTATION", "1", 1); //fixme sukwon.suh

    int ret = app_main(0, nullptr, &event_callback, this);
    if (ret != APP_ERROR_NONE) {
        dlog_print(DLOG_ERROR, LOG_TAG, "The application failed to start, and returned %d", ret);
    }

    return ret;
}

void Application::setAnimationInterval(double interval)
{
    _animationInterval = interval*1000.0f;
    ecore_animator_frametime_set(interval);
}

void Application::setResourceRootPath(const std::string& rootResDir)
{
    _resourceRootPath = rootResDir;
    if (_resourceRootPath[_resourceRootPath.length() - 1] != '/')
    {
        _resourceRootPath += '/';
    }
    FileUtils* pFileUtils = FileUtils::getInstance();
    std::vector<std::string> searchPaths = pFileUtils->getSearchPaths();
    searchPaths.insert(searchPaths.begin(), _resourceRootPath);
    pFileUtils->setSearchPaths(searchPaths);
}

const std::string& Application::getResourceRootPath(void)
{
    return _resourceRootPath;
}

Application::Platform Application::getTargetPlatform()
{
    return Platform::OS_TIZEN;
}

void Application::setDeviceOrientation(int orientation)
{
    _orientation = orientation;
}

bool Application::openURL(const std::string &url)
{
	bool flag = false;
	if(0==url.length())
	{
		return flag;
	}
	app_control_h app_control;

	app_control_create(&app_control);
	app_control_set_operation(app_control, APP_CONTROL_OPERATION_VIEW);
	app_control_set_uri(app_control, url.c_str());

	int ctrlError;
	if ((ctrlError=app_control_send_launch_request(app_control, NULL, NULL)) == APP_CONTROL_ERROR_NONE)
	{
		flag = true;
	}
	else
	{
	    dlog_print(DLOG_ERROR, LOG_TAG, "open url failed, and returned %d", ctrlError);
	}

	app_control_destroy(app_control);
	return flag;
}

//////////////////////////////////////////////////////////////////////////
// static member function
//////////////////////////////////////////////////////////////////////////
Application* Application::getInstance()
{
    CC_ASSERT(__instance);
    return __instance;
}

// @deprecated Use getInstance() instead
Application* Application::sharedApplication()
{
    return Application::getInstance();
}

const char * Application::getCurrentLanguageCode()
{
    static char code[3]={0};
    char *pLanguageName = getenv("LANG");
    if (!pLanguageName)
        return "en";
    strtok(pLanguageName, "_");
    if (!pLanguageName)
        return "en";
    strncpy(code,pLanguageName,2);
    code[2]='\0';
    return code;
}

LanguageType Application::getCurrentLanguage()
{
    char *pLanguageName = getenv("LANG");
    LanguageType ret = LanguageType::ENGLISH;
    if (!pLanguageName)
    {
        return LanguageType::ENGLISH;
    }
    strtok(pLanguageName, "_");
    if (!pLanguageName)
    {
        return LanguageType::ENGLISH;
    }
    
    if (0 == strcmp("zh", pLanguageName))
    {
        ret = LanguageType::CHINESE;
    }
    else if (0 == strcmp("en", pLanguageName))
    {
        ret = LanguageType::ENGLISH;
    }
    else if (0 == strcmp("fr", pLanguageName))
    {
        ret = LanguageType::FRENCH;
    }
    else if (0 == strcmp("it", pLanguageName))
    {
        ret = LanguageType::ITALIAN;
    }
    else if (0 == strcmp("de", pLanguageName))
    {
        ret = LanguageType::GERMAN;
    }
    else if (0 == strcmp("es", pLanguageName))
    {
        ret = LanguageType::SPANISH;
    }
    else if (0 == strcmp("nl", pLanguageName))
    {
        ret = LanguageType::DUTCH;
    }
    else if (0 == strcmp("ru", pLanguageName))
    {
        ret = LanguageType::RUSSIAN;
    }
    else if (0 == strcmp("ko", pLanguageName))
    {
        ret = LanguageType::KOREAN;
    }
    else if (0 == strcmp("ja", pLanguageName))
    {
        ret = LanguageType::JAPANESE;
    }
    else if (0 == strcmp("hu", pLanguageName))
    {
        ret = LanguageType::HUNGARIAN;
    }
    else if (0 == strcmp("pt", pLanguageName))
    {
        ret = LanguageType::PORTUGUESE;
    }
    else if (0 == strcmp("ar", pLanguageName))
    {
        ret = LanguageType::ARABIC;
    }
    else if (0 == strcmp("nb", pLanguageName))
    {
        ret = LanguageType::NORWEGIAN;
    }
    else if (0 == strcmp("pl", pLanguageName))
    {
        ret = LanguageType::POLISH;
    }
    
    return ret;
}

NS_CC_END

#endif // CC_TARGET_PLATFORM == CC_PLATFORM_TIZEN

