cc.FileUtils:getInstance():addSearchPath("src")
cc.FileUtils:getInstance():addSearchPath("res/")
cc.FileUtils:getInstance():addSearchPath("")

-- CC_USE_DEPRECATED_API = true
require "cocos.init"

-- cclog
cclog = function(...)
    print(string.format(...))
end

-- for CCLuaEngine traceback
function __G__TRACKBACK__(msg)
    cclog("----------------------------------------")
    cclog("LUA ERROR: " .. tostring(msg) .. "\n")
    cclog(debug.traceback())
    cclog("----------------------------------------")
end

local function initGLView()
    local director = cc.Director:getInstance()
    local glView = director:getOpenGLView()
    if nil == glView then
        glView = cc.GLViewImpl:create("Lua Empty Test")
        director:setOpenGLView(glView)
    end

    director:setOpenGLView(glView)

    local visibleSize = cc.Director:getInstance():getVisibleSize()
    print("resolution : ", visibleSize.width, visibleSize.height)
    glView:setDesignResolutionSize(visibleSize.width, visibleSize.height, cc.ResolutionPolicy.NO_BORDER)
--    director:setDisplayStats(true)
    director:setAnimationInterval(1.0 / 60)
end

local function createScene()
	local scene = cc.Scene:create()	
	local layer = cc.Layer:create()
	scene:addChild(layer)               	
	layer:setName("HelloWorld")	
	
	local sprite = cc.Sprite3D:create("earth.c3b")
	layer:addChild(sprite)
	sprite:setName("earth")	

	local sprite = cc.Sprite3D:create("cloud.c3b")
	layer:addChild(sprite)
	sprite:setName("cloud")

		local function AddNumber(name, setName)
			local sprite = cc.Sprite3D:create(name)
			layer:addChild(sprite)
			sprite:setName(setName)				
			sprite:setVisible(false)
		end

	AddNumber("text_modeling_0.c3b", "num_h1_0")
	AddNumber("text_modeling_1.c3b", "num_h1_1")
	AddNumber("text_modeling_2.c3b", "num_h1_2")

	--hours end
	AddNumber("text_modeling_0.c3b", "num_h2_0")
	AddNumber("text_modeling_1.c3b", "num_h2_1")
	AddNumber("text_modeling_2.c3b", "num_h2_2")
	AddNumber("text_modeling_3.c3b", "num_h2_3")
	AddNumber("text_modeling_4.c3b", "num_h2_4")
	AddNumber("text_modeling_5.c3b", "num_h2_5")
	AddNumber("text_modeling_6.c3b", "num_h2_6")
	AddNumber("text_modeling_7.c3b", "num_h2_7")
	AddNumber("text_modeling_8.c3b", "num_h2_8")
	AddNumber("text_modeling_9.c3b", "num_h2_9")

	--hour semi
	AddNumber("text_modeling_semi.c3b", "num_h_semi")

	--minute first
	AddNumber("text_modeling_0.c3b", "num_m1_0")
	AddNumber("text_modeling_1.c3b", "num_m1_1")
	AddNumber("text_modeling_2.c3b", "num_m1_2")
	AddNumber("text_modeling_3.c3b", "num_m1_3")
	AddNumber("text_modeling_4.c3b", "num_m1_4")
	AddNumber("text_modeling_5.c3b", "num_m1_5")

	--minute end
	AddNumber("text_modeling_0.c3b", "num_m2_0")
	AddNumber("text_modeling_1.c3b", "num_m2_1")
	AddNumber("text_modeling_2.c3b", "num_m2_2")
	AddNumber("text_modeling_3.c3b", "num_m2_3")
	AddNumber("text_modeling_4.c3b", "num_m2_4")
	AddNumber("text_modeling_5.c3b", "num_m2_5")
	AddNumber("text_modeling_6.c3b", "num_m2_6")
	AddNumber("text_modeling_7.c3b", "num_m2_7")
	AddNumber("text_modeling_8.c3b", "num_m2_8")
	AddNumber("text_modeling_9.c3b", "num_m2_9")

	--hour semi
	AddNumber("text_modeling_semi.c3b", "num_m_semi");

	--minute first
	AddNumber("text_modeling_0.c3b", "num_s1_0");
	AddNumber("text_modeling_1.c3b", "num_s1_1");
	AddNumber("text_modeling_2.c3b", "num_s1_2");
	AddNumber("text_modeling_3.c3b", "num_s1_3")
	AddNumber("text_modeling_4.c3b", "num_s1_4")
	AddNumber("text_modeling_5.c3b", "num_s1_5")

	--minute end
	AddNumber("text_modeling_0.c3b", "num_s2_0")
	AddNumber("text_modeling_1.c3b", "num_s2_1")
	AddNumber("text_modeling_2.c3b", "num_s2_2")
	AddNumber("text_modeling_3.c3b", "num_s2_3")
	AddNumber("text_modeling_4.c3b", "num_s2_4")
	AddNumber("text_modeling_5.c3b", "num_s2_5")
	AddNumber("text_modeling_6.c3b", "num_s2_6")
	AddNumber("text_modeling_7.c3b", "num_s2_7")
	AddNumber("text_modeling_8.c3b", "num_s2_8")
	AddNumber("text_modeling_9.c3b", "num_s2_9")

	
	local point_light = cc.PointLight:create(cc.vec3(0.0, 0.0, 180.0), cc.c3b(255.0, 255.0, 255.0), 1000.0)
	point_light:setName("point_light")
	layer:addChild(point_light)
	
	local visibleSize = cc.Director:getInstance():getVisibleSize()
	local camera = cc.Camera:createPerspective(80.0, visibleSize.width / visibleSize.height, 1.0, 1000.0)
	camera:setName("camera")	
	camera:setPosition3D(cc.vec3(0.0, 0.0, 150.0))
	camera:lookAt(cc.vec3(0.0, 0.0, 0.0))
	
	camera:setCameraFlag(cc.CameraFlag.USER1)
	layer:setCameraMask(cc.CameraFlag.USER1)	
	layer:addChild(camera)
	
	cc.Device:setAccelerometerEnabled(true)	
	
	require "events"
	local listernerGyro  = cc.EventListenerAcceleration:create(onGyro)
	local listenerTimeTick = cc.EventListenerCustom:create("time_tick", OnTimeTick)
	local listenerAmbientTick = cc.EventListenerCustom:create("ambient_tick", OnAmbientTick)
	local listenerAmbientChanged = cc.EventListenerCustom:create("ambient_changed", OnAmbientChanged)	

	local touchListener = cc.EventListenerTouchOneByOne:create()
	touchListener:registerScriptHandler(onTouchBegan, cc.Handler.EVENT_TOUCH_BEGAN )
	
	local eventDispatcher = layer:getEventDispatcher()	
	eventDispatcher:addEventListenerWithSceneGraphPriority(listernerGyro, layer);
	eventDispatcher:addEventListenerWithSceneGraphPriority(listenerTimeTick, layer)
	eventDispatcher:addEventListenerWithSceneGraphPriority(listenerAmbientTick, layer)
	eventDispatcher:addEventListenerWithSceneGraphPriority(listenerAmbientChanged, layer)
	eventDispatcher:addEventListenerWithSceneGraphPriority(touchListener, layer)
	
	return scene
end

local function main()
    collectgarbage("setpause", 100)
    collectgarbage("setstepmul", 5000)

    initGLView()
    local scene = createScene()
    cc.Director:getInstance():runWithScene(scene)
end

xpcall(main, __G__TRACKBACK__)
