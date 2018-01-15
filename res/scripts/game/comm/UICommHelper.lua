UICommHelper = {}

function UICommHelper.setHeroCardInfo(cardNode, heroId, heroStar, heroLv)
    --���Ӣ������
	local heroConfig = getSoldierConfItem(heroId, heroStar)
	if heroConfig == nil then
		print("can't found hero config, hero not exsit!")
		return
	end
    local frameAniName = {"White", "Green", "Blue", "Voilet", "Yellow", "Orange", "Colorful" }
    local bigRaceImg = {"card_golden_race_human.png", "card_golden_race_dead.png", "card_golden_race_nature.png"}
    -- �ڵ��Ƿ�����
    local cardImageNode = CsbTools.getChildFromPath(cardNode, "CardPanel/Front/SummonerImage")
	local cardLvImageNode = CsbTools.getChildFromPath(cardNode, "CardPanel/Front/LvImage")
	local cardNameLabel = CsbTools.getChildFromPath(cardNode, "CardPanel/Front/NameLabel")
	local cardLvLabel = CsbTools.getChildFromPath(cardNode, "CardPanel/Front/LvLabel")
    local cardLvBar = CsbTools.getChildFromPath(cardNode, "CardPanel/Front/LvBar")
    --local cardStarLvLabel = CsbTools.getChildFromPath(cardNode, "CardPanel/Front/StarLv")
    local cardStar = CsbTools.getChildFromPath(cardNode, "CardPanel/Front/Star")
    local cardVocBg = CsbTools.getChildFromPath(cardNode, "CardPanel/Front/Herobar_Star")
    local cardCrystal = CsbTools.getChildFromPath(cardNode, "CardPanel/Front/GemSum")
    local cardProfesion = CsbTools.getChildFromPath(cardNode, "CardPanel/Front/Profesion")
    local cardRace = CsbTools.getChildFromPath(cardNode, "CardPanel/Front/Race")
    local backCardRace = CsbTools.getChildFromPath(cardNode, "CardPanel/Back/CardRaceImage")

    local cardLightB = CsbTools.getChildFromPath(cardNode, "CardPanel/Front/CardLight_B")
    local cardLightF = CsbTools.getChildFromPath(cardNode, "CardPanel/Front/CardLight_F")

    if cardImageNode == nil or cardLvImageNode == nil 
        or cardNameLabel == nil or cardLvLabel == nil 
        or cardLvBar == nil or cardStar == nil
        or cardCrystal == nil or cardProfesion == nil
        or cardRace == nil or cardVocBg == nil then
        return
    end

    -- �Ǽ�����
    local cardStarAct = cardStar:getActionByTag(1111)
    if cardStarAct == nil then
        cardStarAct = cc.CSLoader:createTimeline("ui_new/g_gamehall/c_card/HeroStar_S.csb")
        cardStar:runAction(cardStarAct)
        cardStarAct:setTag(1111)
    end
    
    -- Ӣ������
	local heroName = CommonHelper.getHSString(heroConfig.Common.Name)
	local cardLightB = CsbTools.getChildFromPath(cardNode, "CardPanel/Front/CardLight_B")
	local cardLightF = CsbTools.getChildFromPath(cardNode, "CardPanel/Front/CardLight_F")
	CommonHelper.playCsbAnimate(cardLightB, lightB, frameAniName[heroConfig.Rare], true, nil, true)
	CommonHelper.playCsbAnimate(cardLightF, lightF, frameAniName[heroConfig.Rare], true, nil, true)

    --����ͼƬ
    CsbTools.replaceImg(cardImageNode, heroConfig.Common.Picture)
    CsbTools.replaceImg(cardLvImageNode, IconHelper.getSoldierBigHeadFrame(heroConfig.Rare))
    CsbTools.replaceImg(cardRace, IconHelper.getRaceIcon(heroConfig.Common.Race))
    CsbTools.replaceImg(cardLvBar, IconHelper.getCardLevelBorder(heroConfig.Rare))
    CsbTools.replaceImg(backCardRace, bigRaceImg[heroConfig.Common.Race] or "card_golden_race_human.png")
    CsbTools.replaceImg(cardVocBg, IconHelper.getSoldierJobIconCircle(heroConfig.Rare))
    CsbTools.replaceSprite(cardProfesion, IconHelper.getSoldierJobIcon(heroConfig.Rare, heroConfig.Common.Vocation))
    --��������
	cardNameLabel:setString(heroName)
	--�������Ǹ���
    cardStarAct:play("Star"..heroStar, true)
	--���õȼ�
	cardLvLabel:setString(heroLv)
    --����ˮ������
    cardCrystal:setString(heroConfig.Cost)
end

