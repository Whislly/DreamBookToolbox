#include "MainScene.h"
#include "ToyLayer.h"

ToyLayer::ToyLayer(void)
{
		this->world = NULL;
		this->groundBody = NULL;

		this->toyArray = NULL;
		
		this->mouseJoint = NULL;
}

ToyLayer::~ToyLayer()
{
		if (this->toyArray != NULL)
		{
				this->toyArray->release();
				this->toyArray = NULL;
		}
}

bool ToyLayer::init()
{

		//=================== Box2D World ========================
		b2Vec2 gravity;
		gravity.Set(0.0F, -10.0F);

		this->world = new b2World(gravity);
		world->SetAllowSleeping(true);
		world->SetContinuousPhysics(true);

		//TODO here : contact 
		//ToyContact *contact = new ToyContact();
		//world->SetContactListener(contact);


		CCPoint tmp;
		tmp = VisibleRect::leftBottom();
		b2Vec2 lb = b2Vec2(tmp.x / PTM_RATIO, tmp.y / PTM_RATIO);
		tmp = VisibleRect::rightBottom();
		b2Vec2 rb = b2Vec2(tmp.x / PTM_RATIO, tmp.y / PTM_RATIO);
		tmp = VisibleRect::leftTop();
		b2Vec2 lt = b2Vec2(tmp.x / PTM_RATIO, tmp.y / PTM_RATIO);
		tmp = VisibleRect::rightTop();
		b2Vec2 rt = b2Vec2(tmp.x / PTM_RATIO, tmp.y / PTM_RATIO);


		b2BodyDef bodyDef;
		this->groundBody = this->world->CreateBody(&bodyDef);

		//World Edge
		b2BodyDef bd;
		b2Body* ground = this->world->CreateBody(&bd);
		{
				b2EdgeShape shape;
				shape.Set(lb, rb);
				ground->CreateFixture(&shape, 0.0f);
		}

		{
				b2EdgeShape shape;
				shape.Set(lt, rt);
				ground->CreateFixture(&shape, 0.0f);
		}

		{
				b2EdgeShape shape;
				shape.Set(lb, lt);
				ground->CreateFixture(&shape, 0.0f);
		}

		{
				b2EdgeShape shape;
				shape.Set(rb, rt);
				ground->CreateFixture(&shape, 0.0f);
		}

		//enable
		this->setTouchEnabled( true );
		this->setAccelerometerEnabled( true );

		//=================== Others =======================
		this->toyArray = CCArray::create();
		this->toyArray->retain();

		//CCSpriteBatchNode *boxBatch = CCSpriteBatchNode::create("Images/blocks.png", 100);
		CCSpriteBatchNode *boxBatch = CCSpriteBatchNode::create("Images/ToyBrick/Triangle/1.png", 100);
		boxBatch->setTag(1);
		this->addChild(boxBatch, 0);
		this->boxTexture = boxBatch->getTexture();

		//init
		this->AddRandomToyBrick(ccp(200, 200));
		this->AddRandomToyBrick(ccp(300, 300));
		this->AddRandomToyBrick(ccp(500, 500));

		//=================== Common Setting =====================
		scheduleUpdate();

		return true;
}

void ToyLayer::update(float dt)
{
		//It is recommended that a fixed time step is used with Box2D for stability
		//of the simulation, however, we are using a variable time step here.
		//You need to make an informed choice, the following URL is useful
		//http://gafferongames.com/game-physics/fix-your-timestep/

		int velocityIterations = 8;
		int positionIterations = 1;

		// Instruct the world to perform a single step of simulation. It is
		// generally best to keep the time step and iterations fixed.
		world->Step(dt, velocityIterations, positionIterations);
}

void ToyLayer::draw()
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
}

//=============================== Touches ===============================
//Begin
void ToyLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
		CCSetIterator it;
		CCTouch* touch;

		for ( it = pTouches->begin(); it != pTouches->end(); it++) 
		{
				touch = (CCTouch*)(*it);

				if(!touch)
						break;

				CCPoint location = touch->getLocation();
				b2Vec2 pos = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);

				b2AABB aabb;
				b2Vec2 d;
				d.Set(0.001f, 0.001f);
				aabb.lowerBound = pos - d;
				aabb.upperBound = pos + d;

				PositionQueryCallback callback(pos);
				this->world->QueryAABB(&callback, aabb);

				//if query something
				if (callback.objFixture)
				{
						b2Body *body = callback.objFixture->GetBody();

						b2MouseJointDef md;
						md.bodyA = this->groundBody;
						md.bodyB = body;
						md.target = pos;
						md.maxForce = 1000.0F * body->GetMass();
						this->mouseJoint = (b2MouseJoint*)this->world->CreateJoint(&md);
						body->SetAwake(true);
				}
		}
}

void ToyLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
		CCSetIterator it;
		CCTouch* touch;

		for ( it = pTouches->begin(); it != pTouches->end(); it++) 
		{
				touch = (CCTouch*)(*it);

				if(!touch)
						break;

				CCPoint location = touch->getLocation();
				b2Vec2 pos = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);

				if (this->mouseJoint)
				{
						this->mouseJoint->SetTarget(pos);
				}
		}
}

void ToyLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
		//Add a new body/atlas sprite at the touched location
		CCSetIterator it;
		CCTouch* touch;

		for ( it = pTouches->begin(); it != pTouches->end(); it++)
		{
				touch = (CCTouch*)(*it);

				if(!touch)
						break;

				//Clear
				if (this->mouseJoint)
				{
						this->world->DestroyJoint(this->mouseJoint);
						this->mouseJoint = NULL;
				}
		}
}

void ToyLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
}

//End


void ToyLayer::AddToyBrick(ToyBrick *brick, CCPoint position)
{
		//create brick and add it.
		brick->setPosition( ccp(position.x, position.y) );
		this->toyArray->addObject(brick);
		this->addChild(brick);

		//create corresponding b2Body
		b2BodyDef bodyDef;
		bodyDef.type = b2BodyType::b2_dynamicBody;
		bodyDef.position.Set(position.x/PTM_RATIO, position.y/PTM_RATIO);
		
		b2Body *brickBody = world->CreateBody(&bodyDef);

		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.0F;
		fixtureDef.friction = 0.8F;
		fixtureDef.restitution = 0.5F;

		//create shape
		if (brick->type == Polygon_Shape)
		{
				int count = brick->vecArray->count();
				b2PolygonShape polygon;
				b2Vec2* vecList = new b2Vec2[count];
				for (int i = 0; i < count; i++)
				{
						CCPoint p = brick->vecArray->getControlPointAtIndex(i);
						vecList[i] = b2Vec2(p.x / PTM_RATIO, p.y / PTM_RATIO);
				}
				polygon.Set(vecList, count);
				fixtureDef.shape = &polygon;

				//create fixture
				brickBody->CreateFixture(&fixtureDef);

				delete[] vecList;
		}
		else if (brick->type == Circle_Shape)
		{
				b2CircleShape circle;
				circle.m_radius = brick->radius / PTM_RATIO;
				fixtureDef.shape = &circle;

				//create fixture
				brickBody->CreateFixture(&fixtureDef);
		}

		//bind body with Sprite
		brick->setPhysicsBody(brickBody);
}

void ToyLayer::AddRandomToyBrick(CCPoint position)
{
		ToyBrick *circleBrick = this->CreateBrick_Circle();
		this->AddToyBrick(circleBrick, position);

		ToyBrick *triangleBrick = this->CreateBrick_Triangle();
		this->AddToyBrick(triangleBrick, position);

		ToyBrick *rectBrick = this->CreateBrick_Rectangle();
		this->AddToyBrick(rectBrick, position);
		
		ToyBrick *rectBigBrick = this->CreateBrick_BigRectangle();
		this->AddToyBrick(rectBigBrick, position);
}

ToyBrick* ToyLayer::CreateBrick_Circle()
{
		int num = CommonHelper::GetRandomNum(1, 30);

		//image && texture
		char path[255];
		sprintf(path, "Images/ToyBrick/Circle/%d.png", num);

		CCImage *img = new CCImage();
		img->autorelease();
		if (!img->initWithImageFile(path, cocos2d::CCImage::EImageFormat::kFmtPng))
		{
				return NULL;
		}

		CCTexture2D *texture = new CCTexture2D();
		texture->autorelease();
		if (!texture->initWithImage(img))
		{
				return NULL;
		}

		//
		ToyBrick *brick = ToyBrick::create();
		CCSize size = texture->getContentSize();
		brick->Init(texture, CCRectMake(0, 0, size.width, size.height));
		brick->SetAsCircle(64);

		return brick;
}

ToyBrick* ToyLayer::CreateBrick_Triangle()
{
		int num = CommonHelper::GetRandomNum(1, 11);

		//image && texture
		char path[255];
		sprintf(path, "Images/ToyBrick/Triangle/%d.png", num);

		CCImage *img = new CCImage();
		img->autorelease();
		if (!img->initWithImageFile(path, cocos2d::CCImage::EImageFormat::kFmtPng))
		{
				return NULL;
		}

		CCTexture2D *texture = new CCTexture2D();
		texture->autorelease();
		if (!texture->initWithImage(img))
		{
				return NULL;
		}

		//
		ToyBrick *brick = ToyBrick::create();
		CCSize size = texture->getContentSize();
		brick->Init(texture, CCRectMake(0, 0, size.width, size.height));

		//vector
		CCPointArray *vector = CCPointArray::create(3);
		vector->addControlPoint(ccp(-51, -35));
		vector->addControlPoint(ccp(51, -35));
		vector->addControlPoint(ccp(0, 48));
		brick->SetAsPolygon(vector);

		return brick;
}

ToyBrick* ToyLayer::CreateBrick_Rectangle()
{
		int num = CommonHelper::GetRandomNum(1, 20);

		//image && texture
		char path[255];
		sprintf(path, "Images/ToyBrick/Rectangle/%d.png", num);

		CCImage *img = new CCImage();
		img->autorelease();
		if (!img->initWithImageFile(path, cocos2d::CCImage::EImageFormat::kFmtPng))
		{
				return NULL;
		}

		CCTexture2D *texture = new CCTexture2D();
		texture->autorelease();
		if (!texture->initWithImage(img))
		{
				return NULL;
		}


		//
		ToyBrick *brick = ToyBrick::create();
		CCSize size = texture->getContentSize();
		brick->Init(texture, CCRectMake(0, 0, size.width, size.height));
		
		//vector
		CCPointArray *vector = CCPointArray::create(4);
		vector->addControlPoint(ccp(-52, -52));
		vector->addControlPoint(ccp(52, -52));
		vector->addControlPoint(ccp(52, 52));
		vector->addControlPoint(ccp(-52, 52));
		brick->SetAsPolygon(vector);

		return brick;
}

ToyBrick* ToyLayer::CreateBrick_BigRectangle()
{
		int num = CommonHelper::GetRandomNum(1, 20);

		//image && texture
		char path[255];
		sprintf(path, "Images/ToyBrick/BigRectangle/%d.png", num);

		CCImage *img = new CCImage();
		img->autorelease();
		if (!img->initWithImageFile(path, cocos2d::CCImage::EImageFormat::kFmtPng))
		{
				return NULL;
		}

		CCTexture2D *texture = new CCTexture2D();
		texture->autorelease();
		if (!texture->initWithImage(img))
		{
				return NULL;
		}
		
		//
		ToyBrick *brick = ToyBrick::create();
		CCSize size = texture->getContentSize();
		brick->Init(texture, CCRectMake(0, 0, size.width, size.height));
		
		//vector
		CCPointArray *vector = CCPointArray::create(4);
		vector->addControlPoint(ccp(-101, -101));
		vector->addControlPoint(ccp(101, -101));
		vector->addControlPoint(ccp(101, 101));
		vector->addControlPoint(ccp(-101, 101));
		brick->SetAsPolygon(vector);

		return brick;
}

void ToyLayer::keyBackClicked()
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(0.5f, MainLayer::scene(), true));
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void ToyLayer::processWin32KeyPress( UINT message, WPARAM wParam, LPARAM lParam )
{
    CCLog("onWin32KeyEvent message %d wParam %d lParam %d", message, wParam, lParam);
    /*
    // Up
    Win32KeyHook message 256 wParam 38 lParam 21495809
    onWin32KeyEvent message 256 wParam 38 lParam 21495809
    Win32KeyHook message 257 wParam 38 lParam -1052246015
    onWin32KeyEvent message 257 wParam 38 lParam -1052246015
    // Down
    Win32KeyHook message 256 wParam 40 lParam 22020097
    onWin32KeyEvent message 256 wParam 40 lParam 22020097
    Win32KeyHook message 257 wParam 40 lParam -1051721727
    onWin32KeyEvent message 257 wParam 40 lParam -1051721727
    // Left
    Win32KeyHook message 256 wParam 37 lParam 21692417
    onWin32KeyEvent message 256 wParam 37 lParam 21692417
    Win32KeyHook message 257 wParam 37 lParam -1052049407
    onWin32KeyEvent message 257 wParam 37 lParam -1052049407
    // Right
    Win32KeyHook message 256 wParam 39 lParam 21823489
    onWin32KeyEvent message 256 wParam 39 lParam 21823489
    Win32KeyHook message 257 wParam 39 lParam -1051918335
    onWin32KeyEvent message 257 wParam 39 lParam -1051918335
    // Ctrl - Down
    Win32KeyHook message 256 wParam 17 lParam 1075642369
    //Ctrl - Up
    Win32KeyHook message 257 wParam 17 lParam -1071841279

    //Esc
    onWin32KeyEvent message 256 wParam 27 lParam 1073807361
    onWin32KeyEvent message 258 wParam 27 lParam 1073807361
    onWin32KeyEvent message 257 wParam 27 lParam -1073676287
    */

    switch(message)
    {
    case WM_CHAR:
        if (VK_BACK == wParam)
        {
            CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(0.5f, MainLayer::scene(), true));
        }
        break;
    }
}

void ToyLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getOpenGLView()->SetWin32KeyLayer(this);
}

void ToyLayer::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getOpenGLView()->SetWin32KeyLayer(NULL);
}

#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WIN32