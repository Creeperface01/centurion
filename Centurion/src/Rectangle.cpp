#include "rectangle.h"
#include "boolean_converter.h"
#include "point.h"
#include <stdexcept>

using centurion::Rectangle;
using centurion::Point;
using centurion::BooleanConverter;

Rectangle::Rectangle(int x, int y, int w, int h)
{
	if (w < 1 || h < 1) {
		throw std::invalid_argument("Invalid dimensions for rectangle!");
	}
	rect = { x, y, w, h };
}

Rectangle::Rectangle(int w, int h) : Rectangle(0, 0, w, h)
{}

void Rectangle::SetLocation(int x, int y)
{
	SetX(x);
	SetY(y);
}

void Rectangle::SetX(int x)
{
	rect.x = x;
}

void Rectangle::SetY(int y)
{
	rect.y = y;
}

bool Rectangle::Intersects(Rectangle& otherRect)
{
	SDL_bool result = SDL_HasIntersection(&this->rect, &otherRect.rect);
	return BooleanConverter::Convert(result);
}

bool Rectangle::Contains(int x, int y)
{
	SDL_Point point = { x, y };
	SDL_bool result = SDL_PointInRect(&point, &this->rect);
	return BooleanConverter::Convert(result);
}

bool Rectangle::Contains(Point& point)
{
	return Contains(point.GetX(), point.GetY());
}

int Rectangle::GetX()
{
	return rect.x;
}

int Rectangle::GetY()
{
	return rect.y;
}

int Rectangle::GetWidth()
{
	return rect.w;
}

int Rectangle::GetHeight()
{
	return rect.h;
}

SDL_Rect Rectangle::CreateSDLRect()
{
	return rect;
}