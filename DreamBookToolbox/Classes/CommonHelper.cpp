#include "CommonHelper.h"

CommonHelper::CommonHelper(void)
{
}

int CommonHelper::GetRandomNum(int min, int max)
{
		int totalRange = max - min + 1;

		//make a num between 0 ~ 100
		float randomNumF = CCRANDOM_0_1();
		int seed = randomNumF * 10000;
		//
		int rtn = seed % totalRange + min;

		if (rtn < min)
		{
				rtn = min;
		}
		else if (rtn > max)
		{
				rtn = max;
		}

		return rtn;
}

CCRect CommonHelper::GetRect(CCNode * node)
{
		CCRect rc;
		rc.origin = node->getPosition();
		rc.size = node->getContentSize();
		//rc.origin.x -= rc.size.width / 2;
		//rc.origin.y -= rc.size.height / 2;

		//return rc;
		return CCRect(rc.origin.x, rc.origin.y, rc.size.width, rc.size.height);
}

b2World* CommonHelper::CreateWorld()
{
		//=================== Box2D World ========================
		b2Vec2 gravity;
		gravity.Set(0.0F, -10.0F);

		b2World *world = new b2World(gravity);
		world->SetAllowSleeping(true);
		world->SetContinuousPhysics(true);

		//TODO here : contact 
		//ToyContact *contact = new ToyContact();
		//world->SetContactListener(contact);

		CCPoint tmp;
		tmp = VisibleRect::leftBottom();
		b2Vec2 lb = b2Vec2(tmp.x / PTM_RATIO, tmp.y / PTM_RATIO + 4.0F);
		tmp = VisibleRect::rightBottom();
		b2Vec2 rb = b2Vec2(tmp.x / PTM_RATIO, tmp.y / PTM_RATIO + 4.0F);
		tmp = VisibleRect::leftTop();
		b2Vec2 lt = b2Vec2(tmp.x / PTM_RATIO, tmp.y / PTM_RATIO);
		tmp = VisibleRect::rightTop();
		b2Vec2 rt = b2Vec2(tmp.x / PTM_RATIO, tmp.y / PTM_RATIO);

		//World Edge
		b2BodyDef bd;
		bd.userData = NULL;
		b2Body* ground = world->CreateBody(&bd);

		{
				b2EdgeShape shape;
				shape.Set(lb, rb);
				b2Fixture *fix = ground->CreateFixture(&shape, 0.0f);
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

		return world;
}

void CommonHelper::DrawNode(CCNode *node)
{
		//
}

CCSize CommonHelper::CountContainerNumber(CCSize containerSize, int width, int xInterval, int xMargin, int height, int yInterval, int yMargin)
{
		//calc x-number
		int x = containerSize.width - 2 * xMargin;
		int roughXNum = x / (width + xInterval);
		int subX = roughXNum + 1;
		if (subX * width + xInterval * roughXNum <= x)
		{
				roughXNum++;
		}

		//calc y-number
		int y = containerSize.height - 2 * yMargin;
		int roughYNum = y / (height + yInterval);
		int subY = roughYNum + 1;
		if (subY * height + yInterval * roughYNum <= y)
		{
				roughYNum++;
		}

		return CCSize(roughXNum, roughYNum);
}

void CommonHelper::CopySpriteProperty(CCSprite* target, CCSprite* origin)
{
		CCTexture2D *texture = origin->getTexture();
		target->initWithTexture(texture, origin->getTextureRect());
		target->setScale(origin->getScale());
		target->setContentSize(origin->getContentSize());
		target->setAnchorPoint(CCPointZero);
}

CCSprite* CommonHelper::CloneSprite(CCSprite *sprite)
{
		CCSprite *newSprite = CCSprite::create();
		CommonHelper::CopySpriteProperty(newSprite, sprite);

		return newSprite;
}

CCArray* CommonHelper::getIntArray( char* content )
{
    CCArray* pIntArray = NULL;
    int count = strlen(content);
    if (count > 0)
    {
        pIntArray = CCArray::create();
        int startIdx = 0;
        for (int i = 1; i < count; i++)
        {
            if (content[i] == ',')
            {
                CCString* pTemp = CCString::createWithData((unsigned char*)(content + startIdx), i - startIdx);
                CCInteger* pInt = CCInteger::create(pTemp->intValue());
                pIntArray->addObject(pInt);
                startIdx = i + 1;
            }
        }
        if (startIdx != count)
        {
            CCString* pTemp = CCString::createWithData((unsigned char*)(content + startIdx), count - startIdx);
            CCInteger* pInt = CCInteger::create(pTemp->intValue());
            pIntArray->addObject(pInt);
        }
    }
    return pIntArray;
}

FileSprite* CommonHelper::CloneFileSprite(FileSprite *sprite)
{
		FileSprite *newSprite = FileSprite::create();
		sprintf(newSprite->path, sprite->path);
		CommonHelper::CopySpriteProperty(newSprite, sprite);

		return newSprite;
}
