#include "MarioTail.h"
#include "../../SuperMarioBros3.h"
#include "../Character/Goomba.h"
#include "../Character/Koopa.h"
#include "../Character/Piranha.h"
#include "../Character/Venus.h"
#include "../Prop/Brick.h"
#include "../Prop/Block.h"


void CMarioTail::Load()
{
}

void CMarioTail::Start()
{
}

void CMarioTail::Update(float elapsedMs)
{
}

void CMarioTail::Render()
{
}

void CMarioTail::HandleAction(float elapsedMs)
{
}

void CMarioTail::Standby(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMarioTail::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMarioTail::EActionStage::PROGRESS:
	{
	}
	break;

	case CMarioTail::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CMarioTail::Entry(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMarioTail::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMarioTail::EActionStage::PROGRESS:
	{
	}
	break;

	case CMarioTail::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CMarioTail::Progress(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMarioTail::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMarioTail::EActionStage::PROGRESS:
	{
	}
	break;

	case CMarioTail::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CMarioTail::Recover(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMarioTail::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMarioTail::EActionStage::PROGRESS:
	{
	}
	break;

	case CMarioTail::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

int CMarioTail::IsCollidable()
{
	return 0;
}

int CMarioTail::IsBlocking()
{
	return 0;
}

void CMarioTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CMarioTail::OnNoCollision(float elapsedMs)
{
}

void CMarioTail::OnCollisionWith(pCollision e)
{
}

void CMarioTail::OnCollisionWithItem(pCollision e)
{
}
