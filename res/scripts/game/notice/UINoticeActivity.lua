
--[[
/*******************************************************************
** ������:	¬��
** ��  ��:  2016-08-31 18:03
** ��  ��:	1.0
** ��  ��:  ��������
** Ӧ  ��:
********************************************************************/
--]]

local UINoticeActivity = class("UINoticeActivity", function()
    return require("common.UIView").new()
end)

-- ���캯��
function UINoticeActivity:ctor()

end

-- �����汻����ʱ�ص�
-- ֻ��ʼ��һ��
function UINoticeActivity:init(...)
    self.rootPath = ResConfig.UINoticeActivity.Csb2.main
	self.root = getResManager():getCsbNode(self.rootPath)
	self:addChild(self.root)

    -- UI�ı�
    getChild(self.root, "MainPanel/NoticePanel/TitleText"):setString(CommonHelper.getUIString(1642))
    getChild(self.root, "MainPanel/NoticePanel/TipText"):setString(CommonHelper.getUIString(1713))

    -- �رհ�ť
    local btnClose = getChild(self.root, "MainPanel/NoticePanel/CloseButton")
    CsbTools.initButton(btnClose, handler(self, self.onClick))

    -- ���濪����ѡ��
    local checkBox = getChild(self.root, "MainPanel/NoticePanel/CheckBox_1")
    checkBox:addEventListener(handler(self, self.onCheckBox))
    checkBox:setSelected(false)
end

-- �����汻��ʱ�ص�
-- ÿ�ε���Openʱ�ص�
function UINoticeActivity:onOpen(openerUIID, callback)
    local targetPlatform = cc.Application:getInstance():getTargetPlatform()
    if cc.PLATFORM_OS_IPHONE == targetPlatform or cc.PLATFORM_OS_ANDROID == targetPlatform then
        local scrollview = getChild(self.root, "MainPanel/NoticePanel/NoticeScrollView")
        scrollview:setVisible(false)
        --
        local winSize = cc.Director:getInstance():getVisibleSize()
        self._webView = ccexp.WebView:create()
        self._webView:setAnchorPoint(scrollview:getAnchorPoint())
        self._webView:setContentSize(scrollview:getContentSize())
        self._webView:setPosition(scrollview:getPosition())
        self._webView:setLocalZOrder(scrollview:getLocalZOrder())
        self._webView:loadURL("http://zhs-rc-linux.fanhougame.com/game-notice/preview")
        self._webView:setScalesPageToFit(true)

        self._webView:setOnShouldStartLoading(function(sender, url)
            print("onWebViewShouldStartLoading, url is ", url)
            return true
        end)
        self._webView:setOnDidFinishLoading(function(sender, url)
            print("onWebViewDidFinishLoading, url is ", url)
        end)
        self._webView:setOnDidFailLoading(function(sender, url)
            print("onWebViewDidFinishLoading, url is ", url)
        end)

        scrollview:getParent():addChild(self._webView)

        self:initEvent()
    end
end

-- ÿ�ν���Open�����������ʱ�ص�
function UINoticeActivity:onOpenAniOver()
end

-- �����汻�ر�ʱ�ص�
-- ÿ�ε���Closeʱ�ص�
-- ���Է��ض������
function UINoticeActivity:onClose()
    self:removeEvent()
end

-- �����汻�ö�ʱ�ص�
-- Openʱ������ص��ú���
function UINoticeActivity:onTop(preUIID, ...)
    if self._webView then
        self._webView:setVisible(true)
    end
end

function UINoticeActivity:onClick(obj)
    local name = obj:getName()
    if "CloseButton" == name then
        UIManager.close()
    end
end

function UINoticeActivity:onCheckBox(obj, checkType)
    MusicManager.playSoundEffect(obj:getName())

    if 0 == checkType then  -- ѡ��
        cc.UserDefault:getInstance():setIntegerForKey("NoticeActivityHideTime", os.time())
    else                    -- ȡ��
        cc.UserDefault:getInstance():setIntegerForKey("NoticeActivityHideTime", 0)
    end
end


---------------------------------------------------------------------
-- ��ʼ���¼��ص�
function UINoticeActivity:initEvent()
    -- ��ӽ�����¼�����
    self.openUIHandler = handler(self, self.openUI)
    EventManager:addEventListener(GameEvents.EventOpenUI, self.openUIHandler)
end

-- �Ƴ��¼��ص�
function UINoticeActivity:removeEvent()
    -- �Ƴ�������¼�����
    if self.openUIHandler then
        EventManager:removeEventListener(GameEvents.EventOpenUI, self.openUIHandler)
        self.openUIHandler = nil
    end
end

function UINoticeActivity:openUI(eventName, uiID)
    if uiID ~= UIManager.UI.UINoticeActivity then
        if self._webView then
            self._webView:setVisible(false)
        end
    end
end

return UINoticeActivity