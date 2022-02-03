#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>


namespace ssuds
{

	class City : public sf::CircleShape
	{

	protected:

		// attributes
		std::string mData;
		int mX;
		int mY;
		float mRadius;
		int mIndex;
		bool mIsActive;

	public:

		// constructor
		City(std::string data, int x, int y, float radius, int index);

		// getters
		std::string get_data();

		int get_x();

		int get_y();

		float get_radius();

		int get_index();

		bool isActive();

		sf::Vector2f get_center();

		// setters

		void change_active();

		void set_false();
	};

}