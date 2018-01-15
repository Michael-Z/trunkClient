#ifndef __SIMPLE_SHADER_H__
#define __SIMPLE_SHADER_H__

#include "cocos2d.h"

class CSimpleShader
{
public:
    // h s v Ϊ�������ͼ�����
    static cocos2d::GLProgramState* applyHSVShader(cocos2d::Node* node, const cocos2d::Vec3& hsv);

    // h s v Ϊ�������ͼ�����
    static void applyHSVShaderNoMVP(cocos2d::Node* node, const cocos2d::Vec3& hsv);

    // ����ɫ��
    static cocos2d::GLProgramState* applyHueShader(cocos2d::Node* node, const float hue);

    // Ӧ�ûһ�
    static void applyGray(cocos2d::Node* node);
    // ȥ���һ�
    static void removeGray(cocos2d::Node* node);
};

#endif
