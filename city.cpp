#include <city.h>


ssuds::City::City(std::string data, int x, int y, float radius, int index) : sf::CircleShape(radius)
{
	mData = data;
	mX = x;
	mY = y;
	mRadius = radius;
	mIndex = index;
	mIsActive = false;
}

std::string ssuds::City::get_data()
{
	return mData;
}

int ssuds::City::get_x()
{
	return mX;
}

int ssuds::City::get_y()
{
	return mY;
}

float ssuds::City::get_radius()
{
	return mRadius;
}

int ssuds::City::get_index()
{
	return mIndex;
}

sf::Vector2f ssuds::City::get_center()
{
	int x = mX + mRadius;
	int y = mY + mRadius;
	return sf::Vector2f(x, y);
}

bool ssuds::City::isActive()
{
	return mIsActive;
}

void ssuds::City::change_active()
{
	mIsActive = !mIsActive;
}

void ssuds::City::set_false()
{
	mIsActive = false;
}

