#include "DancingBoneLayer.h"

DancingBoneLayer::DancingBoneLayer(void)
{
	this->isReal = false;
	this->isPushpinDown = false;

	this->world = NULL;
	this->groundBody = NULL;
	this->boneArray = NULL;

	this->pushpinNormal = NULL;
	this->pushpinDown = NULL;

	this->touchDic = NULL;
}

DancingBoneLayer::~DancingBoneLayer()
{
	if (this->boneArray != NULL)
	{
		this->boneArray->release();
	}

	if (this->pushpinNormal != NULL)
	{
		this->pushpinNormal->release();
	}

	if (this->pushpinDown != NULL)
	{
		this->pushpinDown->release();
	}

	if (this->touchDic != NULL)
	{
		this->touchDic->release();
	}
}

bool DancingBoneLayer::init()
{
	this->touchDic = CCDictionary::create();
	this->touchDic->retain();

	//=================== Box2D World ========================
	b2Vec2 gravity;
	gravity.Set(0.0F, -0.0F);

	this->world = new b2World(gravity);

	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);
	//world->SetDestructionListener

	CCPoint tmp;
	tmp = VisibleRect::leftBottom();
	b2Vec2 lb = b2Vec2(tmp.x / PTM_RATIO, tmp.y / PTM_RATIO + 4.0F);
	tmp = VisibleRect::rightBottom();
	b2Vec2 rb = b2Vec2(tmp.x / PTM_RATIO, tmp.y / PTM_RATIO + 4.0F);
	tmp = VisibleRect::leftTop();
	b2Vec2 lt = b2Vec2(tmp.x / PTM_RATIO, tmp.y / PTM_RATIO);
	tmp = VisibleRect::rightTop();
	b2Vec2 rt = b2Vec2(tmp.x / PTM_RATIO, tmp.y / PTM_RATIO);

	//ground
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
	this->setAccelerometerEnabled( true );

	//=================== Others =======================
	this->InitBones();
	this->InitButtonItem();
	this->SetMode(false);

	//=================== Common Setting =====================
	scheduleUpdate();

	return true;
}

void DancingBoneLayer::update(float dt)
{
	//It is recommended that a fixed time step is used with Box2D for stability
	//of the simulation, however, we are using a variable time step here.
	//You need to make an informed choice, the following URL is useful
	//http://gafferongames.com/game-physics/fix-your-timestep/

	int velocityIterations = 8;
	int positionIterations = 1;


	if ((this->isReal) && (this->world != NULL))
	{
		int count = this->world->GetJointCount();
		b2Joint *joint = this->world->GetJointList();
		for (int i = 0; i < count; i++)
		{
			b2Body *b1 = joint->GetBodyA();
			b2Body *b2 = joint->GetBodyB();

			
			if ((b1 != this->groundBody) &&
				(b2 != this->groundBody))
			{
				int numX = 0;
				int numY = 0;
				numX = CommonHelper::GetRandomNum(-200, 200);
				numY = CommonHelper::GetRandomNum(-200, 200);
				b1->ApplyForceToCenter(b2Vec2(numX, numY));

				numX = CommonHelper::GetRandomNum(-200, 200);
				numY = CommonHelper::GetRandomNum(-200, 200);
				b2->ApplyForceToCenter(b2Vec2(numX, numY));
			}

			joint = joint->GetNext();
		}
	}

	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	world->Step(dt, velocityIterations, positionIterations);
}

void DancingBoneLayer::draw()
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

	//draw ground
	glLineWidth(2);
	ccDrawColor4B(124,1,1,128);
	cocos2d::ccDrawLine(ccp(0, 4 * PTM_RATIO), ccp(VisibleRect::rightBottom().x, 4 * PTM_RATIO));

	//draw joint
	if ((!this->isReal) && (this->world != NULL))
	{
		int count = this->world->GetJointCount();
		b2Joint *joint = this->world->GetJointList();
		for (int i = 0; i < count; i++)
		{
			if ((joint->GetBodyA() != this->groundBody) &&
				(joint->GetBodyB() != this->groundBody))
			{
				b2Vec2 vec = joint->GetAnchorA();
				CCPoint pos = ccp(vec.x * PTM_RATIO, vec.y * PTM_RATIO);

				cocos2d::ccDrawCircle(pos, 32, 0, 32, true);
				//cocos2d::ccDrawSolidRect(pos, ccp(pos.x + 64, pos.y + 64), ccc4f(101, 11, 12, 21));
			}
			joint = joint->GetNext();
		}
	}
}

void DancingBoneLayer::InitBones()
{
	this->boneArray = CCArray::create();
	this->boneArray->retain();

	//--------------------------------Bones------------------------------
	ToyBrick *head = this->CreateHead();
	this->AddToyBrick(head, ccp(300, 500));
	ToyBrick *body = this->CreateBody();
	this->AddToyBrick(body, ccp(300, 400));

	ToyBrick *arm1 = this->CreateArm();
	this->AddToyBrick(arm1, ccp(250, 350));
	ToyBrick *arm2 = this->CreateArm();
	this->AddToyBrick(arm2, ccp(250, 450));
	ToyBrick *arm3 = this->CreateArm();
	this->AddToyBrick(arm3, ccp(350, 350));
	ToyBrick *arm4 = this->CreateArm();
	this->AddToyBrick(arm4, ccp(350, 450));


	ToyBrick *arm5 = this->CreateArm();
	this->AddToyBrick(arm5, ccp(250, 250));
	ToyBrick *arm6 = this->CreateArm();
	this->AddToyBrick(arm6, ccp(350, 250));
	ToyBrick *arm7 = this->CreateArm();
	this->AddToyBrick(arm7, ccp(250, 150));
	ToyBrick *arm8 = this->CreateArm();
	this->AddToyBrick(arm8, ccp(350, 150));

	//--------------------------------Toys------------------------------
	ToyBrick *tri1 = this->CreateBrick_Triangle();
	this->AddToyBrick(tri1, ccp(480, 400));
	ToyBrick *tri2 = this->CreateBrick_Triangle();
	this->AddToyBrick(tri2, ccp(700, 450));

	ToyBrick *c1 = this->CreateBrick_Circle();
	this->AddToyBrick(c1, ccp(620, 470));

	ToyBrick *r1 = this->CreateBrick_Rectangle();
	this->AddToyBrick(r1, ccp(530, 220));
	ToyBrick *r2 = this->CreateBrick_Rectangle();
	this->AddToyBrick(r2, ccp(670, 220));
	ToyBrick *r3 = this->CreateBrick_Rectangle();
	this->AddToyBrick(r3, ccp(530, 180));
	ToyBrick *r4 = this->CreateBrick_Rectangle();
	this->AddToyBrick(r4, ccp(670, 180));

	ToyBrick *big1 = this->CreateBrick_BigRectangle();
	this->AddToyBrick(big1, ccp(600, 350));
}

void DancingBoneLayer::SetMode(bool isReal)
{
	this->isReal = isReal;

	//reset pushpin
	this->isPushpinDown = false;
	this->pushpin->setVisible(!isReal);

	//gravity
	float gravityNum = isReal ? -0.5F : 0.0F;
	b2Vec2 gravity;
	gravity.Set(0.0F, gravityNum);
	this->world->SetGravity(gravity);

	//mouse touch
	this->setTouchEnabled(!isReal);

	//bones
	for (int i = 0; i < this->boneArray->count(); i++)
	{
		PhysicsSprite *sprite = (PhysicsSprite*)this->boneArray->objectAtIndex(i);
		b2Fixture *fixture = sprite->body->GetFixtureList();
		while (fixture != NULL)
		{
			b2Filter filter = fixture->GetFilterData();
			filter.groupIndex = isReal ? 1 : -1;
			fixture->SetFilterData(filter);

			fixture = fixture->GetNext();
		};
	}
}

void DancingBoneLayer::InitButtonItem()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	//pushpin
	this->pushpin = CCMenuItemImage::create(
		"Images/DancingBone/pushpin.png",
		"Images/DancingBone/pushpin.png",
		this,
		menu_selector(DancingBoneLayer::OnPushpinClick));
	pushpin->setAnchorPoint(CCPointZero);
	CCSize size = pushpin->getContentSize();
	pushpin->setPosition(ccp(winSize.width - size.width - 10, winSize.height - size.height - 10));
	this->isPushpinDown = false;

	this->pushpinNormal = CCSprite::create("Images/DancingBone/pushpin.png");
	this->pushpinNormal->retain();
	this->pushpinDown = CCSprite::create("Images/DancingBone/pushpinDown.png");
	this->pushpinDown->retain();

	//Menu
	CCMenu* menu = CCMenu::create(pushpin, NULL);
	menu->setPosition(CCPointZero);
	this->addChild(menu, 1);
}

void DancingBoneLayer::OnPushpinClick(CCObject* pSender)
{
	this->isPushpinDown = !this->isPushpinDown;

	//Pause/Resume
	if (this->isPushpinDown)
	{
		this->pushpin->setNormalImage(this->pushpinDown);
		this->pushpin->setSelectedImage(this->pushpinDown);
	}
	else
	{
		this->pushpin->setNormalImage(this->pushpinNormal);
		this->pushpin->setSelectedImage(this->pushpinNormal);
	}
}

//=============================== Touches ===============================
//Begin
void DancingBoneLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
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

		if (this->isPushpinDown)
		{
			MultiBodyQueryCallback callback(pos);
			this->world->QueryAABB(&callback, aabb);

			//if query something
			if (callback.queryBodyArray)
			{
				b2Body *front = NULL;
				b2Body *end = NULL;
				int count = callback.queryBodyArray->count();
				for (int i = 0 ; i < count; i++)
				{
					MultiBodyQueryNode *queryNode = (MultiBodyQueryNode*)callback.queryBodyArray->objectAtIndex(i);

					if (front == NULL)
					{
						front = queryNode->body;
						continue;
					}

					end = queryNode->body;
					//create joint here.
					b2RevoluteJointDef jointDef;
					jointDef.Initialize(front, end, pos);
					jointDef.enableMotor = true;
					jointDef.motorSpeed = 0.0F;
					jointDef.maxMotorTorque = 15.0F;
					jointDef.lowerAngle = -0.125f * b2_pi;
					jointDef.upperAngle = 0.125f * b2_pi;
					b2Joint *pushpinJoint = this->world->CreateJoint(&jointDef);

					front = end;
				}
			}
		}
		else
		{
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
				body->SetAwake(true);

				MouseJointObject *mouseJoint = new MouseJointObject();
				mouseJoint->mouseJoint = (b2MouseJoint*)this->world->CreateJoint(&md);
				mouseJoint->autorelease();
				mouseJoint->retain();
				this->touchDic->setObject(mouseJoint, touch->getID());
			}
		}
	}
}

void DancingBoneLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
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

		CCObject *obj = this->touchDic->objectForKey(touch->getID());
		if (obj == NULL)
		{
			continue;
		}
		MouseJointObject *mouseJoint = (MouseJointObject*)obj;
		mouseJoint->mouseJoint->SetTarget(pos);
	}
}

void DancingBoneLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
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
		CCObject *obj = this->touchDic->objectForKey(touch->getID());
		if (obj == NULL)
		{
			continue;
		}
		MouseJointObject *mouseJoint = (MouseJointObject*)obj;
		this->world->DestroyJoint(mouseJoint->mouseJoint);
		this->touchDic->removeObjectForKey(touch->getID());
		mouseJoint->release();
	}
}

void DancingBoneLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
}
//End

ToyBrick* DancingBoneLayer::CreateBrick_Circle()
{
	int num = CommonHelper::GetRandomNum(1, 30);

	//image && texture
	char path[255];
	sprintf(path, "Images/ToyBrick/Circle/%d.png", num);

	CCImage *img = new CCImage();
	img->autorelease();
	if (!img->initWithImageFile(path, CCImage::kFmtPng))
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

ToyBrick* DancingBoneLayer::CreateBrick_Triangle()
{
	int num = CommonHelper::GetRandomNum(1, 11);

	//image && texture
	char path[255];
	sprintf(path, "Images/ToyBrick/Triangle/%d.png", num);

	CCImage *img = new CCImage();
	img->autorelease();
	if (!img->initWithImageFile(path, CCImage::kFmtPng))
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

ToyBrick* DancingBoneLayer::CreateBrick_Rectangle()
{
	int num = CommonHelper::GetRandomNum(1, 20);

	//image && texture
	char path[255];
	sprintf(path, "Images/ToyBrick/Rectangle/%d.png", num);

	CCImage *img = new CCImage();
	img->autorelease();
	if (!img->initWithImageFile(path, CCImage::kFmtPng))
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

ToyBrick* DancingBoneLayer::CreateBrick_BigRectangle()
{
	int num = CommonHelper::GetRandomNum(1, 20);

	//image && texture
	char path[255];
	sprintf(path, "Images/ToyBrick/BigRectangle/%d.png", num);

	CCImage *img = new CCImage();
	img->autorelease();
	if (!img->initWithImageFile(path, CCImage::kFmtPng))
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


ToyBrick* DancingBoneLayer::CreateHead()
{
	//image && texture
	char path[255];
	sprintf(path, "Images/DancingBone/head.png");

	CCImage *img = new CCImage();
	img->autorelease();
	if (!img->initWithImageFile(path, CCImage::kFmtPng))
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

	//vector (61, 89)
	CCPointArray *vector = CCPointArray::create(4);
	vector->addControlPoint(ccp(-30, -45));
	vector->addControlPoint(ccp(30, -45));
	vector->addControlPoint(ccp(30, 45));
	vector->addControlPoint(ccp(-30, 45));
	brick->SetAsPolygon(vector);

	return brick;
}

ToyBrick* DancingBoneLayer::CreateBody()
{
	//image && texture
	char path[255];
	sprintf(path, "Images/DancingBone/body.png");

	CCImage *img = new CCImage();
	img->autorelease();
	if (!img->initWithImageFile(path, CCImage::kFmtPng))
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

	//vector (87, 150)
	CCPointArray *vector = CCPointArray::create(4);
	vector->addControlPoint(ccp(-42, -75));
	vector->addControlPoint(ccp(42, -75));
	vector->addControlPoint(ccp(42, 75));
	vector->addControlPoint(ccp(-42, 75));
	brick->SetAsPolygon(vector);

	return brick;
}

ToyBrick* DancingBoneLayer::CreateArm()
{
	//image && texture
	char path[255];
	sprintf(path, "Images/DancingBone/arm.png");

	CCImage *img = new CCImage();
	img->autorelease();
	if (!img->initWithImageFile(path, CCImage::kFmtPng))
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

	//vector (24, 102)
	CCPointArray *vector = CCPointArray::create(4);
	vector->addControlPoint(ccp(-12, -50));
	vector->addControlPoint(ccp(12, -50));
	vector->addControlPoint(ccp(12, 50));
	vector->addControlPoint(ccp(-12, 50));
	brick->SetAsPolygon(vector);

	return brick;
}

void DancingBoneLayer::AddToyBrick(ToyBrick *brick, CCPoint position)
{
	//create brick and add it.
	brick->setPosition( ccp(position.x, position.y) );
	this->boneArray->addObject(brick);
	this->addChild(brick);

	//create corresponding b2Body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
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
