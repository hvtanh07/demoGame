#include "Camera.h"

Camera::Camera(int w, int h/*, int b_left, int b_right*/)
{
	_width = w;
	_height = h;

	// Biên mặc định ban đầu là kích thước MAP
	_boundaryLeft = 0;
	_boundaryRight = 0;// (float)(MapWidth - SCREEN_WIDTH);

	vx = 0;
}

Camera::~Camera()
{
}
void Camera::Update(DWORD dt)
{
	this->dt = dt;

	if (_xCam < _boundaryLeft)
		_xCam = _boundaryLeft;

	if (_xCam > _boundaryRight)
		_xCam = _boundaryRight;

}


void Camera::SetPosition(float x, float y)
{
	_xCam = x;
	_yCam = y;
}
