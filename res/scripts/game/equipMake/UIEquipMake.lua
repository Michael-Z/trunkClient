--[[
	UIEquipMake װ���������

]]

-- csb�ļ�
local csbFile = ResConfig.UIEquipMake.Csb2
local btnFile = "ui_new/g_gamehall/b_bag/AllButton2.csb"
local downListBufftonFile = "ui_new/g_gamehall/s_smith/DownListButton.csb"
local AllButton = "ui_new/g_gamehall/b_bag/AllButton.csb"
local breakButton = "ui_new/g_gamehall/s_smith/BreakButton.csb"

local MakeItem = "ui_new/g_gamehall/s_smith/MakeItem.csb"
local MakeButton = "ui_new/g_gamehall/s_smith/MakeButton.csb"

local isQuality = cc.UserDefault:getInstance():getBoolForKey("isQuality")

local equipMakeModel = getGameModel():getEquipMakeModel()
local UIEquipMakeRedHelper = require("game.equipMake.UIEquipMakeRedHelper")
local maxCount = 10

local buttonType = {equipMake  = 1,equipBreak = 2}
--�Ժ�Ҫ��չ����������չ�ͺ���
-- ս����ְҵ
local jobType = {[1]=6, [2]=1, [3]=2, [4]=3, [5]=4, [6]=5}
-- �ȼ�
local levelType = {1,2,3}
-- ��λ
local partType = {1,2,3,4,5,6}

local levelName = {20,35,50}   --����Ժ�Ҫ��չ,�ǵ�ȥUIEquipBag��part7BtnCallBack����һ���޸�,��Щ���ǲ߻�����Ŀ�

-- ְҵ�����԰�
local jobName = {[1]=1260, [2]=1261, [3]=1262, [4]=1263, [5]=1264, [6]=520}
-- ��λ���԰�
local partName = {612,613,614,615,616,617}

--������ʾ���԰�
local makeTipName = {1275,1276,1277,1278,1279,1280}

local Cailiao = {}

local effecMusic = {52, 52, 52, 53, 53}

local animation = {"Open", "Open2", "Open2", "Open3", "Open3"}

local UIEquipMake = class("UIEquipMake", function() return require("common.UIView").new() end)

function UIEquipMake:ctor()
	self.rootPath	= csbFile.SmithShop
	self.root   	= getResManager():getCsbNode(self.rootPath)
	self:addChild(self.root)

	self.mIsLevel = true

	self.mCurJob   = jobType[1]
	self.mCurLevel = levelType[1]
	self.mCurParts = partType[1]
	self.mIsCanSend = true
	self.mMoneyenough = true
	self.isSuccess = true
	self.isIng = true
	self.mWhichCaliao = {}

	--���
	self.mLevelRed = nil
	self.mJobRedPoint = {}
	self.mLevelRedPoint = {}
	self.mPartsRedPoint = {}
	self.mCurIndex = 1

	self.mCurJobBreak = 1--jobType[1]
	self.mIsJobBreak = true
	self.mSelectedCount = 0  -- ��ѡ��װ����
	self.mBreakMoney = 0     -- �ֽ���Ҫ�Ľ��

	self.mIsMakeBack = false   --��onTopʱ�ж��ǲ��Ǵ���������ʱ��

	self.mImportantEquipCount = 0 --�ֽ�ʱ��ѡ��Ʒ�ʴ�������װ��������Ϊ0ʱ��ʾû��

	self.mEquipDataIndex = {} -- �����õ���װ���б�
	self.mRealShowEuips = {}  --�ֽ�Ҫչʾ��װ��
	self.mBreakEquip = {}

	-- �˳���ť
	self.mBackButton = CsbTools.getChildFromPath(self.root, "BackButton")
	CsbTools.initButton(self.mBackButton, handler(self, self.backBtnCallBack), nil, nil, "mBackButton")

	self.mForging = CsbTools.getChildFromPath(self.root, "Forging")
	self.mForging:setVisible(false)
	self.mForgingText = CsbTools.getChildFromPath(self.mForging, "MainPanel/TextTips/Text_1")

	self.mForgingBackg = CsbTools.getChildFromPath(self.mForging, "Background")
	 CsbTools.initButton(self.mForgingBackg,  handler(self,  self.backGroudCallback), nil, nil, "mTabMakeButton")

	--  ����ֽ�����л�
	self.mMakeBreakButton = CsbTools.getChildFromPath(self.root, "MainPanel/TabButton_7")
	self.mMakeBreakButtonText = CsbTools.getChildFromPath(self.root, "MainPanel/TabButton_7/Node/ButtonText")
	CsbTools.initButton(self.mMakeBreakButton,  handler(self,  self.btnCallBack), CommonHelper.getUIString(1240), self.mMakeBreakButtonText, "self.mMakeBreakButton")
	self.mMakeBreakButton:setTag(buttonType.equipMake)
	self.mMakeIcon = CsbTools.getChildFromPath(self.root, "MainPanel/TabButton_7/Node/smith_makeicon")
	self.mBreakIcon = CsbTools.getChildFromPath(self.root, "MainPanel/TabButton_7/Node/smith_breakicon")
	-- ������ֽ����
	self.mMakePanel  = CsbTools.getChildFromPath(self.root, "MainPanel/MakePanel")
	self.mBreakPanel = CsbTools.getChildFromPath(self.root, "MainPanel/BreakPanel")
	self.mMakePanel:setVisible(true)
	self.mBreakPanel:setVisible(false)
	--�����ʯ
	self.mDiamond  = CsbTools.getChildFromPath(self.root, "MainPanel/Diamond")
	self.mCoin     = CsbTools.getChildFromPath(self.root, "MainPanel/Coin")

	self.mMakeItem = CsbTools.getChildFromPath(self.mMakePanel, "MainPanel/MakeItem")

	-- �������
	--���
	self.mPropScrollView = CsbTools.getChildFromPath(self.mMakePanel, "MainPanel/PropScrollView")
	self.mPropScrollView:setScrollBarEnabled(false)
	self.mLevelList      = CsbTools.getChildFromPath(self.mMakePanel, "MainPanel/LevelList")  
	--�ұ�
	self.mPriceNum       = CsbTools.getChildFromPath(self.mMakePanel, "MainPanel/PriceNum")    --��Ҫ��Ǯ
	self.mPropName       = CsbTools.getChildFromPath(self.mMakePanel, "MainPanel/PropName")    --װ������

	self.mMakeButton     	  = CsbTools.getChildFromPath(self.mMakePanel, "MainPanel/MakeButton") 
	self.mMakeButtonOrange    = CsbTools.getChildFromPath(self.mMakePanel, "MainPanel/MakeButton/Button_Orange")   --��Ʒװ������װ�����찴ť
	self.mMakeButtonGreen     = CsbTools.getChildFromPath(self.mMakePanel, "MainPanel/MakeButton/Button_Green")     --��ͨװ������װ�����찴ť
	self.mMakeButtonGrey      = CsbTools.getChildFromPath(self.mMakePanel, "MainPanel/MakeButton/Button_Grey")       --���ϲ���

	CsbTools.initButton(self.mMakeButtonOrange,  handler(self,  self.beginEquipMake), nil, nil, "Text")
	CsbTools.initButton(self.mMakeButtonGreen,   handler(self,  self.beginEquipMake), nil, nil, "Text")

	self.mTextOrange           = CsbTools.getChildFromPath(self.mMakeButtonOrange, "Text")				   --װ�������ı�
	self.mTextGreen            = CsbTools.getChildFromPath(self.mMakeButtonGreen, "Text")				   --װ�������ı�
	self.mTextGrey             = CsbTools.getChildFromPath(self.mMakeButtonGrey, "Text")				   --װ�������ı�

	for i=1,5 do
		self["Material_"..i] = {}
	end
	for i=1,5 do
		self["Material_"..i].root = CsbTools.getChildFromPath(self.mMakePanel, "MainPanel/Material_"..i)  --����
		self["Material_"..i].node = CsbTools.getChildFromPath(self["Material_"..i].root, "MaterialItem/PropItem")  
		self["Material_"..i].Num = CsbTools.getChildFromPath(self["Material_"..i].root, "MaterialItem/Num")  --��������
		self["Material_"..i].Name = CsbTools.getChildFromPath(self["Material_"..i].root, "MaterialItem/Name")  --��������
		self["Material_"..i].MaterialIcon = CsbTools.getChildFromPath(self["Material_"..i].root, "MaterialItem/PropItem/Item/icon")  --����ICON
		self["Material_"..i].Level = CsbTools.getChildFromPath(self["Material_"..i].root, "MaterialItem/PropItem/Item/Level")  --����ICON
		CsbTools.getChildFromPath(self["Material_"..i].root, "MaterialItem/PropItem/Item/Num"):setVisible(false) 
		item = ccui.Button:create()
        item:setName("CastMaterialButton")
		item:setTouchEnabled(true)
		item:setScale9Enabled(true)
		item:setTag(i)
		local itemSize = CsbTools.getChildFromPath(self.mMakePanel, "MainPanel/Material_1/MaterialItem"):getContentSize()
		item:setContentSize(itemSize)
		CsbTools.initButton(item, handler(self, self.TouchCallBack))
		self["Material_"..i].root:addChild(item)
	end

	self.mCheckBox 		 = CsbTools.getChildFromPath(self.mMakePanel, "MainPanel/CheckBox")  --��ѡ��
	self.mCheckBox:addEventListener(handler(self, self.checkBoxSelectedEvent))
    self.mCheckBox:setSelected(isQuality)

    self.mViewButton = CsbTools.getChildFromPath(self.mMakePanel, "MainPanel/ViewButton")  
	CsbTools.initButton(self.mViewButton,  handler(self,  self.viewCallBack), nil, nil, "mViewButton")

	self.mQuestion = CsbTools.getChildFromPath(self.mMakePanel, "MainPanel/QuestionButton")
	CsbTools.initButton(self.mQuestion,  handler(self,  self.questionCallBack), nil, nil, "mQuestion")

	self.mArrowIcon2 = CsbTools.getChildFromPath(self.mMakePanel, "MainPanel/ArrowIcon2")
	self.mArrowIcon  = CsbTools.getChildFromPath(self.mMakePanel, "MainPanel/ArrowIcon/smith_arrow_icon_2")

	self.mMakeTip = CsbTools.getChildFromPath(self.mMakePanel, "MainPanel/Tips2")

	self.mPropImage = CsbTools.getChildFromPath(self.mMakePanel, "MainPanel/PropImage")
	self.mOldPositionX,self.mOldPositionY = self.mPropImage:getPosition()
	-- �ֽ����
	self.mJobListBreak 		= CsbTools.getChildFromPath(self.mBreakPanel, "MainPanel/LevelList")
	self.mJobListBreak:setLocalZOrder(9999)
	self.mBreakButtonFather = CsbTools.getChildFromPath(self.mBreakPanel, "BreakButton")
	self.mBreakButton = CsbTools.getChildFromPath(self.mBreakPanel, "BreakButton/BreakButton")
	CsbTools.initButton(self.mBreakButton, handler(self, self.beginEquipBreakFrist))
	
	self.mPriceNumBreak			= CsbTools.getChildFromPath(self.mBreakPanel, "MainPanel/PriceNum")
	self.mPropScrollViewBreak 	= CsbTools.getChildFromPath(self.mBreakPanel, "MainPanel/PropScrollView")
	self.mPropScrollViewBreak:setScrollBarEnabled(false)
	self.mPropScrollViewBreak:setDirection(1)
	self.mMaterialScrollView 	= CsbTools.getChildFromPath(self.mBreakPanel, "MainPanel/MaterialScrollView")
	self.mMaterialScrollView:setScrollBarEnabled(false)

	-- �������
	self.hallCoinLb = CsbTools.getChildFromPath(self.root, "Coin/Coin/CoinLabel")
    self.hallDiamondLb = CsbTools.getChildFromPath(self.root, "Diamond/Diamond/PowerLabel_0")

	local buyCoinBtn = CsbTools.getChildFromPath(self.root, "Coin/Coin/CoinButton")
	CsbTools.initButton(buyCoinBtn, function ()
		UIManager.open(UIManager.UI.UIGold)
	end)
	-- ������ʯ+��ť
	local buyDiamondBtn = CsbTools.getChildFromPath(self.root, "Diamond/Diamond/PowerButton_0")
	CsbTools.initButton(buyDiamondBtn, function ()
        UIManager.open(UIManager.UI.UIShop, ShopType.DiamondShop)
	end)

	-- item��size
	local itemCsb 	= getResManager():getCsbNode(csbFile.PropBar)
	self.mPropBarSize	= CsbTools.getChildFromPath(itemCsb, "PropItem"):getContentSize()

	local itemCsb1 	= getResManager():getCsbNode(ResConfig.UIBag.Csb2.item)
	self.itemSize	= CsbTools.getChildFromPath(itemCsb1, "Item"):getContentSize()
    itemCsb:cleanup()
    itemCsb1:cleanup()

	self.cailiaoSize = self.itemSize
end

function UIEquipMake:onOpen(preId,isMainCity, uiData)
	self.mMakeTime = 1
	self.mMaxTime = 1
	self.isSuccess = true
	self.mForging:setVisible(false)
	-- �����ǽ���ʱ,ְҵΪսʿ,�ȼ�Ϊ��ɫ�ܴ�����õȼ�
    local cmd = NetHelper.makeCommand(MainProtocol.Bag, BagProtocol.makeEquipSC)
    local mCallBack = handler(self, self.makeCallBack)
    NetHelper.setResponeHandler(cmd, mCallBack)

    local cmd = NetHelper.makeCommand(MainProtocol.Bag, BagProtocol.breakEquipSC)
    local mCallBack = handler(self, self.breakCallBack)
    NetHelper.setResponeHandler(cmd, mCallBack)

    local newUiData = {}
    -- ����������������Դ����
	if isMainCity and uiData==nil then

	 	newUiData.Eq_Vocation = 6
	  	newUiData.Eq_Parts    = 1
	  	local userLevel = getGameModel():getUserModel():getUserLevel()
	  	if userLevel < levelName[2] then
	  		newUiData.Eq_Level = 1
	  	elseif  levelName[2] <= userLevel and userLevel < levelName[3] then
	  		newUiData.Eq_Level = 2
	  	elseif  levelName[3] <= userLevel  then
	  		newUiData.Eq_Level = 3
	  	end
	end
	--���
	UIEquipMakeRedHelper:abcdefg()

 	self:initUI(uiData==nil and newUiData or uiData)
end

function UIEquipMake:onClose()
	local cmd = NetHelper.makeCommand(MainProtocol.Bag, BagProtocol.makeEquipCS)
 	NetHelper.removeResponeHandler(cmd,handler(self, self.netCallBack))
end

function UIEquipMake:onTop()
	self:initCommonUI()
	self:initRightUI()
	if self.mIsMakeBack then
		self.mIsMakeBack = false
		--�ô���дһ��Ư���Ķ���
		if self.mPropImage then
			print("��ʼ���ų����ô���д��һ���ǳ�Ư���Ķ���")
			self.mPropImage:runAction(cc.Sequence:create(cc.MoveBy:create(0.5,{x=700,y=-30}), cc.CallFunc:create(function()
					self.mPropImage:setVisible(false)
					self.mMakeItem:setVisible(true)
					self.mPropImage:setPosition(cc.p(self.mOldPositionX,self.mOldPositionY))
					self.isSuccess = true
              end)))
		end
	end
end

--[[
	uiData Ϊ�˿����ڲ�ͬ�Ľ�����ת�����������ɲ�ͬ�ĳ�ʼ��,��Ҫ�򿪵�ʱ�������
	uiData.Eq_Vocation ְҵ uiData.Eq_Level    �ȼ� uiData.Eq_Parts    ��λ               ]]
--------------- �����ʼ��----------------------
function UIEquipMake:initUI(uiData)
	--��ʾ��ԭ
	self.mMakePanel:setVisible(true)
	self.mBreakPanel:setVisible(false)

   	-- ���ִ���״̬λ��ԭ
	self.mIsLevel = true
	-- �ֽ�״̬λ��ԭ
	self.mIsJobBreak = true
	self.mCurJobBreak = 1--jobType[2]


	self.mCurJob      = uiData.Eq_Vocation
	self.mCurLevel    = uiData.Eq_Level
	self.mCurParts    = uiData.Eq_Parts


	self.mEquipDataIndex = {}
   	self.mEquipDataIndex = equipMakeModel:getEquipByHead(self.mCurJob, levelName[self.mCurLevel])
	--dump(self.mEquipDataIndex)	
	print("���ҳ�������������Щ��self.mCurJob, self.mCurLevel, self.mCurParts", self.mCurJob, self.mCurLevel, self.mCurParts)

	--�������
	self:initCommonUI()
	self:initLeftUI()
	self:initRightUI()

	--�ֽ����
	self:initBreakUI()
end

function UIEquipMake:initCommonUI()
    local gold = getGameModel():getUserModel():getGold()
    local gem = getGameModel():getUserModel():getDiamond()
    self.hallCoinLb:setString(gold)
    self.hallDiamondLb:setString(gem)
end

function UIEquipMake:initLeftUI()

	self.mJobRedPoint = {}
	self.mLevelRedPoint = {}

	--  �ȼ�ѡ��
	local OrderButton = CsbTools.getChildFromPath(self.mLevelList, "OrderButton")
	local ButtonName = CsbTools.getChildFromPath(self.mLevelList, "OrderButton/ButtonName")
	CsbTools.initButton(OrderButton, handler(self, self.levelListCallBack), "OrderButton")
	ButtonName:setString(levelName[self.mCurLevel]..CommonHelper.getUIString(528))

	self.mLevelRed = CsbTools.getChildFromPath(OrderButton, "RedTipPoint")
	self.mLevelRed:setVisible(UIEquipMakeRedHelper.mCanShowLevelRed[self.mCurJob] ~= nil)
	-- �ȼ����Ҫ֧����չ,�±���,��д�ÿ���չ�߼�
	local DownListView = CsbTools.getChildFromPath(self.mLevelList, "DownListView")
	self.mLevelListBtn = {}
	local template = DownListView:getItem(0):clone()
	DownListView:removeAllItems()
	DownListView:setContentSize(cc.size(DownListView:getContentSize().width,template:getContentSize().height*#levelType))
	for i=1,#levelType do
		self.mLevelListBtn[i] = template:clone()
		self.mLevelListBtn[i]:setTag(levelType[i])
		CsbTools.initButton(self.mLevelListBtn[i], handler(self, self.levelListSonCallBack), "Button_"..i)
		self.mLevelListBtn[i]:setTitleText(levelName[i]..CommonHelper.getUIString(528))
		DownListView:pushBackCustomItem(self.mLevelListBtn[i])

		self.mLevelRedPoint[i] = CsbTools.getChildFromPath(self.mLevelListBtn[i] , "RedTipPoint")
		self.mLevelRedPoint[i]:setVisible(UIEquipMakeRedHelper.mCanShowLevelRed[self.mCurJob] ~=nil and UIEquipMakeRedHelper.mCanShowLevelRed[self.mCurJob][levelName[i]] ~= nil)
	end

	self.mJobList = {}
	for i=1,#jobType do
		self.mJobList[i] = CsbTools.getChildFromPath(self.root, "MainPanel/TabButton_"..i)
		self.mJobList[i]:setTag(i)
		CsbTools.initButton(self.mJobList[i], handler(self, self.jobListSonCallBack), "Button_"..i)
		local text = CsbTools.getChildFromPath(self.mJobList[i], "AllButton/ButtonPanel/NameLabel")
		text:setString(CommonHelper.getUIString(jobName[jobType[i]]))

		self.mJobRedPoint[i] = CsbTools.getChildFromPath(self.mJobList[i] , "AllButton/RedTipPoint")
		self.mJobRedPoint[i]:setVisible(UIEquipMakeRedHelper.mCanShowJobRed[jobType[i]] ~= nil)
	end

   	for i=1,#jobType  do
   		CommonHelper.playCsbAnimate(self.mJobList[i], AllButton, "Normal", false, nil, true)
   		self.mJobList[i]:setLocalZOrder(i)
   	end

	local temp = 1
	for i=1,#jobType do
		if self.mCurJob == jobType[i] then
			temp = i
			break
		end
	end
	CommonHelper.playCsbAnimate(self.mJobList[temp], AllButton, "On", false, nil, true)

	self.mJobList[temp]:setLocalZOrder(99999)
	self:initLeftScrollView()
end

-- ��ʼ��װ��ѡ���scrollView
function UIEquipMake:initLeftScrollView()
	
	local intervalX = 10
	local intervalY = 10
	local offsetX = 0
	local offsetY = 5
	local hang = #self.mEquipDataIndex

	local innerSize = self.mPropScrollView:getContentSize()

	local h = offsetY + hang*self.mPropBarSize.height + (hang + 1)*intervalY
	if h > innerSize.height then
		innerSize.height = h
	end

    if self.mPropScrollView:getInnerContainerSize().height ~= innerSize.height then
	    self.mPropScrollView:setInnerContainerSize(innerSize)
    end  

	self.mPropScrollView:removeAllChildren()
	self.mPartsRedPoint = {}
	self.mCurIndex = self.mEquipDataIndex[1]
	for i,index in ipairs(self.mEquipDataIndex) do
		local item = nil
		item = ccui.Button:create()
        item:setName("CastEquipmentButton")
		item:setTouchEnabled(true)
		item:setScale9Enabled(true)
		item:setContentSize(self.mPropBarSize)
		CsbTools.initButton(item, handler(self, self.propBarTouchCallBack))
		self.mPropScrollView:addChild(item)
		local itemCsb = getResManager():cloneCsbNode(csbFile.PropBar)
		itemCsb:setPosition(cc.p(self.mPropBarSize.width/2, self.mPropBarSize.height/2))
		itemCsb:setTag(7758258)
		item:addChild(itemCsb)	
		local posX = offsetX + self.mPropBarSize.width/2
		local posY = innerSize.height - offsetY - (i - 0.5)*self.mPropBarSize.height - (i - 1)*intervalY
		item:setPosition(cc.p(posX,posY))
		item:setTag(i)
		self:initPropBar(itemCsb, index, i)
	end

	-- �������ĸ�λ��,ѡ���ļ�װ��
	-- ѡ��װ����,�ұ�Ҫ��ʼ��
	local percent = self.mCurParts==1 and 0 or (self.mCurParts)/6 * 100 
	self.mPropScrollView:jumpToPercentVertical(percent)
	print("self.mCurParts", self.mCurParts)
	local button = self.mPropScrollView:getChildByTag(self.mCurParts)
	if button then
		local itemCsb = button:getChildByTag(7758258)
		CommonHelper.playCsbAnimate(itemCsb, csbFile.PropBar, "Choose", false, nil, true)
	end
end

function UIEquipMake:initPropBar(cardCsb, index, i)
	local equipData = equipMakeModel:getEquipByIndex(index)
	local PropIcon = CsbTools.getChildFromPath(cardCsb, "PropItem/PropIcon")
	local Name = CsbTools.getChildFromPath(cardCsb, "PropItem/Name")
	local Level = CsbTools.getChildFromPath(cardCsb, "PropItem/Level")
	local Body = CsbTools.getChildFromPath(cardCsb, "PropItem/Body")
	self.mPartsRedPoint[i] = CsbTools.getChildFromPath(cardCsb, "PropItem/RedTipPoint")
	self.mPartsRedPoint[i]:setVisible(UIEquipMakeRedHelper.mCanShowPartsRed[index] ~= nil)

	CsbTools.replaceSprite(PropIcon, equipData.Item_Icon)
	Name:setString(CommonHelper.getPropString(equipData.Item_Name))
	Level:setString(equipData.Eq_Level..CommonHelper.getUIString(528))
	Body:setString(CommonHelper.getUIString(partName[equipData.Eq_Parts]))				
end

function UIEquipMake:initRightUI()
	--��ʼ���ұ�UIʱ,Ҫ�ѱ�־�Ǹ�ԭ ,��Ȼ�ͷ��Ͳ���Э����
	self.mIsCanSend = true
	self.mMoneyenough = true
	self.mWhichCaliao = {}
	self.mMakeMoney = 0
	local equipData = equipMakeModel:getEquipByIndex(self.mEquipDataIndex[self.mCurParts])

	CommonHelper.playCsbAnimate(self.mMakeItem, MakeItem, "Unknow", false, nil, true)

	local myGold = getGameModel():getUserModel():getGold()
	local needGold = isQuality and equipData.Eq_QualityCastGoldCost or equipData.Eq_NormalCastGoldCost
	self.mPriceNum:setString(needGold)    --��Ҫ��Ǯ
    self.mPriceNum:setColor(display.COLOR_WHITE)
	if myGold < needGold then
		self.mPriceNum:setColor(display.COLOR_RED)
		self.mIsCanSend = false
		self.mMoneyenough = false -- ��Ҳ����־λ
	end
	self.mMakeMoney = needGold
	self.mPropName:setString(CommonHelper.getPropString(equipData.Item_Name))    --װ������
	self.mMakeTip:setString(CommonHelper.getUIString(makeTipName[self.mCurParts]))

	--�����в��Ҳ�������,û��Ϊ0
	--����һ����
	for i=1,5 do
		local id = equipData["Eq_Synthesis"..i]
		print("i =  id ", i , id)
		if id ==0  then
			self["Material_"..i].root:setVisible(false)
		else
            self["Material_"..i].root:setVisible(true)
			local propConf = getPropConfItem(equipData["Eq_Synthesis"..i])
			local count = getGameModel():getBagModel():getItemCountById(equipData["Eq_Synthesis"..i])
			local needCount = equipData["Eq_Synthesis"..i.."Param"]
			self["Material_"..i].Num:setString(count.."/"..needCount)
			self["Material_"..i].Num:setColor(cc.c3b(167,102,0))

			if count < needCount then
				if i~=5 then
					self.mIsCanSend =  false  --�к�ɫ,���÷���
				end
				self["Material_"..i].Num:setColor(display.COLOR_RED)
				table.insert(self.mWhichCaliao,i , i)
			end
			local name = CommonHelper.getPropString(propConf.Name)
			self["Material_"..i].Name:setString(name and name or "")

			UIAwardHelper.setPropItemOfConf(self["Material_"..i].node, propConf, 0)

			-- CsbTools.replaceImg(self["Material_"..i].MaterialIcon, propConf.Icon) --icon
			-- local frame = getItemLevelSettingItem(propConf.Quality).ItemFrame
			-- CsbTools.replaceImg(self["Material_"..i].Level, frame) --��ͼ
		end
	end

	self.mTextOrange:setString(CommonHelper.getUIString(1245))  --��ʾ��Ʒ����
	self.mTextGreen:setString( CommonHelper.getUIString(1244))  --��ʾ��ͨ����
	self.mTextGrey:setString(CommonHelper.getUIString(137))  	--���ϲ���

	-- checkBox����,�б�������
	self.mArrowIcon2:setVisible(false)--������ͷ
	self.mArrowIcon:setVisible(false)
	if isQuality then  -- ѡ��
		self.mArrowIcon2:setVisible(true)
		self.mArrowIcon:setVisible(true)
		CommonHelper.playCsbAnimate(self.mMakeButton, MakeButton, "Orange", false, nil, true)
    else                    -- ȡ��
    	CommonHelper.playCsbAnimate(self.mMakeButton, MakeButton, "Green", false, nil, true)
		self.mArrowIcon2:setVisible(false)
		self.mArrowIcon:setVisible(false)
    end

    -- if not self.mMoneyenough then
    -- 	CommonHelper.playCsbAnimate(self.mMakeButton, MakeButton, "Grey", false, nil, true)
    -- end
end

-- �ֽⲿ��API
function UIEquipMake:initBreakUI()
	-- ְҵѡ��
	local OrderButtonB = CsbTools.getChildFromPath(self.mJobListBreak, "OrderButton")
	local ButtonNameB = CsbTools.getChildFromPath(self.mJobListBreak, "OrderButton/ButtonName")
	CsbTools.initButton(OrderButtonB, handler(self, self.jobListCallBackBreak), "OrderButtonB")
	ButtonNameB:setString(self.mCurJobBreak==1 and CommonHelper.getUIString(513) or CommonHelper.getUIString(jobName[self.mCurJobBreak-1]))

	local DownListViewB = CsbTools.getChildFromPath(self.mJobListBreak, "DownListView")
	local template = DownListViewB:getItem(0):clone()

	DownListViewB:removeAllItems()
	DownListViewB:setContentSize(cc.size(DownListViewB:getContentSize().width,template:getContentSize().height*6))

	self.mLevelListBtnBreak = {}
	for i=1,6 do
		self.mLevelListBtnBreak[i] = template:clone()
		self.mLevelListBtnBreak[i]:setTag(i)
		CsbTools.initButton(self.mLevelListBtnBreak[i], handler(self, self.jobListSonCallBackBreak), "Button_"..i)
		self.mLevelListBtnBreak[i]:setTitleText(i==1 and CommonHelper.getUIString(513) or CommonHelper.getUIString(jobName[i-1]))
		DownListViewB:pushBackCustomItem(self.mLevelListBtnBreak[i])
	end

	self.mShowEquips = {}
	self.mShowEquips = equipMakeModel:getEquipModelCanBreakEquip()
	self:selectEquip()
	self:initTopScrollViewBreak()
	CommonHelper.playCsbAnimate(self.mBreakButtonFather, breakButton,  self.mSelectedCount > 0 and "Able" or "Disable", false, nil, true)
	--dump(self.mRealShowEuips)
	self:initDowniScrollViewBreak()

end

function UIEquipMake:initTopScrollViewBreak()

	self.mPropScrollViewBreak:removeAllChildren()
	if #self.mRealShowEuips == 0 then
		self.mPropScrollViewBreak:setInnerContainerSize(self.mPropScrollViewBreak:getContentSize())
		return
	end

	local itemPlace = {}
	local hang = 0
	local lie = 8

	for i, info in ipairs(self.mRealShowEuips) do
		if lie == 8 then
			hang = hang + 1
			lie = 1
		else
			lie = lie + 1
		end
		table.insert(itemPlace, {hang = hang, lie = lie})
	end

	local intervalX = 9
	local intervalY = 9
	local offsetX = 6
	local offsetY = 6
	local innerSize = self.mPropScrollViewBreak:getContentSize()
	--��scrollview�ߵĴ�С
	local hang = itemPlace[#itemPlace].hang

	local h = offsetY + hang*self.itemSize.height + (hang + 1)*intervalY
	if h > innerSize.height then
		innerSize.height = h
	end

    if self.mPropScrollViewBreak:getInnerContainerSize().height ~= innerSize.height then
	    self.mPropScrollViewBreak:setInnerContainerSize(innerSize)
    end      

	for i=1,#itemPlace do
		local item = nil
		item = ccui.Button:create()
		item:setTouchEnabled(true)
		item:setScale9Enabled(true)
		item:setContentSize(self.itemSize)
		CsbTools.initButton(item, handler(self, self.itemCallBack))

		local itemCsb = getResManager():cloneCsbNode(csbFile.PropItem)
		itemCsb:setPosition(cc.p(self.itemSize.width/2, self.itemSize.height/2))
		itemCsb:setTag(7758258)
		self:initTopitem(itemCsb, i)
		item:addChild(itemCsb)	

		local hang = itemPlace[i].hang
		local lie = itemPlace[i].lie
		local posX = offsetX + (lie - 0.5)* self.itemSize.width + (lie - 1)*intervalX
		local posY = innerSize.height - offsetY - (hang - 0.5)*self.itemSize.height - (hang - 1)*intervalY
		item:setPosition(cc.p(posX,posY))
		item:setTag(i)

		self.mPropScrollViewBreak:addChild(item)
	end
end

function UIEquipMake:initTopitem(itemCsb, index)
	local icon = CsbTools.getChildFromPath(itemCsb, "PropItem/Item/icon")
	local level = CsbTools.getChildFromPath(itemCsb, "PropItem/Item/Level")
	local mathLa = CsbTools.getChildFromPath(itemCsb, "PropItem/Item/Num")
	mathLa:setVisible(false)

	if self.mRealShowEuips[index].propConf ~= nil then
		CsbTools.replaceImg(level, getItemLevelSettingItem(self.mRealShowEuips[index].propConf.Quality).ItemFrame)
		CsbTools.replaceImg(icon, self.mRealShowEuips[index].propConf.Icon)
	else
		CsbTools.replaceImg(level, getItemLevelSettingItem(1).ItemFrame)
	end
end

function UIEquipMake:initDowniScrollViewBreak()

	self.mMaterialScrollView:removeAllChildren()

	local size = 0
	for _,_ in pairs(Cailiao) do
		size = size + 1
	end

	if size == 0 then
		self.mMaterialScrollView:setInnerContainerSize(self.mMaterialScrollView:getContentSize())
		return
	end

	local itemPlace = {}
	local hang = 0
	local lie = 8

	for i,id in pairs(self.mXXXX) do
		if id ~=0 then
			if lie == 8 then
				hang = hang + 1
				lie = 1
			else
				lie = lie + 1
			end
			table.insert(itemPlace, {hang = hang, lie = lie, id = id})
		end
	end

	if #itemPlace ==0 then
		return
	end

	local intervalX = 9
	local intervalY = 9
	local offsetX = 6
	local offsetY = 6
	local innerSize = self.mMaterialScrollView:getContentSize()
	--��scrollview��Ĵ�С
	local lie = itemPlace[#itemPlace].lie

	local w = offsetX + lie*self.cailiaoSize.height + (lie + 1)*intervalX
	if w > innerSize.width then
		innerSize.width = w
	end

    if self.mMaterialScrollView:getInnerContainerSize().width ~= innerSize.width then
	    self.mMaterialScrollView:setInnerContainerSize(innerSize)
    end      

	for i=1,#itemPlace do
		local item = nil
		item = ccui.Button:create()
		item:setTouchEnabled(false)
		item:setScale9Enabled(true)
		item:setContentSize(self.cailiaoSize)
		--CsbTools.initButton(item, handler(self, self.itemCallBack))

		local itemCsb = getResManager():cloneCsbNode(ResConfig.UIBag.Csb2.item)--csbFile.MaterialItem)
		itemCsb:setPosition(cc.p(self.cailiaoSize.width/2, self.cailiaoSize.height/2))
		itemCsb:setTag(7758258)
		self:initDownitem(itemCsb, itemPlace[i].id)
		item:addChild(itemCsb)	

		local hang = itemPlace[i].hang
		local lie = itemPlace[i].lie
		local posX = offsetX + (lie - 0.5)* self.cailiaoSize.width + (lie - 1)*intervalX
		local posY = innerSize.height - offsetY - (hang - 0.5)*self.cailiaoSize.height - (hang - 1)*intervalY
		item:setPosition(cc.p(posX,posY))
		item:setTag(i)

		self.mMaterialScrollView:addChild(item)
	end
end

function UIEquipMake:initDownitem(itemCsb, id)

	local icon = CsbTools.getChildFromPath(itemCsb, "Item/icon")
	local mathLa = CsbTools.getChildFromPath(itemCsb, "Item/Num")
	local bg = CsbTools.getChildFromPath(itemCsb, "Item/Level")
	mathLa:setString(Cailiao[id])
	local iconName = getPropConfItem(id)
	CsbTools.replaceImg(icon, iconName.Icon)
	local frame = getItemLevelSettingItem(iconName.Quality).ItemFrame
	CsbTools.replaceImg(bg, frame)
end

function UIEquipMake:itemCallBack(ref)
	local tag = ref:getTag()
	print("tag is", tag)
	local button = self.mPropScrollViewBreak:getChildByTag(tag)
	if button then
		local itemCsb = button:getChildByTag(7758258)
		if itemCsb then
			if not self.mRealShowEuips[tag].isSelected and self.mSelectedCount >= 10 then
				CsbTools.addTipsToRunningScene(CommonHelper.getUIString(1288))
				return
			end

			local SelectImage = CsbTools.getChildFromPath(itemCsb, "SelectImage")
			self.mRealShowEuips[tag].isSelected = not SelectImage:isVisible()
			SelectImage:setVisible(not SelectImage:isVisible())

			self.mBreakMoney = self.mRealShowEuips[tag].isSelected and (self.mBreakMoney + self.mRealShowEuips[tag].breakData.Gold) or 
			(self.mBreakMoney - self.mRealShowEuips[tag].breakData.Gold)
			
			self.mSelectedCount = self.mRealShowEuips[tag].isSelected and (self.mSelectedCount + 1) or (self.mSelectedCount - 1)
			print("self.mSelectedCount", self.mSelectedCount)
			if self.mRealShowEuips[tag].propConf.Quality >2 then  --��Ʒ��
				self.mImportantEquipCount = self.mRealShowEuips[tag].isSelected and (self.mImportantEquipCount + 1) or (self.mImportantEquipCount - 1)
			end
			
		end
	end
	local text = CsbTools.getChildFromPath(self.mBreakButton, "PriceNum")
	text:setString(self.mBreakMoney)
	CommonHelper.playCsbAnimate(self.mBreakButtonFather, breakButton,  self.mSelectedCount > 0 and "Able" or "Disable", false, nil, true)

	-- ���ϼ���
	for i=1,#self.mRealShowEuips[tag].breakData.Decomposit do
		local id = self.mRealShowEuips[tag].breakData.Decomposit[i].Decomposit
		local count = self.mRealShowEuips[tag].breakData.Decomposit[i].DecompositionParam
		print("id,  count", id, count)
		if Cailiao[id] == nil then
			table.insert(Cailiao,id ,count)
		else
			Cailiao[id] = self.mRealShowEuips[tag].isSelected and Cailiao[id] + count or Cailiao[id] - count
			if Cailiao[id] == 0 then
				Cailiao[id] = nil
			end
		end
	end
	print("���Եõ��Ĳ����������������")
	--dump(Cailiao)
	self:reSortBrekCailiao()
	--dump(Cailiao)
	self:initDowniScrollViewBreak()
end

-- ������ѡ ��ְҵ
function UIEquipMake:selectEquip()
	self.mSelectedCount = 0
	self.mBreakMoney = 0
	self.mImportantEquipCount = 0
	self.mRealShowEuips = {}
	Cailiao = {}
	local function job1()
		print("job1ѡ�����",self.mCurJobBreak==1 and CommonHelper.getUIString(513) or CommonHelper.getUIString(jobName[self.mCurJobBreak-1]))
		for _,info in pairs(self.mShowEquips) do
			info.isSelected = false
			if #info.breakData.Vocation~=0 then
				for i=1, #info.breakData.Vocation do
					if info.breakData.Vocation[i]==1 then
						table.insert(self.mRealShowEuips, info)
						break
					end
				end
			end
		end
	end

	local function job2()
		print("job2ѡ�����",self.mCurJobBreak==1 and CommonHelper.getUIString(513) or CommonHelper.getUIString(jobName[self.mCurJobBreak-1]))
		for _,info in pairs(self.mShowEquips) do
			info.isSelected = false
			if #info.breakData.Vocation~=0 then
				for i=1, #info.breakData.Vocation do
					if info.breakData.Vocation[i]==2 then
						table.insert(self.mRealShowEuips, info)
						break
					end
				end
			end
		end
	end

	local function job3()
		print("job3ѡ�����",self.mCurJobBreak==1 and CommonHelper.getUIString(513) or CommonHelper.getUIString(jobName[self.mCurJobBreak-1]))
		for _,info in pairs(self.mShowEquips) do
			info.isSelected = false
			if #info.breakData.Vocation~=0 then
				for i=1, #info.breakData.Vocation do
					if info.breakData.Vocation[i]==3 then
						table.insert(self.mRealShowEuips, info)
						break
					end
				end
			end
		end
	end

	local function job4()
		print("job4ѡ�����",self.mCurJobBreak==1 and CommonHelper.getUIString(513) or CommonHelper.getUIString(jobName[self.mCurJobBreak-1]))
		for _,info in pairs(self.mShowEquips) do
			info.isSelected = false
			if #info.breakData.Vocation~=0 then
				for i=1, #info.breakData.Vocation do
					if info.breakData.Vocation[i]==4 then
						table.insert(self.mRealShowEuips, info)
						break
					end
				end
			end
		end
	end

	local function job5()
		print("job5ѡ�����",self.mCurJobBreak==1 and CommonHelper.getUIString(513) or CommonHelper.getUIString(jobName[self.mCurJobBreak-1]))
		for _,info in pairs(self.mShowEquips) do
			info.isSelected = false
			if #info.breakData.Vocation~=0 then
				for i=1, #info.breakData.Vocation do
					if info.breakData.Vocation[i]==5 then
						table.insert(self.mRealShowEuips, info)
						break
					end
				end
			end
		end
	end

	local function job6()
		for _,info in pairs(self.mShowEquips) do
			info.isSelected = false
		end
		print("job6ѡ�����",self.mCurJobBreak==1 and CommonHelper.getUIString(513) or CommonHelper.getUIString(jobName[self.mCurJobBreak-1]))
		self.mRealShowEuips = self.mShowEquips
	end

	local func = {job1,job2,job3,job4,job5,job6}
	func[self.mCurJobBreak==1 and 6 or self.mCurJobBreak-1]()

	self:reSortEquip()
end

-- �ɷֽ��װ������
function UIEquipMake:reSortEquip()
	-- local function sortIDByLv(info1, info2)
	-- 	if info1.breakData.Level < info2.breakData.Level then
	-- 		return true
	-- 	elseif info1.breakData.Level == info2.breakData.Level then
	-- 		if info1.propConf.Quality < info2.propConf.Quality then
	-- 			return true
	-- 		elseif info1.propConf.Quality == info2.propConf.Quality then
	-- 			if info1.breakData.Parts < info2.breakData.Parts then
	-- 				return true
	-- 			elseif info1.breakData.Parts == info2.breakData.Parts then
	-- 				if info1.equipId < info2.equipId then
	-- 					return true
	-- 				end
	-- 			end
	-- 		end
	-- 	end
	-- 	return false
	-- end

	local function sortIDByLv(info1, info2)
		if info1.propConf.Quality < info2.propConf.Quality then
			return true
		elseif info1.propConf.Quality == info2.propConf.Quality then
			if info1.breakData.Level < info2.breakData.Level then
				return true
			elseif info1.breakData.Level == info2.breakData.Level then
				if info1.breakData.Parts < info2.breakData.Parts then
					return true
				elseif info1.breakData.Parts == info2.breakData.Parts then
					if info1.equipId < info2.equipId then
						return true
					end
				end
			end
		end
		return false
	end

	table.sort(self.mRealShowEuips, sortIDByLv)	
end

-- �ֽ�ɻ�ò��ϵ�����
function UIEquipMake:reSortBrekCailiao()
	local function sortIDByQuality(id1, id2)

		local info1 = getPropConfItem(id1)
		local info2 = getPropConfItem(id2)
		if info1.Quality > info2.Quality then
			return true
		elseif info1.Quality == info2.Quality then
			if Cailiao[id1] > Cailiao[id2] then
				return true
			end
		end
		return false
	end

	self.mXXXX = {}
	for i,_ in pairs(Cailiao) do
		table.insert(self.mXXXX, i)
	end
	table.sort(self.mXXXX, sortIDByQuality)	
	--dump(self.mXXXX)
end

-- �ֽ���ְҵ��
function UIEquipMake:jobListCallBackBreak(ref)
	print("ְҵѡ��")
	if self.mIsJobBreak then
		self.mIsJobBreak = false
		CommonHelper.playCsbAnimate(self.mJobListBreak, downListBufftonFile, "On", false, nil, true)
	else
		self.mIsJobBreak  = true
		CommonHelper.playCsbAnimate(self.mJobListBreak, downListBufftonFile, "Normal", false, nil, true)
	end
end

-- �ֽ���ְҵ����ӽ���,ʲôְҵ��
function UIEquipMake:jobListSonCallBackBreak(ref)
	local ButtonName = CsbTools.getChildFromPath(self.mJobListBreak, "OrderButton/ButtonName")
	local tag = ref:getTag()
	self:jobListCallBackBreak(nil)
	self.mCurJobBreak = tag
	ButtonName:setString(self.mCurJobBreak==1 and CommonHelper.getUIString(513) or CommonHelper.getUIString(jobName[self.mCurJobBreak-1]))
	self:selectEquip()

	CommonHelper.playCsbAnimate(self.mBreakButtonFather, breakButton,  self.mSelectedCount > 0 and "Able" or "Disable", false, nil, true)
	self:initTopScrollViewBreak()
	self:initDowniScrollViewBreak()
end

--------------- ��ť�ص� ----------------------
-- ����ұ�ְҵ,ʲôְҵ��
function UIEquipMake:jobListSonCallBack(ref)

	local tag = ref:getTag()
	local showTag = jobType[tag]
	if showTag == self.mCurJob then
		return
	end

   	for i=1,#jobType  do
   		CommonHelper.playCsbAnimate(self.mJobList[i], AllButton, "Normal", false, nil, true)
   		self.mJobList[i]:setLocalZOrder(i)
   	end

   	self.mCurJob = showTag
	self.mEquipDataIndex = {}
   	self.mEquipDataIndex = equipMakeModel:getEquipByHead(self.mCurJob, levelName[self.mCurLevel])

   	CommonHelper.playCsbAnimate(self.mJobList[tag], AllButton, "On", false, nil, true)
   	self.mJobList[tag]:setLocalZOrder(99999)
   	-- ���, ְҵ����,�ȼ���ҲҪ��һ��
   	for i=1,#levelType do
		self.mLevelRedPoint[i]:setVisible(UIEquipMakeRedHelper.mCanShowLevelRed[self.mCurJob] ~= nil and UIEquipMakeRedHelper.mCanShowLevelRed[self.mCurJob][levelName[i]] ~= nil)
	end
	self.mLevelRed:setVisible(UIEquipMakeRedHelper.mCanShowLevelRed[self.mCurJob] ~= nil)

	self:initLeftScrollView()
	self:initRightUI()
end

-- ����ȼ���
function UIEquipMake:levelListCallBack(ref)
	print("�ȼ�ѡ��")
	if self.mIsLevel then
		self.mIsLevel = false
		CommonHelper.playCsbAnimate(self.mLevelList, downListBufftonFile, "On", false, nil, true)
	else
		self.mIsLevel = true
		CommonHelper.playCsbAnimate(self.mLevelList, downListBufftonFile, "Normal", false, nil, true)
	end
end

-- ����ȼ�����ӽ���,����,ʲô�ȼ���
function UIEquipMake:levelListSonCallBack(ref)
	local ButtonName = CsbTools.getChildFromPath(self.mLevelList, "OrderButton/ButtonName")
	local tag = ref:getTag()
	local showTag = levelType[tag]
	ButtonName:setString(CommonHelper.getUIString(26)..levelName[showTag])
	self:levelListCallBack(nil)
	self.mCurLevel = showTag
	self.mCurParts = 1
	self.mEquipDataIndex = {}
   	self.mEquipDataIndex = equipMakeModel:getEquipByHead(self.mCurJob, levelName[self.mCurLevel])
	self:initLeftScrollView()
	self:initRightUI()
end

-- ���ѡ��װ��
function UIEquipMake:propBarTouchCallBack(ref)
	local index = ref:getTag()
	if index == self.mCurParts then --�ظ��������
		return
	end
	--�ѱ�ı��ûѡ���״̬
	local children = self.mPropScrollView:getChildren()
	for _,node in pairs(children) do
		local itemCsb = node:getChildByTag(7758258)
		CommonHelper.playCsbAnimate(itemCsb, csbFile.PropBar, "Normal", false, nil, true)
	end

	local itemCsb = ref:getChildByTag(7758258)
	CommonHelper.playCsbAnimate(itemCsb, csbFile.PropBar, "Choose", false, nil, true)

	self.mCurParts = index
	self.mCurIndex = self.mEquipDataIndex[index]
	self:initRightUI()
end

function UIEquipMake:checkBoxSelectedEvent(sender,eventType)
    MusicManager.playSoundEffect(sender:getName())
	local equipData = equipMakeModel:getEquipByIndex(self.mEquipDataIndex[self.mCurParts])
	print("eventType", eventType)
    if 0 == eventType then  -- ѡ��
        isQuality = true
        print("����")
        CommonHelper.playCsbAnimate(self.mMakeButton, MakeButton, "Orange", false, nil, true)
    	local needGold = equipData.Eq_NormalCastGoldCost
		self.mPriceNum:setString(needGold)
		self.mTextOrange:setString(CommonHelper.getUIString(1245))
		self.mArrowIcon2:setVisible(true)
		self.mArrowIcon:setVisible(true)
    else                    -- ȡ��
        isQuality = false
        print("������")
        CommonHelper.playCsbAnimate(self.mMakeButton, MakeButton, "Green", false, nil, true)
    	local needGold = equipData.Eq_QualityCastGoldCost
		self.mTextGreen:setString(CommonHelper.getUIString(1244))
		self.mPriceNum:setString(needGold)
		self.mArrowIcon2:setVisible(false)
		self.mArrowIcon:setVisible(false)
    end

    -- if not self.mMoneyenough then
    -- 	CommonHelper.playCsbAnimate(self.mMakeButton, MakeButton, "Grey", false, nil, true)
    -- end

    cc.UserDefault:getInstance():setBoolForKey("isQuality", isQuality)
 end  

-- ����
function UIEquipMake:backBtnCallBack(ref)
	UIManager.close()
end

function UIEquipMake:TouchCallBack(ref)
	-- ����;��
	local tag = ref:getTag()
	local equipData = equipMakeModel:getEquipByIndex(self.mEquipDataIndex[self.mCurParts])
	local id = equipData["Eq_Synthesis"..tag]

	print("tag id is  ", tag,id)
	UIManager.open(UIManager.UI.UIPropQuickTo, id)
end

-- �ұ߱�ǩ�����л�
function UIEquipMake:btnCallBack(ref)
	local tag = ref:getTag()
	self.mIsLevel = true
	self.mIsJobBreak = true
	self.isSuccess = true

	CommonHelper.playCsbAnimate(self.mJobListBreak, downListBufftonFile, "Normal", false, nil, true)
	CommonHelper.playCsbAnimate(self.mLevelList, downListBufftonFile, "Normal", false, nil, true)


	if tag == buttonType.equipBreak then
		self.mMakePanel:setVisible(true)
		self.mBreakPanel:setVisible(false)
		self.mMakeBreakButton:setTag(buttonType.equipMake)
		self.mMakeBreakButtonText:setString(CommonHelper.getUIString(1240))
		self.mMakeIcon:setVisible(true)
		self.mBreakIcon:setVisible(false)
		for i=1,#jobType do
			self.mJobList[i]:setVisible(true)
		end
	elseif tag ==buttonType.equipMake then
		self.mMakePanel:setVisible(false)
		self.mBreakPanel:setVisible(true)
		self.mMakeBreakButton:setTag(buttonType.equipBreak)
		self.mMakeBreakButtonText:setString(CommonHelper.getUIString(1241))
		self.mMakeIcon:setVisible(false)
		self.mBreakIcon:setVisible(true)
		for i=1,#jobType do
			self.mJobList[i]:setVisible(false)
		end
	end
end

function UIEquipMake:viewCallBack(ref)
	local equipInfo = {}
	local equipData = equipMakeModel:getEquipByIndex(self.mEquipDataIndex[self.mCurParts])

	equipInfo.name = CommonHelper.getPropString(equipData.Item_Name)
	equipInfo.level = equipData.Eq_Level..CommonHelper.getUIString(528)
	equipInfo.job = CommonHelper.getUIString(jobName[self.mCurJob])
	equipInfo.equipData = equipData
	UIManager.open(UIManager.UI.UIEquipMakeView, equipInfo, isQuality)
end

function UIEquipMake:questionCallBack(ref)

	UIManager.open(UIManager.UI.UIEquipMakeQuestion)
end

function UIEquipMake:beginEquipMake(ref)
	ref.soundId = nil

	if self.isSuccess then
		
		print("���͵�����Ϊ   �к�,  �Ƿ�Ϊ��������",self.mEquipDataIndex[self.mCurParts], isQuality)
		local equipData = equipMakeModel:getEquipByIndex(self.mEquipDataIndex[self.mCurParts])
		print("�����װ������Ϊ",CommonHelper.getPropString(equipData.Item_Name))

		if not self.mIsCanSend then
			print("ҪôǮ����,Ҫô���ϲ���,С���Ӳ���Ǯ�������װ��?")
            ref.soundId = MusicManager.commonSound.fail
			if not self.mMoneyenough then
                UIManager.open(UIManager.UI.UIGold)
				--CsbTools.addTipsToRunningScene(CommonHelper.getUIString(572))
				return
			end
            --dump(self.mWhichCaliao)
			if table.maxn(self.mWhichCaliao) > 0 then
				CsbTools.addTipsToRunningScene(CommonHelper.getUIString(1250))
				return
			end
			return
		end

		if isQuality then
			if self.mWhichCaliao[5] ~= nil then
				CsbTools.addTipsToRunningScene(CommonHelper.getUIString(1251))
				return
			end
		end
		-- �����ռ��С�ж�
		local bagCount = getGameModel():getBagModel():getItemCount()
		local maxBagCount = getGameModel():getBagModel():getCurCapacity()
		print("������Ʒ��, ���� ���������",bagCount, maxBagCount)
		if bagCount>=maxBagCount then
			CsbTools.addTipsToRunningScene(CommonHelper.getUIString(118))
			return
		end

		print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ���ʹ���װ��Э��")
		local buffData = NetHelper.createBufferData(MainProtocol.Bag, BagProtocol.makeEquipCS)
	    bufferData:writeInt(self.mEquipDataIndex[self.mCurParts])
	    bufferData:writeInt(equipData.Eq_Level)
	    bufferData:writeInt(equipData.Eq_Parts)
	    bufferData:writeInt(equipData.Eq_Vocation)
	    bufferData:writeBool(isQuality)
	   	NetHelper.request(buffData)
	end
end

function UIEquipMake:backGroudCallback(ref)
	
	if not self.isIng then
		return
	end
	self.mForgingText:setVisible(false)
	self.isIng = false
	if self.mMakeTime < self.mMaxTime then
		MusicManager.playSoundEffect(effecMusic[self.mMakeTime])
		CommonHelper.playCsbAnimate(self.mForging, csbFile.Forging, animation[self.mMakeTime], false, function()
		self.mMakeTime = self.mMakeTime + 1
		self.isIng = true
		end, true)
	else
		self:showResult()
	end
end

function UIEquipMake:beginEquipBreakFrist(ref)
	if self.mImportantEquipCount > 0 then
		UIManager.open(UIManager.UI.UIEquipMakeTip, handler(self, self.beginEquipBreak))
	else
		self:beginEquipBreak()
	end
end

function UIEquipMake:beginEquipBreak()
	print("��ʼ�ֽ�")
	print("��ѡ�ĸ���Ϊ ���Ϊ", self.mSelectedCount, self.mBreakMoney)
	print("��Ʒװ���ĸ���Ϊ,", self.mImportantEquipCount)
	--dump(Cailiao)

    CommonHelper.checkConsumeCallback(1, self.mBreakMoney, function ()
	    print("���ͷֽ�Э��")
	    local buffData = NetHelper.createBufferData(MainProtocol.Bag, BagProtocol.breakEquipCS)
        bufferData:writeInt(self.mSelectedCount)

	    for i=1,#self.mRealShowEuips do
		    if self.mRealShowEuips[i].isSelected then
	   		    bufferData:writeInt(self.mRealShowEuips[i].equipId)
			    bufferData:writeInt(self.mRealShowEuips[i].confId)
		    end
	    end

   	    NetHelper.request(buffData)
    end)
end

function UIEquipMake:showResult()

	local propIcon = CsbTools.getChildFromPath(self.mMakeItem, "PropIcon")
	local equipData = equipMakeModel:getEquipByIndex(self.mEquipDataIndex[self.mCurParts])
	CsbTools.replaceSprite(propIcon, equipData.Item_Icon)
	MusicManager.playSoundEffect(effecMusic[self.mMakeTime])
	self.isIng = false
	self.mForging:setVisible(true)
	CommonHelper.playCsbAnimate(self.mForging, csbFile.Forging, animation[self.mMakeTime], false, function()
		self.mForging:setVisible(false)
		CommonHelper.playCsbAnimate(self.mMakeItem, MakeItem, "Made", false, 
		function()
		self.isIng = true

		self.mMaxTime = 1
		self.mMakeTime = 1
			print("self.mMakeMoney=", self.mMakeMoney)
			self:initCommonUI()
			self:initRightUI()

			print("self.mEquipId", self.mEquipId)
			UIManager.open(UIManager.UI.UIShowEquip, self.mEquipId)
			CsbTools.replaceImg(self.mPropImage, equipData.Item_Icon)
			self.mPropImage:setVisible(true)
			self.mIsMakeBack = true --onTopҪ��
			self.mMakeItem:setVisible(false)

			-- �ֽⲿ��Ҫ���³�ʼ��һ��
			self.mShowEquips = {}
			self.mShowEquips = equipMakeModel:getEquipModelCanBreakEquip()
			self:selectEquip()
			self:initTopScrollViewBreak()
			CommonHelper.playCsbAnimate(self.mBreakButtonFather, breakButton,  self.mSelectedCount > 0 and "Able" or "Disable", false, nil, true)
			self:initDowniScrollViewBreak()
		end, true)
	end, true)
end

function UIEquipMake:makeCallBack(mainCmd, subCmd, buffData)
	print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ����������")
	local a = buffData:readInt()
	self.mConfId = buffData:readInt()
	self.mEquipId = buffData:readInt()					
	--local confId = buffData:readInt()					
	self.mMainPropNum = buffData:readChar()				
	self.mEffectIds = {}
	for j = 1, 8 do
		table.insert(self.mEffectIds, buffData:readChar()) 	
	end
	self.mEffectVals = {}
	for j = 1, 8 do
		table.insert(self.mEffectVals, buffData:readShort())	
	end

	self:deleteUserData()
	getGameModel():getEquipModel():addEquip(self.mEquipId, self.mConfId, self.mMainPropNum, self.mEffectIds, self.mEffectVals)
	getGameModel():getBagModel():addItem(self.mEquipId, self.mConfId)

	self.propConf   = getPropConfItem(self.mConfId)
	--dump(self.propConf)
	self.isSuccess = false
	self.mMaxTime = self.propConf.Quality

	-- ����֪ͨ 
	EventManager:raiseEvent(GameEvents.EventEquipMake, {quality = self.mMaxTime})

	if self.mMaxTime == 1 then
		self:showResult()
		return
	end
	self.mForging:setVisible(true)
	self.isIng = false
	MusicManager.playSoundEffect(effecMusic[self.mMakeTime])
	CommonHelper.playCsbAnimate(self.mForging, csbFile.Forging, animation[self.mMakeTime], false, function()
		self.mMakeTime = self.mMakeTime + 1
		self.mForgingText:setVisible(true)
		self.isIng = true
	end, true)

	--���
	UIEquipMakeRedHelper:abcdefg()
	self:initLeftScrollView()
   	for i=1,#levelType do
		self.mLevelRedPoint[i]:setVisible(UIEquipMakeRedHelper.mCanShowLevelRed[self.mCurJob] ~= nil and UIEquipMakeRedHelper.mCanShowLevelRed[self.mCurJob][levelName[i]] ~= nil)
	end
	for i=1,#jobType do
		self.mJobRedPoint[i]:setVisible(UIEquipMakeRedHelper.mCanShowJobRed[jobType[i]] ~= nil)
	end
	self.mPartsRedPoint[self.mCurParts]:setVisible(UIEquipMakeRedHelper.mCanShowPartsRed[self.mCurIndex] ~= nil)
	self.mLevelRed:setVisible(UIEquipMakeRedHelper.mCanShowLevelRed[self.mCurJob] ~= nil)
end

function UIEquipMake:deleteUserData()
	local equipData = equipMakeModel:getEquipByIndex(self.mEquipDataIndex[self.mCurParts])

	local needGold = isQuality and equipData.Eq_QualityCastGoldCost or equipData.Eq_NormalCastGoldCost
	ModelHelper.addCurrency(UIAwardHelper.ResourceID.Gold, -needGold) --�����
	for i=1,4 do
		local needCount = equipData["Eq_Synthesis"..i.."Param"]
		getGameModel():getBagModel():removeItems(equipData["Eq_Synthesis"..i], needCount)
	end
	if isQuality then
		local needCount = equipData["Eq_Synthesis5Param"]
		getGameModel():getBagModel():removeItems(equipData["Eq_Synthesis5"], needCount)
	end
end

function UIEquipMake:breakCallBack(mainCmd, subCmd, buffData)
	print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ �ֽ�������")
	local gold = buffData:readInt()
	local count = buffData:readInt()
	print("gold,count",gold,count)
	local result = {}
	for i=1,count do
		local tmep = {}

		tmep.id = buffData:readInt()
		tmep.num = buffData:readInt()
		table.insert(result, tmep)
	end
	--dump(result)
	if #result then
		ModelHelper.addCurrency(UIAwardHelper.ResourceID.Gold, -gold) --�����

		for i=1,#self.mRealShowEuips do --�Ƴ�װ��
			if self.mRealShowEuips[i].isSelected then
				getGameModel():getEquipModel():removeEquip(self.mRealShowEuips[i].equipId)
				getGameModel():getBagModel():removeItems(self.mRealShowEuips[i].equipId, 1)
			end
		end
		
		--�Ӳ���  --����Լ����ǰ��ģ�ͼ����� ,������
		-- for i=1,#result do
		-- 	getGameModel():getBagModel():addItem(result[i].id, result[i].num)
		-- end
		-- չʾ����
		UIManager.open(UIManager.UI.UIAward, result)--,CommonHelper.getUIString(1286))
		-- ˢ�½���
		self:initCommonUI()
		self.mShowEquips = {}
		self.mShowEquips = equipMakeModel:getEquipModelCanBreakEquip()
		self:selectEquip()
		self:initTopScrollViewBreak()
		CommonHelper.playCsbAnimate(self.mBreakButtonFather, breakButton,  self.mSelectedCount > 0 and "Able" or "Disable", false, nil, true)
		self:initDowniScrollViewBreak()


		-- ��Ҫˢ��һ�´���Ľ���
		self:initCommonUI()
		self:initRightUI()
	end
end

return UIEquipMake 