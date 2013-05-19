#include "PicShowLayer.h"

PicShowLayer::PicShowLayer(void)
{
		this->currentSelect = NULL;
		this->elementArr = NULL;
}

PicShowLayer::~PicShowLayer(void)
{
		if (this->elementArr != NULL)
		{
				delete this->elementArr;
		}
}

void PicShowLayer::update(float dt)
{
		//It is recommended that a fixed time step is used with Box2D for stability
		//of the simulation, however, we are using a variable time step here.
		//You need to make an informed choice, the following URL is useful
		//http://gafferongames.com/game-physics/fix-your-timestep/

		int velocityIterations = 8;
		int positionIterations = 1;

		// Instruct the world to perform a single step of simulation. It is
		// generally best to keep the time step and iterations fixed.
		//world->Step(dt, velocityIterations, positionIterations);
}

void PicShowLayer::draw()
{
		//
		// IMPORTANT:
		// This is only for debug purposes
		// It is recommend to disable it
		//
		CCLayer::draw();

		ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

		kmGLPushMatrix();

		kmGLPopMatrix();

		//debug
		CCSize size = this->getContentSize();
		ccDrawRect(ccp(0,0), ccp(size.width, size.height));

		//debug
		/*
		for (int i = 0; i < this->elementArr->count(); i++)
		{
				CCSprite *s = (CCSprite*)this->elementArr->objectAtIndex(i);
				CCRect rect = s->boundingBox();
				ccDrawRect(ccp(s->getPosition().x, s->getPosition().y), ccp(rect.getMaxX(), rect.getMaxY()));
		}
		*/
}

PicShowLayer* PicShowLayer::create(int startIndex)
{
	//CCFileUtils *fileUtil = CCFileUtils::sharedFileUtils();
	//std::vector<std::string> aa = fileUtil->getSearchResolutionsOrder();
	//std::vector<std::string> searchPaths = fileUtil->getSearchPaths();
	
		PicShowLayer* layer = new PicShowLayer();
		layer->startIndex = startIndex;
		layer->autorelease();

		layer->init();

		return layer;
}

bool PicShowLayer::init()
{
		this->elementArr = new CCArray();

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		int xInterval = 10;
		int xMargin = 20;
		int yInterval = 10;
		int yMargin = 20;

		this->setContentSize(CCSize(winSize.width - 30 - 260, winSize.height - 30 - 130));
		CCSize size = this->getContentSize();

		CCSize contentSize = CommonHelper::CountContainerNumber(size, Size_N2, xInterval, xMargin,  Size_N2, yInterval, yMargin);
		int xNum = (int)contentSize.width;
		int yNum = (int)contentSize.height;
		int x = xMargin;
		int y = this->getContentSize().height - Size_N2 - yMargin;
		int yCount = 0;
		// create icons
		char path[255] = {0};
		int count = 0;
		for (int i = this->startIndex + 1; i <= Pic_Count; i++)
		{
				count++;

				sprintf(path, "Images/PicPickup/Icons-N2/%d.png", i);
				CCSprite *s = CCSprite::create(path);
				s->setPosition(ccp(x, y));
				s->setAnchorPoint(CCPointZero);
				CCSize size = s->getContentSize();
				float more = size.width > size.height ? size.width : size.height;
				float scale = Size_N2 / more;
				s->setScale(scale);
				s->setContentSize(CCSize(Size_N2, Size_N2));
				this->addChild(s);
				this->elementArr->addObject(s);

				x += xInterval + Size_N2;

				if (i % xNum == 0)
				{
						//reset - x
						x = xMargin;
						//calc y
						y -= (yInterval + Size_N2);
						yCount++;

						if (yCount >= yNum)
						{
								break;
						}
				}
		}

		return true;
}

//=============================== Touches ===============================
//Begin
void PicShowLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
		CCSetIterator it;
		CCTouch* touch;

		this->currentSelect = NULL;

		for ( it = pTouches->begin(); it != pTouches->end(); it++)
		{
				touch = (CCTouch*)(*it);

				if(!touch)
						break;

				CCPoint location = touch->getLocation();
				location.x -= this->getPositionX();
				location.y -= this->getPositionY();

				for (int i = 0; i < this->elementArr->count(); i++)
				{
						CCSprite *s = (CCSprite*)this->elementArr->objectAtIndex(i);
						//CCRect rect = s->boundingBox();
						CCPoint thePos = s->getPosition();
						CCSize theSize = s->getContentSize();
						CCRect rect = CCRect(thePos.x, thePos.y, theSize.width, theSize.height);
						//CCRect rect = CCRect(s->getPosition().x, s->getPosition().y, s->getContentSize().width, s->getContentSize().height);
						//selected
						if (rect.containsPoint(location))
						{
								this->currentSelect = s;
								break;
						}
				}
		}
}

void PicShowLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
}

void PicShowLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
		if (this->currentSelect == NULL)
		{
				return;
		}

		//Add a new body/atlas sprite at the touched location
		CCSetIterator it;
		CCTouch* touch;

		for ( it = pTouches->begin(); it != pTouches->end(); it++)
		{
				touch = (CCTouch*)(*it);

				if(!touch)
						break;

				CCPoint location = touch->getLocation();
				location.x -= this->getPositionX();
				location.y -= this->getPositionY();

				CCSprite* sEnd = NULL;
				for (int i = 0; i < this->elementArr->count(); i++)
				{
						CCSprite *s = (CCSprite*)this->elementArr->objectAtIndex(i);
						//CCRect rect = s->boundingBox();
						
						CCPoint thePos = s->getPosition();
						CCSize theSize = s->getContentSize();
						CCRect rect = CCRect(thePos.x, thePos.y, theSize.width, theSize.height);
						//selected
						if (rect.containsPoint(location))
						{
								sEnd = s;
								break;
						}
				}

				//click (begin / end at the same object)
				if (sEnd == this->currentSelect)
				{
						IInputNotifier *notifier = dynamic_cast<IInputNotifier*>(this->getParent());
						notifier->ClickOnSprite(location, this->currentSelect);
				}
		}

		this->currentSelect = NULL;
}

void PicShowLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
}

//End


