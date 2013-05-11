#include "PropertyLayer.h"

PropertyInput::PropertyInput(void)
{
		this->text = NULL;
		this->sprite = NULL;
		this->propertyType = PropertyType_None;
}

PropertyInput::~PropertyInput()
{
}

bool PropertyInput::init()
{
		this->text = CCTextFieldTTF::textFieldWithPlaceHolder("", CCSize(0, 0), CCTextAlignment::kCCTextAlignmentLeft, "Thonburi", 20);
		this->text->setAnchorPoint(cocos2d::CCPointZero);
		this->addChild(this->text);

		return true;
}

CCRect PropertyInput::GetRangeRect()
{
		CCPoint start = this->text->getPosition();
		CCRect rect = CCRect(start.x, start.y, this->inputRect.width, this->inputRect.height);

		return rect;
}

void PropertyInput::OnEnter()
{
		this->text->attachWithIME();
}

void PropertyInput::OnLeave()
{
		this->text->detachWithIME();
}



PropertyLayer::PropertyLayer(void)
{
}


PropertyLayer::~PropertyLayer(void)
{
}

bool PropertyLayer::init()
{
		{
				PropertyInput *input1 = PropertyInput::create();

				//sprite - 1
				input1->sprite = CCSprite::create("Images/PhysicsTech/Prop_Quality.png");
				input1->sprite->setAnchorPoint(cocos2d::CCPointZero);
				input1->sprite->setPosition(ccp(0, 100));
				input1->addChild(input1->sprite);
				//input - sprite - 1
				input1->inputSprite = CCSprite::create("Images/PhysicsTech/QualityInput.png");
				input1->inputSprite->setAnchorPoint(cocos2d::CCPointZero);
				input1->inputSprite->setPosition(ccp(input1->sprite->getPositionX() + 32 + 5, 95));
				input1->addChild(input1->inputSprite);
				//input - 1
				input1->text->setPosition(ccp(input1->sprite->getPositionX() + 32 + 25, 105));
				input1->inputRect = CCSize(100, 20);
				input1->text->setContentSize(input1->inputRect);
				//prop - type - 1
				input1->propertyType = PropertyType_Mass;
				//add
				this->nodeArray->addObject(input1);
				this->addChild(input1);
		}

		{		
				PropertyInput *input2 = PropertyInput::create();
				//sprite - 2
				input2->sprite = CCSprite::create("Images/PhysicsTech/Prop_IsStatic.png");
				input2->sprite->setAnchorPoint(cocos2d::CCPointZero);
				input2->sprite->setPosition(ccp(50, 60));
				input2->addChild(input2->sprite);
				//input - sprite - 2
				input2->inputSprite = CCSprite::create("Images/PhysicsTech/StaticInput.png");
				input2->inputSprite->setAnchorPoint(cocos2d::CCPointZero);
				input2->inputSprite->setPosition(ccp(input2->sprite->getPositionX() + 32 + 5, 55));
				input2->addChild(input2->inputSprite);
				//input - 2
				input2->text->setPosition(ccp(input2->sprite->getPositionX() + 32 + 25, 65));
				input2->inputRect = CCSize(100, 20);
				input2->text->setContentSize(input2->inputRect);
				//prop - type - 1
				input2->propertyType = PropertyType_Static;
				//add
				this->nodeArray->addObject(input2);
				this->addChild(input2);
		}

		this->setVisible(false);

		return true;
}

void PropertyLayer::draw()
{
		//
		// IMPORTANT:
		// This is only for debug purposes
		// It is recommend to disable it
		//
		CCLayer::draw();

		ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

		kmGLPushMatrix();

		world->DrawDebugData();

		kmGLPopMatrix();

		//TODO : draw something here.

		/*
		//draw inputs
		if (this->nodeArray == NULL)
		{
				return;
		}

		glLineWidth(2);
		ccDrawColor4B(124,1,1,128);

		int count = this->nodeArray->count();
		for (int i = 0; i < count; i++)
		{
				PropertyInput *input = (PropertyInput*)this->nodeArray->objectAtIndex(i);
				if (input->isVisible() == false)
				{
						continue;
				}

				CCPoint start = input->text->getPosition();
				CCPoint end = ccp(start.x + input->inputRect.width, start.y + input->inputRect.height);
				cocos2d::ccDrawRect(start ,end);
		}
		*/
}

void PropertyLayer::SetProperty(PropertyType type, void* data)
{
		PropertyInput *found = NULL;

		//fiind the corresoding property type
		int count = this->nodeArray->count();
		for (int i = 0; i < count; i++)
		{
				PropertyInput *input = (PropertyInput*)this->nodeArray->objectAtIndex(i);
				if (input->propertyType == type)
				{
						found = input;
						break;
				}
		}

		//if no found, return
		if (found == NULL)
		{
				return;
		}

		switch (type)
		{
		case PropertyType_Static:
				{
						bool *b = (bool*)data;
						bool isStatic = *b;
						if (isStatic)
						{
								found->text->setString("true");
						}
						else
						{
								found->text->setString("false");
						}
				}
				break;
		case PropertyType_Mass:
				{
						float *f = (float*)data;
						float mass = *f;
						char s[16];
						sprintf(s, "%f", mass);
						found->text->setString(s);
				}
				break;
		default:
				break;
		}
}

