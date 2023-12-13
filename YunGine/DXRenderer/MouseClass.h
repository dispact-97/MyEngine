#pragma once
#include <queue>
#include "MouseEvent.h"

class MouseClass
{
public:
	MouseClass();
	~MouseClass();

public:
	// mouse status
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x , int y);
	void OnRightReleased(int x, int y);
	void OnMiddlePressed(int x, int y);
	void OnMiddleReleased(int x, int y);

	void OnWheelUp(int x, int y);

	void OnWheelDows(int x, int y);

	void OnMouseMove(int x, int y);

	bool IsLeftDown();
	bool IsMiddleDown();
	bool IsRightDown();

	int GetPosX();
	int GetPosY();
	MousePoint GetPos();

	bool EventBufferIsEmpty();
	MouseEvent ReadEvent();

private:
	std::queue<MouseEvent> eventBuffer;
	bool leftIsDown = false;
	bool rightIsDown = false;
	bool mbuttonDown = false;
	int x = 0;
	int y = 0;
};

