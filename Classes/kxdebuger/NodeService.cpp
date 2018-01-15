#include "NodeService.h"
#include "message.pb.h"
#include "KxDebugerProtocol.h"
#include "KxCSComm.h"
#include "GameComm.h"
#include "Role.h"
#include "ArmatureComponent.h"
#include <spine/spine-cocos2dx.h>

namespace kxdebuger {

//�����������
enum NodeAttributeType
{
	NAT_ANCHOR,				    //ê��
	NAT_POSITION,				//λ��
	NAT_SIZE,					//�ߴ�

	NAT_VISIBLE,				//
	NAT_TOUCH,					//
	NAT_TAG,					//
	NAT_SCALE,				    //
	NAT_ROTATION,				//
	NAT_TILT,					//
	NAT_OPACITY,				//
	NAT_COLOR,				    //
	NAT_FLIP_H,					//
	NAT_FLIP_V,					//

	NAT_RES,					//

	NAT_TEXT,					//
	NAT_TEXT_COLOR,			    //
	NAT_TEXT_FONT_SIZE,			//
	NAT_TEXT_FONT_NAME,			//

	NAT_TEXT_ALIGN_H,			//
	NAT_TEXT_ALIGN_V,			//

	NAT_CLIP,					//
	NAT_BG_COLOR_TYPE,			//
	NAT_BG_COLOR,				//
	NAT_BG_OPACITY,				//

	NAT_BOUNCE,					//
	NAT_SCROLL_DIR,				//
	NAT_SCROLL_SIZE,			//
};

//��ֵ����
enum NodeValueType
{
    NVT_BOOL,
    NVT_INT,
    NVT_FLOAT,
    NVT_STRING,
    NVT_OPACITY,
    NVT_COLOR,
    NVT_PAIR,
};

//�ϸ����
Node* lastNode = nullptr;
static const int DrawNodeTag = 780256;

template<typename T>
inline std::string ToPairString(T x, T y)
{
    return toolToStr(x) + "," + toolToStr(y);
}

inline std::string ToColorString(int r, int g, int b)
{
    return toolToStr(r) + "," + toolToStr(g) + "," + toolToStr(b);
}

//���������
bool toNodeTree(Node* node, NodeTree* tree)
{
    CHECK_RETURN(node);
    CHECK_RETURN(tree);
    int* nodeId = reinterpret_cast<int*>(node);
    char buf[32];
    memset(buf, 0, sizeof(buf));
    snprintf(buf, sizeof(buf), "%d", node);
    tree->set_nodeid(atoi(buf));
    //LOG("tree node ID: %d", (::google::protobuf::int32)node);
    tree->set_name(node->getName());
    // ����ϵͳ��������ϸ���ӽڵ��б�
    if (dynamic_cast<ParticleSystem*>(node) == nullptr
        && dynamic_cast<Armature*>(node) == nullptr)
    {
        for (auto item : node->getChildren())
        {
            NodeTree* child = tree->add_children();
            toNodeTree(item, child);
        }
    }

	return true;
}

//�ҽ��
bool findNode(Node* d, Node* n)
{
    CHECK_RETURN(d);
    CHECK_RETURN(n);
    if (d == n)
    {
        return true;
    }

	for (auto item : d->getChildren())
	{
        if (findNode(item, n))
        {
            return true;
        }
	}

	return false;
}

//�����Ϣ
void fill(InfoGroup* group, int type, int valuetype, const std::string& value)
{
	Info* info = group->add_attributes();
	info->set_attributetype((::google::protobuf::int32)type);
	info->set_valuetype((::google::protobuf::int32)valuetype);
	info->set_value(value);
}

//��������Ϣ
void fillBase(NodeInfo* ni, Node* node)
{
	InfoGroup* group = ni->add_attributes();
	group->set_groupname("Base");
    fill(group, NAT_ANCHOR, NVT_PAIR, ToPairString(node->getAnchorPoint().x, node->getAnchorPoint().y));
    fill(group, NAT_POSITION, NVT_PAIR, ToPairString(node->getPositionX(), node->getPositionY()));
    fill(group, NAT_SIZE, NVT_PAIR, ToPairString(node->getContentSize().width, node->getContentSize().height));
}

//���һ����Ϣ
void fillCommon(NodeInfo* ni, Node* node)
{
	InfoGroup* group = ni->add_attributes();
	group->set_groupname("Common");
	fill(group, NAT_VISIBLE,	NVT_BOOL,	 toolToStr(node->isVisible()));
	fill(group, NAT_TAG,		NVT_INT,	 toolToStr(node->getTag()));
    fill(group, NAT_SCALE,      NVT_PAIR,    ToPairString(node->getScaleX(), node->getScaleY()));
	fill(group, NAT_ROTATION,	NVT_FLOAT,	 toolToStr(node->getRotation()));
    fill(group, NAT_TILT,       NVT_PAIR,    ToPairString(node->getSkewX(), node->getSkewY()));
    fill(group, NAT_OPACITY,    NVT_OPACITY, toolToStr((int)(node->getOpacity() * 100 / 255)));
    fill(group, NAT_COLOR,      NVT_COLOR,   ToColorString(node->getColor().r, node->getColor().g, node->getColor().b));
	Widget* w = dynamic_cast<Widget*>(node);
	CHECK_RETURN_VOID(w);
    fill(group, NAT_TOUCH,      NVT_BOOL,    toolToStr(w->isTouchEnabled()));
	fill(group, NAT_FLIP_H,     NVT_BOOL,	 toolToStr(w->isFlippedX()));
	fill(group, NAT_FLIP_V,     NVT_BOOL,	 toolToStr(w->isFlippedY()));
}

//�������
void fillFeature(NodeInfo* ni, Node* node)
{
	Widget* w = dynamic_cast<Widget*>(node);
	CHECK_RETURN_VOID(w);
	InfoGroup* group = ni->add_attributes();
	group->set_groupname("Feature");
	ImageView* image = dynamic_cast<ImageView*>(w);
	if (image)
	{	
		fill(group, NAT_RES, NVT_STRING, image->getDescription());    
		return;
	}
	Button* button = dynamic_cast<Button*>(w);
	if (button)
	{
		std::string str = button->getTitleText();
		fill(group, NAT_TEXT, NVT_STRING, str);
		str = button->getTitleFontName();
		fill(group, NAT_TEXT_FONT_NAME, NVT_STRING, str);
		fill(group, NAT_TEXT_FONT_SIZE, NVT_FLOAT, toolToStr(button->getTitleFontSize()));
        fill(group, NAT_TEXT_COLOR, NVT_COLOR, ToColorString(button->getTitleColor().r, button->getTitleColor().g, button->getTitleColor().b));
		return;		
	}
	Text* text = dynamic_cast<Text*>(w);
	if (text)
	{
		std::string str = text->getString();
		fill(group, NAT_TEXT, NVT_STRING, str);
		str = text->getFontName();
		fill(group, NAT_TEXT_FONT_NAME, NVT_STRING, str);
		fill(group, NAT_TEXT_FONT_SIZE, NVT_FLOAT, toolToStr(text->getFontSize()));
        fill(group, NAT_TEXT_COLOR, NVT_COLOR, ToColorString(text->getTextColor().r, text->getTextColor().g, text->getTextColor().b));
		fill(group, NAT_TEXT_ALIGN_H, NVT_INT, toolToStr((int)text->getTextHorizontalAlignment()));
		fill(group, NAT_TEXT_ALIGN_V, NVT_INT, toolToStr((int)text->getTextVerticalAlignment()));
		return;
	}
	PageView* page = dynamic_cast<PageView*>(w);
	if (page)
	{
		fill(group, NAT_CLIP, NVT_BOOL, toolToStr(page->isClippingEnabled()));
        fill(group, NAT_BG_OPACITY, NVT_OPACITY, toolToStr((int)page->getBackGroundColorOpacity()));
		fill(group, NAT_BG_COLOR_TYPE, NVT_INT, toolToStr((int)page->getBackGroundColorType()));
        fill(group, NAT_BG_COLOR, NVT_COLOR, ToColorString((int)page->getBackGroundColor().r, (int)page->getBackGroundColor().g, (int)page->getBackGroundColor().b));
		return;
	}
	ScrollView* scroll = dynamic_cast<ScrollView*>(w);
	if (scroll)
	{
		fill(group, NAT_CLIP, NVT_BOOL, toolToStr(scroll->isClippingEnabled()));
		fill(group, NAT_BOUNCE, NVT_BOOL, toolToStr(scroll->isBounceEnabled()));
        fill(group, NAT_SCROLL_SIZE, NVT_PAIR, ToPairString(scroll->getInnerContainerSize().width, scroll->getInnerContainerSize().height));
        fill(group, NAT_BG_OPACITY, NVT_OPACITY, toolToStr((int)scroll->getBackGroundColorOpacity()));
		fill(group, NAT_BG_COLOR_TYPE, NVT_INT, toolToStr((int)scroll->getBackGroundColorType()));
        fill(group, NAT_BG_COLOR, NVT_COLOR, ToColorString((int)scroll->getBackGroundColor().r, (int)scroll->getBackGroundColor().g, (int)scroll->getBackGroundColor().g));
		return;
	}
	Layout* lay = dynamic_cast<Layout*>(w);
	if (lay)
	{
		fill(group, NAT_CLIP, NVT_BOOL, toolToStr(lay->isClippingEnabled()));
        fill(group, NAT_BG_OPACITY, NVT_OPACITY, toolToStr((int)lay->getBackGroundColorOpacity()));
		fill(group, NAT_BG_COLOR_TYPE, NVT_INT, toolToStr((int)lay->getBackGroundColorType()));
        fill(group, NAT_BG_COLOR, NVT_COLOR, ToColorString((int)lay->getBackGroundColor().r, (int)lay->getBackGroundColor().g, (int)lay->getBackGroundColor().b));
		return;
	}
}

//�޸Ļ�����Ϣ
void modifyBase(const InfoGroup& group, Node* node)
{
	for (int j = 0; j < group.attributes_size(); j++)
	{
		const Info& info = group.attributes(j);
		auto type = info.attributetype();
		auto value = info.value();

        std::vector<std::string> valueSep;
        CConfAnalytic::StringSplit(value, valueSep, ",");
        if (valueSep.size() != 2)
        {
            return;
        }

		switch (type)
		{
		case NAT_ANCHOR:
            node->setAnchorPoint(Vec2(utils::atof(valueSep[0].c_str()), utils::atof(valueSep[1].c_str())));
			break;
		case NAT_POSITION:
            node->setPosition(Vec2(utils::atof(valueSep[0].c_str()), utils::atof(valueSep[1].c_str())));
			break;
		case NAT_SIZE:
            node->setContentSize(Size(utils::atof(valueSep[0].c_str()), utils::atof(valueSep[1].c_str())));
			break;
		default:
			break;
		}
	}
}

//�޸�һ����Ϣ
void modifyCommon(const InfoGroup& group, Node* node)
{
	auto w = dynamic_cast<Widget*>(node);
	for (int j = 0; j < group.attributes_size(); j++)
	{
		const Info& info = group.attributes(j);
		auto type = info.attributetype();
		auto vtype = info.valuetype();
		auto value = info.value();
		switch (type)
		{
		case NAT_VISIBLE:
			node->setVisible(value != "0");
			break;
		case NAT_TOUCH:
            w->setTouchEnabled(value != "0");
			break;
		case NAT_TAG:
			node->setTag(atoi(value.c_str()));
			break;
        case NAT_SCALE:
        {
            std::vector<std::string> valueSep;
            CConfAnalytic::StringSplit(value, valueSep, ",");
            if (valueSep.size() != 2)
            {
                return;
            }
            node->setScaleX(utils::atof(valueSep[0].c_str()));
            node->setScaleY(utils::atof(valueSep[1].c_str()));
        }
            break;
		case NAT_ROTATION:
			node->setRotation(utils::atof(value.c_str()));
			break;
        case NAT_TILT:
        {
            std::vector<std::string> valueSep;
            CConfAnalytic::StringSplit(value, valueSep, ",");
            if (valueSep.size() != 2)
            {
                return;
            }

            node->setSkewX(utils::atof(valueSep[0].c_str()));
            node->setSkewY(utils::atof(valueSep[1].c_str()));
        }
			break;
		case NAT_OPACITY:
            node->setOpacity((atoi(value.c_str()) / 100) * 255);
			break;
		case NAT_COLOR:
			//node->setColor(Color3B(atoi(value.c_str()), node->getColor().g, node->getColor().b));
			break;
		case NAT_FLIP_H:
            w->setFlippedX(value != "0");
            break;
		case NAT_FLIP_V:
            w->setFlippedY(value != "0");
            break;
		default:
			break;
		}
	}
}

//�޸�����
void modifyFeature(const InfoGroup& group, Node* node)
{
    Widget* w = dynamic_cast<Widget*>(node);
    CHECK_RETURN_VOID(w);
	for (int j = 0; j < group.attributes_size(); j++)
	{
		const Info& info = group.attributes(j);
		auto type = info.attributetype();
		auto vtype = info.valuetype();
		auto value = info.value();
		switch (type)
		{
        case NAT_RES:
        {
            ImageView* image = dynamic_cast<ImageView*>(w);
            if (image)
            {
                //image->
            }
        }
			break;
        case NAT_TEXT:
        {
            Button* button = dynamic_cast<Button*>(w);
            if (button)
            {
                button->setTitleText(value);
                return;
            }
            Text* text = dynamic_cast<Text*>(w);
            if (text)
            {
                text->setString(value);
                return;
            }
        }
            break;
        case NAT_TEXT_COLOR:
        {
            std::vector<std::string> valueSep;
            CConfAnalytic::StringSplit(value, valueSep, ",");
            if (valueSep.size() != 3)
            {
                return;
            }
            Color3B color = Color3B(atoi(valueSep[0].c_str()), atoi(valueSep[1].c_str()), atoi(valueSep[2].c_str()));
            Button* button = dynamic_cast<Button*>(w);
            if (button)
            {
                button->setTitleColor(color);
                return;
            }
            Text* text = dynamic_cast<Text*>(w);
            if (text)
            {
                text->setColor(color);
                return;
            }
        }
            break;
        case NAT_TEXT_FONT_SIZE:
        {
            Button* button = dynamic_cast<Button*>(w);
            if (button)
            {
                button->setTitleFontSize(utils::atof(value.c_str()));
                return;
            }
            Text* text = dynamic_cast<Text*>(w);
            if (text)
            {
                text->setFontSize(atoi(value.c_str()));
                return;
            }
        }
            break;
        case NAT_TEXT_FONT_NAME:
        {
            Button* button = dynamic_cast<Button*>(w);
            if (button)
            {
                button->setTitleFontName(value);
                return;
            }
            Text* text = dynamic_cast<Text*>(w);
            if (text)
            {
                text->setFontName(value);
                return;
            }
        }
            break;
        
        case NAT_TEXT_ALIGN_H:
        {
            Text* text = dynamic_cast<Text*>(w);
            if (text)
            {
                text->setTextHorizontalAlignment((TextHAlignment)(atoi(value.c_str())));
            }
        }
            break;
        case NAT_TEXT_ALIGN_V:
        {
            Text* text = dynamic_cast<Text*>(w);
            if (text)
            {
                text->setTextVerticalAlignment((TextVAlignment)(atoi(value.c_str())));
            }
        }
            break;

        case NAT_CLIP:
        {
            PageView* page = dynamic_cast<PageView*>(w);
            if (page)
            {
                page->setClippingEnabled(value != "0");
                return;
            }
            ScrollView* scroll = dynamic_cast<ScrollView*>(w);
            if (scroll)
            {
                scroll->setClippingEnabled(value != "0");
                return;
            }
            Layout* lay = dynamic_cast<Layout*>(w);
            if (lay)
            {
                lay->setClippingEnabled(value != "0");
                return;
            }
        }
            break;
        case NAT_BG_COLOR_TYPE:
        {
            PageView* page = dynamic_cast<PageView*>(w);
            if (page)
            {
                page->setBackGroundColorType((ui::Layout::BackGroundColorType)atoi(value.c_str()));
                return;
            }
            ScrollView* scroll = dynamic_cast<ScrollView*>(w);
            if (scroll)
            {
                scroll->setBackGroundColorType((ui::Layout::BackGroundColorType)atoi(value.c_str()));
                return;
            }
            Layout* lay = dynamic_cast<Layout*>(w);
            if (lay)
            {
                lay->setBackGroundColorType((ui::Layout::BackGroundColorType)atoi(value.c_str()));
                return;
            }
        }
            break;
        case NAT_BG_COLOR:
        {
            std::vector<std::string> valueSep;
            CConfAnalytic::StringSplit(value, valueSep, ",");
            if (valueSep.size() != 3)
            {
                return;
            }
            Color3B color = Color3B(atoi(valueSep[0].c_str()), atoi(valueSep[1].c_str()), atoi(valueSep[2].c_str()));
            PageView* page = dynamic_cast<PageView*>(w);
            if (page)
            {
                page->setBackGroundColor(color);
                return;
            }
            ScrollView* scroll = dynamic_cast<ScrollView*>(w);
            if (scroll)
            {
                scroll->setBackGroundColor(color);
                return;
            }
            Layout* lay = dynamic_cast<Layout*>(w);
            if (lay)
            {
                lay->setBackGroundColor(color);
                return;
            }
        }
            break;
        case NAT_BG_OPACITY:
        {
            
            PageView* page = dynamic_cast<PageView*>(w);
            if (page)
            {
                page->setBackGroundColorOpacity(atoi(value.c_str()));
                return;
            }
            ScrollView* scroll = dynamic_cast<ScrollView*>(w);
            if (scroll)
            {
                scroll->setBackGroundColorOpacity(atoi(value.c_str()));
                return;
            }
            Layout* lay = dynamic_cast<Layout*>(w);
            if (lay)
            {
                lay->setBackGroundColorOpacity(atoi(value.c_str()));
                return;
            }
        }
            break;

        case NAT_BOUNCE:
            ScrollView* scroll = dynamic_cast<ScrollView*>(w);
            if (scroll)
            {
                scroll->setBounceEnabled(value != "0");
            }
            break;
        //case NAT_SCROLL_DIR:
        //    break;
		}
	}
}

//�޸Ľ��
void modifyNode(NodeInfo* nodeInfo)
{
	Node* node = (Node*)nodeInfo->nodeid();
	for (int i = 0; i < nodeInfo->attributes_size(); i++)
	{
		const InfoGroup& group = nodeInfo->attributes(i);
		std::string str = group.groupname();
		if (!strcmp(str.c_str(), "Base"))
		{
			modifyBase(group, node);
		}
		else if (!strcmp(str.c_str(), "Common"))
		{
			modifyCommon(group, node);
		}
		else if (!strcmp(str.c_str(), "Feature"))
		{
			modifyFeature(group, node);
		}
	}
}

NodeService::NodeService()
{

}

NodeService::~NodeService()
{

}

void NodeService::process(int actionId, void* data, int len, KxServer::IKxComm *target)
{
	auto s = Director::getInstance()->getRunningScene();
	switch (actionId)
	{
	case ActionQueryNodeTree:
		{
             NodeTree* tree = new NodeTree();
             if (!toNodeTree(s, tree))
             {
                 return;
             }
             
			 std::string str;
			 tree->SerializeToString(&str);
             
             sendData(ServiceNode, ActionQueryNodeTree, str.c_str(), str.length(), target);
		}
		break;
	case ActionQueryNodeInfo:
		{
            // ����NodeId
            NodeId nodeID;
            if (nodeID.ParseFromArray(data, len))
            {
                Node *node = reinterpret_cast<Node*>(nodeID.nodeid());
                bool b = findNode(s, node);
                CHECK_RETURN_VOID(b);

                NodeInfo* nodeInfo = new NodeInfo();
                nodeInfo->set_nodeid(nodeID.nodeid());
                fillBase(nodeInfo, node);
                fillCommon(nodeInfo, node);
                fillFeature(nodeInfo, node);

                std::string str;
                nodeInfo->SerializeToString(&str);

                sendData(ServiceNode, ActionQueryNodeInfo, str.c_str(), str.length(), target);
            }
		}
		break;
	case ActionModifyNodeInfo:
		{
            // ����NodeInfo
			NodeInfo info;
            if (info.ParseFromArray(data, len))
            {
                Node* node = (Node*)info.nodeid();
                bool b = findNode(s, node);
                CHECK_RETURN_VOID(b);

                //�����޸���
                modifyNode(&info);

                // �޷�����Ϣ
            }
		}
		break;
	case ActionActiveNode:
		{
            // ����NodeId
            NodeId nodeID;
            if (nodeID.ParseFromArray(data, len))
            {
                Node *node = reinterpret_cast<Node*>(nodeID.nodeid());
                bool b = findNode(s, node);
                CHECK_RETURN_VOID(b);
                CHECK_RETURN_VOID(dynamic_cast<Menu*>(node) == nullptr);

                //�����һ���ڵ㻹����
                if (lastNode && findNode(s, lastNode))
                {
                    CHECK_RETURN_VOID(node != lastNode);	//�����ظ�����
                    //����У�ɾ��
                    auto child = lastNode->getChildByTag(DrawNodeTag);
                    if (child)
                    {
                        child->removeFromParentAndCleanup(true);
                    }
                }

                //���û�У����
                DrawNode* draw = DrawNode::create();
                draw->setGlobalZOrder(100);
                draw->setTag(DrawNodeTag);
                node->addChild(draw);
                Size content = node->getContentSize();
                Color4F color = Color4F(1.0, 1.0, 0.0, 1.0);
                CRole *pRole = dynamic_cast<CRole*>(node);
                if (pRole)
                {
                    CAnimateComponent* ani = dynamic_cast<CAnimateComponent*>(pRole->getComponent("MainAnimate"));
                    if (ani)
                    {
                        Node* mainAni = ani->getMainAnimate();
                        if (mainAni)
                        {
                            Armature *pArmature = dynamic_cast<Armature*>(mainAni);
                            if (pArmature)
                            {
                                content = pArmature->getBoundingBox().size;
                            }
                            else
                            {
                                spine::SkeletonAnimation *pSkeletonAnimation = dynamic_cast<spine::SkeletonAnimation*>(mainAni);
                                content = pSkeletonAnimation->getBoundingBox().size;
                            }

                            draw->drawRect(Vec2(-content.width/2, 0), Vec2(content.width/2, content.height), color);
                        }
                    }
                }
                else
                {
                    draw->drawRect(Vec2::ZERO, Vec2(content.width, content.height), color);
                }
                
                lastNode = node;

                // �޷�����Ϣ
            }
		}
		break;
	case ActionRemoveNode:
		{
			// ����NodeId
            NodeId nodeID;
            if (nodeID.ParseFromArray(data, len))
            {
                Node *node = reinterpret_cast<Node*>(nodeID.nodeid());
                bool b = findNode(s, node);
                CHECK_RETURN_VOID(b);

                node->removeFromParentAndCleanup(true);

                // �޷�����Ϣ
            }
		}
		break;
	default:
		break;
	}
}

}