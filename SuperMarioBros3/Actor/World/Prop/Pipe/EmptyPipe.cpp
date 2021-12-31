#include "EmptyPipe.h"

void CEmptyPipe::Load()
{
}

void CEmptyPipe::Start()
{
}

void CEmptyPipe::Update(float elapsedMs)
{
}

void CEmptyPipe::Render()
{
}

void CEmptyPipe::HandleAction(float elapsedMs)
{
}

int CEmptyPipe::IsCollidable()
{
	return 0;
}

int CEmptyPipe::IsBlocking()
{
	return 0;
}

void CEmptyPipe::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CEmptyPipe::OnNoCollision(float elapsedMs)
{
}

void CEmptyPipe::OnCollisionWith(pCollision e)
{
}
