#include "MouseClass.h"

#include "MouseEvent.h"


MouseClass::MouseClass()
{

}

MouseClass::~MouseClass()
{

}

void MouseClass::OnLeftPressed(int x, int y)
{
	this->leftIsDown = true;
	MouseEvent me(MouseEvent::EventType::LPress, x, y);
	this->eventBuffer.push(me);
}

void MouseClass::OnLeftReleased(int x, int y)
{
	this->leftIsDown = false;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::LRelease, x, y));
}

void MouseClass::OnRightPressed(int x, int y)
{
	this->rightIsDown = true;
	this->eventBuffer.push(MouseEvent(MouseEvent::EventType::RRelease, x, y));
}

void MouseClass::OnRightReleased(int x, int y)
{

}

void MouseClass::OnRightReleased(int x, int y)
{

}

void MouseClass::OnMiddlePressed(int x, int y)
{

}

void MouseClass::OnMiddleReleased(int x, int y)
{

}

void MouseClass::OnWheelUp(int x, int y)
{

}

void MouseClass::OnWheelDows(int x, int y)
{

}

void MouseClass::OnMouseMove(int x, int y)
{

}

bool MouseClass::IsLeftDown()
{

}

bool MouseClass::IsMiddleDown()
{

}

bool MouseClass::IsRightDown()
{

}

int MouseClass::GetPosX()
{

}

int MouseClass::GetPosY()
{

}

MousePoint MouseClass::GetPos()
{

}

bool MouseClass::EventBufferIsEmpty()
{

}

MouseEvent MouseClass::ReadEvent()
{

}
