--region *.lua
--�������ӻص�
--���ļ���[BabeLua]����Զ�����
--endregion

ConnectionTips = {}

require("game.comm.PlatformModel")
local scheduler = require("framework.scheduler")
--��Ӽ���
function ConnectionTips.init()
    print("ConnectionTips")
    ConnectionTips.isConnecting = false
    ConnectionTips.isKick = false
    EventManager:addEventListener(GameEvents.EventNetDisconnect, ConnectionTips.DisconnectCallback)
    EventManager:addEventListener(GameEvents.EventNetReconnect, ConnectionTips.ReconnectCallback)

    if device.platform == "ios" then
        ConnectionTips.CheckNetworkReachablityAways()
    end
end

function ConnectionTips.CheckNetworkReachablityAways()
    ConnectionTips.checkReachabilityFunc = scheduler.scheduleGlobal(function(dt)
        local ok, ret = luaoc.callStaticMethod("LuaCallOC", "getNetStatus")
        if ok then
            if ret == 0 then
                --���粻ͨ
                print("ConnectionTips CheckNetworkReachablityAways network disconnect!!!")
                ConnectionTips.DisconnectCallback()
            end
        else
            print("ConnectionTips CheckNetworkReachablityAways error in iOS!!", ok, ret)
        end
    end, 3.0)
end


function ConnectionTips.createTips()
    local wifiTips = display.getRunningScene():getChildByName("WIFITIPS")
    local wifiAction
    if wifiTips then
        if not wifiTips:isVisible() then
            wifiTips:setVisible(true)
            wifiAction = wifiTips:getActionByTag(wifiTips:getTag())
            if wifiAction then
                wifiAction:play("Normal", true)
            end
        end
    else
        -- ��һ�ζ��߽�tips��ӵ���������
        wifiTips = getResManager():getCsbNode(ResConfig.Common.Csb2.wifiTips)
        CommonHelper.layoutNode(wifiTips)
        if wifiTips then
            wifiAction = cc.CSLoader:createTimeline(ResConfig.Common.Csb2.wifiTips)
            wifiTips:runAction(wifiAction)
            wifiAction:play("Normal", true)
            wifiTips:setName("WIFITIPS")
            wifiTips:setGlobalZOrder(6)
	        wifiTips:setLocalZOrder(6)
            display.getRunningScene():addChild(wifiTips)
        end
    end
    return wifiTips
end

--���߻ص�
function ConnectionTips.DisconnectCallback(eventName, noShowTips)
    print("DisconnectCallback========== b")
    if ConnectionTips.isConnecting or ConnectionTips.isKick then
        return
    end
    
    ConnectionTips.isConnecting = true
    ConnectionTips.isTipsShow = false

    if not noShowTips then
        --�ӳ���ʾwifi��ʾ, ��ʱѭ����
        ConnectionTips.tipsFunc = scheduler.scheduleGlobal(function(dt) 
                if not ConnectionTips.isTipsShow then
                    ConnectionTips.createTips()
                    ConnectionTips.isTipsShow = true
                end
	        end, 1.5)
    end

    --��ʱ������������
    ConnectionTips.updateFunc = scheduler.scheduleGlobal(function(dt) 
            print("����session ing")
            reconnectToServer(0)
	    end, 0.5)
   print("DisconnectCallback========== e")
end

--�����ɹ��ص�
function ConnectionTips.ReconnectCallback(eventName, result)
    if 0 == result then
        print("ReconnectCallback reconnect fail==========")
        return
    end
    print("ReconnectCallback reconnect success==========")
    --�����ɹ�, �Ƴ�������ʱ
    scheduler.unscheduleGlobal(ConnectionTips.updateFunc)
    --������֤��Ϣ
    if PlatformModel.loginType == "SDKLogin" or PlatformModel.loginType == "QQHall" then
        print("ReconnectCallback send check with SDKLogin!!!!")
        local BufferData = NetHelper.createBufferData(MainProtocol.Login, LoginProtocol.LoginCheckPfCS) 
        BufferData:writeInt(PlatformModel.pfType)
        BufferData:writeCharArray(PlatformModel.openId, 128)
        BufferData:writeCharArray(PlatformModel.token or "", 256)
        NetHelper.request(BufferData)
    elseif PlatformModel.userId ~= 0 then
        print("PlatformModel.userId", PlatformModel.userId)
        print("ReconnectCallback send check with testCS!!!!")
        -- ��ʱʹ��uid��¼, ��ʹ��openId, token
        local bufferData = NetHelper.createBufferData(MainProtocol.Login, LoginProtocol.LoginCheckTestCS) 
        bufferData:writeInt(tonumber(PlatformModel.userId))  
        NetHelper.request(bufferData)
    else
        --�ο͵�¼
        print("ReconnectCallback send check with guest!!!!")
        local deviceId = cc.UserDefault:getInstance():getStringForKey("DeviceIdentifier")
        local password = cryptoMd5("fanhouzhs" .. deviceId .. "yueliushui10yi")
        local BufferData = NetHelper.createBufferData(MainProtocol.Login, LoginProtocol.LoginCheckGuestCS)
        BufferData:writeCharArray(deviceId, 40)
        BufferData:writeCharArray(password, 32)
        NetHelper.request(BufferData)
    end

    --������֤
    local cmd = NetHelper.makeCommand(MainProtocol.Login, LoginProtocol.LoginCheckSC)
    NetHelper.setResponeHandler(cmd, ConnectionTips.ReconnectCheck)
end

function ConnectionTips.ReconnectCheck(mainCmd, subCmd, buffData)
    print("ReconnectCallback check success==========")
    local userId = buffData:readInt()
    local isNewUser = buffData:readInt()
    local isGuest = buffData:readInt()

    print("UILoginSDK onLoginCheck buffData: ", userId, isNewUser, isGuest)

    if userId < 0 then
        print("====== Reconnect: check error!! userid < 0!!!")
    else
        -- ��֤ͨ��������������Ϣ
        bufferData = NetHelper.createBufferData(MainProtocol.Login, LoginProtocol.LoginReconnectCS) 
        NetHelper.request(bufferData)
        ConnectionTips.ReconnectFinish()
    end
end

function ConnectionTips.ReconnectFinish()
    print("ReconnectCallback finish==========")
    local wifiTips = display.getRunningScene():getChildByName("WIFITIPS")
    if wifiTips then
        wifiTips:setVisible(false)
    end
 
    local cmd = NetHelper.makeCommand(MainProtocol.Login, LoginProtocol.LoginCheckSC)
    NetHelper.removeResponeHandler(cmd, ConnectionTips.ReconnectCheck)

    if ConnectionTips.tipsFunc then
        scheduler.unscheduleGlobal(ConnectionTips.tipsFunc)
    end
    ConnectionTips.isConnecting = false

    -- ֪ͨս���ڽ���ս���������л�
    local bufferData = NetHelper.createBufferData(MainProtocol.Login, LoginProtocol.LoginReconnectCS) 
    raiseEvent(bufferData)

    -- ��ʾ�����ɹ�
    EventManager:raiseEvent(GameEvents.EventNetReconnectFinish)
end
