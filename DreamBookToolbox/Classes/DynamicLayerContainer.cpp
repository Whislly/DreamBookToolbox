#include "DynamicLayerContainer.h"

DynamicLayerContainer::DynamicLayerContainer(void)
{
		this->arr = NULL;
}

DynamicLayerContainer::~DynamicLayerContainer(void)
{
		if (this->arr != NULL)
		{
				delete this->arr;
		}
}

DynamicLayerContainer* DynamicLayerContainer::create(int totalCount)
{
		DynamicLayerContainer *containter = new DynamicLayerContainer();
		containter->autorelease();
		containter->totalCount = totalCount;

		containter->currentLayerIndex = -1;
		containter->arr = new CCArray();

		containter->init();

		return containter;
}

bool DynamicLayerContainer::init()
{
		int start = 0;
		while (start < this->totalCount)
		{
				PicShowLayer *createLayer = PicShowLayer::create(start);
				createLayer->setAnchorPoint(CCPointZero);
				createLayer->setVisible(false);
				this->arr->addObject(createLayer);
				this->addChild(createLayer);

				start += createLayer->elementArr->count();
		}

		this->currentLayerIndex = 0;
		PicShowLayer *current = this->GetCurrentShowLayer();
		current->setVisible(true);
		current->setTouchEnabled(true);

		return true;
}

void DynamicLayerContainer::ClickOnSprite(CCPoint location, CCSprite *sprite)
{
		IInputNotifier *notifier = dynamic_cast<IInputNotifier*>(this->getParent());
		notifier->ClickOnSprite(location, sprite);
}

PicShowLayer* DynamicLayerContainer::GetCurrentShowLayer()
{
		return (PicShowLayer*)this->arr->objectAtIndex(this->currentLayerIndex);
}

PicShowLayer* DynamicLayerContainer::GoToNext()
{
		if (this->currentLayerIndex >= (this->arr->count() - 1))
		{
				return NULL;
		}

		PicShowLayer* current = this->GetCurrentShowLayer();
		current->setTouchEnabled(false);

		this->currentLayerIndex++;
		PicShowLayer* next = this->GetCurrentShowLayer();
		next->setTouchEnabled(true);

		return next;
}

PicShowLayer* DynamicLayerContainer::GoToPre()
{
		if (this->currentLayerIndex <= 0)
		{
				return NULL;
		}

		PicShowLayer* current = this->GetCurrentShowLayer();
		current->setTouchEnabled(false);

		this->currentLayerIndex--;
		PicShowLayer* pre = this->GetCurrentShowLayer();
		pre->setTouchEnabled(true);

		return pre;
}
