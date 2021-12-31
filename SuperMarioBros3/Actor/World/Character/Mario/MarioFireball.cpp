#include "MarioFireball.h"

void CMarioFireball::Load()
{
}

void CMarioFireball::Start()
{
}

void CMarioFireball::Update(float elapsedMs)
{
}

void CMarioFireball::Render()
{
}

void CMarioFireball::HandleAction(float elapsedMs)
{
}

void CMarioFireball::Standby(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMarioFireball::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMarioFireball::EActionStage::PROGRESS:
	{
	}
	break;

	case CMarioFireball::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CMarioFireball::Entry(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMarioFireball::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMarioFireball::EActionStage::PROGRESS:
	{
	}
	break;

	case CMarioFireball::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CMarioFireball::Progress(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMarioFireball::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMarioFireball::EActionStage::PROGRESS:
	{
	}
	break;

	case CMarioFireball::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CMarioFireball::Recover(float elapsedMs)
{
	switch (_actionStage)
	{
	case CMarioFireball::EActionStage::START:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CMarioFireball::EActionStage::PROGRESS:
	{
	}
	break;

	case CMarioFireball::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

int CMarioFireball::IsCollidable()
{
    return 0;
}

int CMarioFireball::IsBlocking()
{
    return 0;
}

void CMarioFireball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CMarioFireball::OnNoCollision(float elapsedMs)
{
}

void CMarioFireball::OnCollisionWith(pCollision e)
{
}

void CMarioFireball::OnCollisionWithItem(pCollision e)
{
}
