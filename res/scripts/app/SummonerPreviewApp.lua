require("config")
require("framework.init")
require("framework.shortcodes")
require("framework.cc.init")
require("common.Protocol")
require("common.NetHelper")
require("game.comm.CsbTools")
require("common.ModelHelper")
require("common.CommonHelper")
require("common.IconHelper")
require("game.comm.GlobalListen")
require("Helper")
require("summonerComm.UIConfig")
require("summonerComm.SceneConfig")

local UIManager = require("common.UIManager")
local SceneManager = require("common.SceneManager")

if table.unpack == nil then
    table.unpack = unpack
end

local SummonerPreviewApp = class("SummonerPreviewApp", cc.mvc.AppBase)

function SummonerPreviewApp:ctor()
    SummonerPreviewApp.super.ctor(self) 
    UIManager.init()
    SceneManager.init()
end

function SummonerPreviewApp:run()
    -- �ؿ�����
    local stageId   = 1
    local stageLv   = 1
    local bufferData = newBufferData()
    bufferData:writeInt(stageId)        -- �ؿ�ID
    bufferData:writeInt(stageLv)        -- �ؿ��ȼ�
    bufferData:writeInt(1)              -- ��ս����
    bufferData:writeInt(0)              -- ��չ�ֶ�
    bufferData:writeInt(0)              -- ��չ�ֶ�
    bufferData:writeInt(0)              -- ս����buff�ֶ�
    bufferData:writeInt(1)              -- �������
    
    -- �������
    local userId    = -1
    local userLv    = 1
    local cardCount = 7
    local userName = "Player"
    bufferData:writeInt(userId)         -- ���id
    bufferData:writeInt(userLv)         -- ��ҵȼ�
    bufferData:writeInt(1)              -- �����Ӫ
    bufferData:writeInt(0)              -- buff����
    bufferData:writeInt(cardCount)      -- ʿ������
    bufferData:writeInt(0)              -- Ӷ������
    bufferData:writeInt(0)              -- ��������ʾ(����)
    bufferData:writeString(userName)    --
    for i = string.len(userName) + 2, 32 do
        bufferData:writeChar(0)
    end

    -- �ٻ�ʦ
    local heroId = cc.UserDefault:getInstance():getIntegerForKey("myhero", 1000);
    bufferData:writeInt(heroId)

    -- ʿ���б�
    for i=1, cardCount do
        local cardIdStr     = "mysolider" .. i
        local cardStarStr   = "star" .. i
        local cardTalentStr = "talent" .. i
        local cardLevelStr  = "level" .. i
        local cardId        = cc.UserDefault:getInstance():getIntegerForKey(cardIdStr, 10200)
        local cardStar      = cc.UserDefault:getInstance():getIntegerForKey(cardStarStr, 2)
        local cardTalent    = cc.UserDefault:getInstance():getIntegerForKey(cardTalentStr, 2)
        local cardLevel     = cc.UserDefault:getInstance():getIntegerForKey(cardLevelStr, 1)
        bufferData:writeInt(cardId)     -- ʿ��id
        bufferData:writeInt(cardLevel)  -- ʿ���ȼ�
        bufferData:writeInt(cardStar)   -- ʿ���Ǽ�
        bufferData:writeInt(0)          -- ʿ������

        for j = 1, 8 do
            bufferData:writeChar(0)     -- �츳
        end

        bufferData:writeInt(0)          -- װ������
    end
    
    bufferData:resetOffset()
    -- �򿪷���
    openAndinitRoom(bufferData)
    deleteBufferData(bufferData)
    initUserId(userId)
    -- ���ش���������Դ
    local SceneManager = require("common.SceneManager")
    SceneManager.loadScene(SceneManager.Scene.SceneBattle)
end

return SummonerPreviewApp