#include "ToyBrick.h"

ToyBrick::ToyBrick(void)
{
		this->vecArray = NULL;
}

ToyBrick::~ToyBrick()
{
    if (this->vecArray)
    {
        this->vecArray->release();
        this->vecArray = NULL;
    }
}

void ToyBrick::Init(CCTexture2D* texture, CCRect textureRect)
{
		this->initWithTexture(texture, textureRect);
}

void ToyBrick::SetAsPolygon(CCPointArray* vecArray)
{
		this->type = Polygon_Shape;
		this->vecArray = vecArray;
		this->vecArray->retain();
}

void ToyBrick::SetAsCircle(int radius)
{
		this->type = Circle_Shape;
		this->radius = radius;
}
