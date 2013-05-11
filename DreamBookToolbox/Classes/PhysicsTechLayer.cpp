#include "PhysicsTechLayer.h"

PhysicsTechLayer::PhysicsTechLayer(void)
{
		this->world = NULL;
		this->propertyLayer = NULL;
		this->currentSelection = NULL;
}

PhysicsTechLayer::~PhysicsTechLayer(void)
{
}

bool PhysicsTechLayer::init()
{
		//static ground
		b2BodyDef bodyDef;
		bodyDef.userData = NULL;
		this->groundBody = this->world->CreateBody(&bodyDef);

		//static triangle
		{
				b2BodyDef triangleBodyDef;
				triangleBodyDef.position.Set(0.0F, 4.0F);
				//triangleBodyDef.type = b2BodyType::b2_dynamicBody;
				triangleBodyDef.type = b2BodyType::b2_staticBody;

				CustomUserData *userData = new CustomUserData();
				userData->draw = true;
				userData->id = 1;
				userData->groupId = 1;
				userData->color = ccc4f(0.2, 0.3, 0.5, 1);
				triangleBodyDef.userData = userData;
				b2Body *triangleBody = this->world->CreateBody(&triangleBodyDef);

				//fixture
				b2FixtureDef fixtureDef;
				fixtureDef.density = 1.0F;
				fixtureDef.friction = 0.0F;
				fixtureDef.restitution = 0.0F;

				//shape
				b2PolygonShape polygon;
				b2Vec2 border[3];
				border[0] = b2Vec2(0.0F, 0.0F);
				border[1] = b2Vec2(16.0F, 0.0F);
				border[2] = b2Vec2(0.0F, 8.0F);
				polygon.Set(border, 3);

				fixtureDef.shape = &polygon;

				//create fixture
				triangleBody->CreateFixture(&fixtureDef);
		}

		//movable rectangle object
		{
				b2BodyDef rectBodyDef;
				rectBodyDef.angle = -0.15F * b2_pi;
				rectBodyDef.position.Set(4.0F, (4.0F / 0.57735F) + 4.0F);
				rectBodyDef.type = b2BodyType::b2_dynamicBody;
				rectBodyDef.userData = NULL;
				b2Body *rectBody = this->world->CreateBody(&rectBodyDef);

				//fixture
				b2FixtureDef fixtureDef;
				fixtureDef.density = 1.0F;
				fixtureDef.friction = 0.0F;
				fixtureDef.restitution = 0.1F;

				//shape
				b2PolygonShape polygon;
				b2Vec2 border[4];
				float k = 26.0F / PTM_RATIO;
				border[0] = b2Vec2(-k, -k);
				border[1] = b2Vec2(k, -k);
				border[2] = b2Vec2(k, k);
				border[3] = b2Vec2(-k, k);
				polygon.Set(border, 4);

				fixtureDef.shape = &polygon;

				//create fixture
				rectBody->CreateFixture(&fixtureDef);


				PhysicsSprite *rect = PhysicsSprite::create();
				//rect->setAnchorPoint(ccp(1.0F, 1.0F));
				//rect->set
				this->addChild(rect);
				CCImage *img = new CCImage();
				img->autorelease();
				img->initWithImageFile("Images/PhysicsTech/1.png", cocos2d::CCImage::EImageFormat::kFmtPng);
				CCTexture2D *texture = new CCTexture2D();
				texture->autorelease();
				texture->initWithImage(img);
				rect->initWithTexture(texture);
				rect->setPhysicsBody(rectBody);
		}

		//enable
		this->setTouchEnabled( true );
		this->setAccelerometerEnabled( true );


		//=================== Common Setting =====================
		scheduleUpdate();

		return true;
}

void PhysicsTechLayer::update(float dt)
{
		//It is recommended that a fixed time step is used with Box2D for stability
		//of the simulation, however, we are using a variable time step here.
		//You need to make an informed choice, the following URL is useful
		//http://gafferongames.com/game-physics/fix-your-timestep/

		int velocityIterations = 8;
		int positionIterations = 1;

		// Instruct the world to perform a single step of simulation. It is
		// generally best to keep the time step and iterations fixed.
		this->world->Step(dt, velocityIterations, positionIterations);
}

void PhysicsTechLayer::draw()
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

		//draw ground
		glLineWidth(2);
		ccDrawColor4B(124,1,1,128);
		cocos2d::ccDrawLine(ccp(0, 4 * PTM_RATIO), ccp(VisibleRect::rightBottom().x, 4 * PTM_RATIO));

		int count  = this->world->GetBodyCount();
		b2Body* body = this->world->GetBodyList();
		for (int i = 0 ; i < count; i++)
		{
				CustomUserData *data = (CustomUserData*)body->GetUserData();
				if (data == NULL)
				{
						body = body->GetNext();
						continue;
				}

				CustomUserData *userData = (CustomUserData*)data;
				if (userData->draw)
				{
						b2Vec2 pos = body->GetPosition();

						//draw the object here.
						b2Shape *shape = body->GetFixtureList()->GetShape();
						if (shape->GetType() == b2Shape::e_polygon)
						{
								b2PolygonShape *pShape = (b2PolygonShape*)shape;

								CCPoint *points = new CCPoint[pShape->m_vertexCount];
								b2Vec2 *vecs = pShape->m_vertices;
								for (int i = 0; i < pShape->m_vertexCount; i++)
								{
										points[i] = CCPoint((vecs->x + pos.x) * PTM_RATIO, (vecs->y + pos.y) * PTM_RATIO);
										vecs++;
								}

								//ccDrawColor4B(userData->color.r, userData->color.g, userData->color.b, userData->color.a);

								//ccDrawColor4B(0,0,255,128);
								//cocos2d::ccDrawSolidPoly(points, pShape->m_vertexCount, ccc4f(0.2, 0.3, 0.5, 1));
								cocos2d::ccDrawSolidPoly(points, pShape->m_vertexCount, userData->color);
								//cocos2d::ccDrawPoly(points, pShape->m_vertexCount, true);

								delete[] points;
						}
						else if (shape->GetType() == b2Shape::e_circle)
						{
						}
				}

				body = body->GetNext();
		}
}

//=============================== Touches ===============================
//Begin
void PhysicsTechLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
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

				PosQuery callback(pos);
				this->world->QueryAABB(&callback, aabb);

				//if query something
				if (callback.object)
				{
						b2Vec2 pos = callback.object->GetPosition();
						this->propertyLayer->setPosition(pos.x * PTM_RATIO - 24, pos.y * PTM_RATIO - 24);

						if (this->currentSelection != NULL)
						{
								//select this, equals close
								if (this->currentSelection == callback.object)
								{
										this->propertyLayer->setVisible(false);
										this->SetProperty(this->propertyLayer, callback.object);
										this->currentSelection = NULL;
								}
								//select another
								else
								{
										this->propertyLayer->setVisible(true);
										this->SetProperty(this->propertyLayer, this->currentSelection);
										this->GetPropertyAndShow(callback.object, this->propertyLayer);
										this->currentSelection = callback.object;
								}
						}
						else
						{
								this->propertyLayer->setVisible(true);
								this->GetPropertyAndShow(callback.object, this->propertyLayer);
								this->currentSelection = callback.object;
						}

						/*
						bool visible = this->propertyLayer->isVisible();

						//set value to object
						if (visible)
						{
								if (this->currentSelection != NULL)
								{
										this->SetProperty(this->propertyLayer, callback.object);
								}
						}
						//get value from object & show
						else
						{
								this->currentSelection = callback.object;
								this->GetPropertyAndShow(callback.object, this->propertyLayer);
						}

						this->propertyLayer->setVisible(!visible);
						*/
				}
				/*else
				{
						if (this->currentSelection != NULL)
						{
								this->SetProperty(this->propertyLayer, this->currentSelection);
								this->propertyLayer->setVisible(false);
						}
						this->currentSelection = NULL;
				}*/
		}
}

void PhysicsTechLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
}

void PhysicsTechLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
		//Add a new body/atlas sprite at the touched location
		CCSetIterator it;
		CCTouch* touch;

		for ( it = pTouches->begin(); it != pTouches->end(); it++)
		{
				touch = (CCTouch*)(*it);

				if(!touch)
						break;
		}
}

void PhysicsTechLayer::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
}

//End


void PhysicsTechLayer::GetPropertyAndShow(b2Body *object, PropertyLayer *layer)
{
		float mass = object->GetMass();
		layer->SetProperty(PropertyType_Mass, &mass);

		bool isStatic = object->GetType() == b2BodyType::b2_staticBody ? true : false;
		layer->SetProperty(PropertyType_Static, &isStatic);
}

void PhysicsTechLayer::SetProperty(PropertyLayer *layer, b2Body *object)
{
		if (layer->nodeArray == NULL)
		{
				return;
		}

		int count = layer->nodeArray->count();
		for (int i = 0; i < count; i++)
		{
				PropertyInput *input = (PropertyInput*)layer->nodeArray->objectAtIndex(i);
				const char* str = input->text->getString();
				switch (input->propertyType)
				{
				case PropertyType_Static:
						{
								if (strcmp(str, "true") == 0)
								{
										object->SetType(b2BodyType::b2_staticBody);
								}
								else
								{
										object->SetType(b2BodyType::b2_dynamicBody);
								}
						}
						break;
				case PropertyType_Mass:
						{
								b2MassData mass;
								object->GetMassData(&mass);
								mass.mass = atof(str);
								object->SetMassData(&mass);
						}
						break;
				default:
						break;
				}
		}
}








