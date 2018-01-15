--[[
/*******************************************************************
** ������:	¬��
** ��  ��:  2016-09-7 12:20
** ��  ��:	1.0
** ��  ��:  �ٻ�ʦ������ʾ����
** Ӧ  ��:
********************************************************************/
--]]

local UISummonerBuyTips = class("UISummonerBuyTips", function()
    return require("common.UIView").new()
end)

-- ���캯��
function UISummonerBuyTips:ctor()

end

-- �����汻����ʱ�ص�
-- ֻ��ʼ��һ��
function UISummonerBuyTips:init(...)
    --���ؽ���
    self.rootPath = ResConfig.UISummonerBuyTips.Csb2.main
    self.root = getResManager():getCsbNode(self.rootPath)
    self:addChild(self.root)

    -- ���԰����
    getChild(self.root, "TipsPanel/TitleText"):setString(getUILanConfItem(281))
    getChild(self.root, "TipsPanel/TipsText"):setString(getUILanConfItem(282))
    getChild(self.root, "TipsPanel/Text_1_0"):setString(getUILanConfItem(283))
    getChild(self.root, "TipsPanel/UnlockButton/ButtonText"):setString(getUILanConfItem(500))

    -- �رհ�ť
    local btnClose = getChild(self.root, "TipsPanel/Button_Close")
    CsbTools.initButton(btnClose, handler(self, self.onClick), nil, nil, "ButtonText")

    -- ȷ����ť
    local btnConfirm = getChild(self.root, "TipsPanel/UnlockButton")
    CsbTools.initButton(btnConfirm, handler(self, self.onClick), nil, nil, "ButtonText")
end

-- �����汻��ʱ�ص�
-- ÿ�ε���Openʱ�ص�
function UISummonerBuyTips:onOpen(openerUIID, summonerID, callback)
    self.summonerID = summonerID
    self.callback = callback

    self.summonerConf = getSaleSummonerConfItem(self.summonerID)

    self:setSummonerBuyInfo()
end

-- ÿ�ν���Open�����������ʱ�ص�
function UISummonerBuyTips:onOpenAniOver()
end

-- �����汻�ر�ʱ�ص�
-- ÿ�ε���Closeʱ�ص�
-- ���Է��ض������
function UISummonerBuyTips:onClose()
    self.summonerID = nil
    self.callback = nil
    self.summonerConf = nil

    if self.buySummonerHandler then
        local cmd = NetHelper.makeCommand(MainProtocol.Summoner, SummonerProtocol.BuySC)
        NetHelper.removeResponeHandler(cmd, self.buySummonerHandler)
        self.buySummonerHandler = nil
    end
end

-- �����汻�ö�ʱ�ص�
-- Openʱ������ص��ú���
function UISummonerBuyTips:onTop(preUIID, ...)

end

-- ��ǰ���水ť����¼�
function UISummonerBuyTips:onClick(obj)
    local objName = obj:getName()
    if "Button_Close" == objName then
        UIManager.close()
    elseif "UnlockButton" == objName then
        obj.soundId = nil
        -- �ж�vip�Ƿ�ﵽ
        if self.summonerConf ~= nil then
            -- �ж��Ƿ����㹻�Ļ��ҹ����ٻ�ʦ
            if self.summonerConf.Type == 2 then
                local userGold = getGameModel():getUserModel():getGold()
                if userGold >= self.summonerConf.Num then
                    self:sendBuySummonerCmd()
                else
                    obj.soundId = MusicManager.commonSound.fail
                    CsbTools.addTipsToRunningScene(CommonHelper.getUIString(572))
                end
            elseif self.summonerConf.Type == 1 then
                local userDiamond = getGameModel():getUserModel():getDiamond()
                if userDiamond >= self.summonerConf.Num then
                    self:sendBuySummonerCmd()
                else
                    obj.soundId = MusicManager.commonSound.fail
                    CsbTools.addTipsToRunningScene(CommonHelper.getUIString(530))
                end
            end
        else
            print("self.summonerConf is nil")
        end
    end
end

-- �����ٻ�ʦ������Ϣ
function UISummonerBuyTips:setSummonerBuyInfo()
    local heroConf = getHeroConfItem(self.summonerID)

    -- �ٻ�ʦͷ��
    local summonerHead = getChild(self.root, "TipsPanel/FileNode_1")
    CommonHelper.playCsbAnimate(summonerHead, "ui_new/g_gamehall/b_bag/HeroItem.csb", "Summoner", false, nil, true)
    local iconImg = getChild(summonerHead, "Item/Icon")
	local frameImg = getChild(summonerHead, "Item/Level")
	CsbTools.replaceImg(iconImg, heroConf.Common.HeadIcon)
	CsbTools.replaceImg(frameImg, IconHelper.getSoldierHeadFrame(5))
    -- �ٻ�ʦ����
    local summonerName = getChild(self.root, "TipsPanel/NameText")
    summonerName:setString(getHSLanConfItem(heroConf.Common.Name))
    -- �ٻ�ʦ�۸�
    local summonerPrice = getChild(self.root, "TipsPanel/PriceLabel")
    summonerPrice:setString(self.summonerConf.Num)
    -- �ٻ�ʦ�����������
    local coinImage = getChild(self.root, "TipsPanel/Image_bg/pub_gem_15")
    if self.summonerConf.Type == 1 then
        coinImage:setSpriteFrame("pub_gem.png")
    elseif self.summonerConf.Type == 2 then
        coinImage:setSpriteFrame("pub_gold.png")
    end
end

-- ���͹����ٻ�ʦ����
function UISummonerBuyTips:sendBuySummonerCmd()
    -- ע�Ṻ���ٻ�ʦ����
    local cmd = NetHelper.makeCommand(MainProtocol.Summoner, SummonerProtocol.BuySC)
    self.buySummonerHandler = handler(self, self.acceptBuySummonerCmd)
    NetHelper.setResponeHandler(cmd, self.buySummonerHandler)

    -- ��������
    local buffData = NetHelper.createBufferData(MainProtocol.Summoner, SummonerProtocol.BuyCS)
    buffData:writeInt(self.summonerID)
    NetHelper.request(buffData)
end

-- ���չ����ٻ�ʦ����
function UISummonerBuyTips:acceptBuySummonerCmd(mainCmd, subCmd, buffData)
    -- ע�������ٻ�ʦ����
    local cmd = NetHelper.makeCommand(mainCmd, subCmd)
    NetHelper.removeResponeHandler(cmd, self.buySummonerHandler)
    self.buySummonerHandler = nil

    local tp = buffData:readInt()   -- ��������
    local num = buffData:readInt()  -- ��������

    -- �۳�����
    if tp == 1 then
        ModelHelper.addCurrency(UIAwardHelper.ResourceID.Diamond, -num)
    elseif tp == 2 then
        ModelHelper.addCurrency(UIAwardHelper.ResourceID.Gold, -num)
    end
    -- ����ٻ�ʦ
    ModelHelper.AddSummoner(self.summonerID)
    -- ���ͷ��
    if self.summonerConf then
        ModelHelper.addHead(self.summonerConf.HeadID)
    end

    if self.callback and "function" == type(self.callback) then
        self.callback()
    end

    -- ��ʾ�ٻ�ʦ
    UIManager.replace(UIManager.UI.UIShowSummoner, self.summonerID)
end

return UISummonerBuyTips