#include <level.h>
#include <city.h>
#include <iostream>
#include <fstream>
#include <sstream>


// constructor
ssuds::Level::Level(std::string fname)
{
	mFname = fname;
	parse_file();
	set_values();
}

void ssuds::Level::parse_file()
{

	std::fstream fp(mFname, std::ios::in);

	if (fp.is_open())
	{
		int index = 0;
		while (!fp.eof())
		{
			std::string line;
			getline(fp, line);
			if (line[0] == 'n')
			{
				// node data
				std::istringstream iss(line);
				std::string pre;
				std::string data;
				int x;
				int y;
				float radius;
				iss >> pre >> data >> x >> y >> radius;
				if (!fp.eof())
				{
					ssuds::City* tempCity = new ssuds::City(data, x, y, radius, index);
					mGraph.add_node(tempCity);
					index++;
				}
			}

			if (line[0] == 'e')
			{
				// edge data
				std::istringstream iss(line);
				std::string pre;
				int start_index;
				int end_index;
				float data;
				iss >> pre >> start_index >> end_index >> data;
				if (!fp.eof())
				{
					ssuds::City* startCity;
					ssuds::City* endCity;
					for (int i = 0; i < size(); i++)
					{
						if (mGraph.get_node(i)->get_index() == start_index)
						{
							startCity = mGraph.get_node(i);
						}
						if (mGraph.get_node(i)->get_index() == end_index)
						{
							endCity = mGraph.get_node(i);
						}
					}
					mGraph.add_edge(startCity, endCity, data);
				}
			}
		}
	}

	else
	{
		std::cout << "Failed to open " << mFname << std::endl;
	}

}

void ssuds::Level::set_values()
{
	int num_cities = size();
	for (int i = 0; i < num_cities; i++)
	{
		ssuds::City* curCity = mGraph.get_node(i);
		curCity->setPosition(sf::Vector2f(curCity->get_x(), curCity->get_y()));
		curCity->setFillColor(sf::Color::Yellow);
		curCity->setRadius(curCity->get_radius());
		if (curCity->isActive())
		{
			curCity->setOutlineColor(sf::Color::Blue);
			curCity->setOutlineThickness(3);
		}
		else
			curCity->setOutlineThickness(0);
	}
}

void ssuds::Level::change_active(ssuds::City* city)
{
	for (int i = 0; i < size(); i++)
	{
		if (get_city(i)->get_data() != city->get_data())
		{
			get_city(i)->set_false();
		}
	}
	mActiveCity = NULL;
	city->change_active();
	if (city->isActive())
		mActiveCity = city;
}

int ssuds::Level::size()
{
	return mGraph.get_num_nodes();
}

int ssuds::Level::get_num_neighbors(City* city)
{
	return mGraph.get_num_neighbors(city);
}

ssuds::City* ssuds::Level::get_city(int index)
{
	return mGraph.get_node(index);
}

ssuds::City* ssuds::Level::get_neighbor(City* city, int index)
{
	return mGraph.get_neighbor(city, index);
}

float ssuds::Level::get_edge(City* start, City* end)
{
	return mGraph.get_edge(start, end);
}

void ssuds::Level::draw_edges(sf::RenderWindow& win, sf::Font& font)
{
	for (int i = 0; i < size(); i++)
	{
		ssuds::City* curCity = get_city(i);
		int num_neighbors = get_num_neighbors(curCity);
		for (int j = 0; j < num_neighbors; j++)
		{
			ssuds::City* otherCity = get_neighbor(curCity, j);
			sf::VertexArray line(sf::LinesStrip, 2);
			line[0].position = curCity->get_center();
			line[0].color = sf::Color::Red;
			line[1].position = otherCity->get_center();
			line[1].color = sf::Color::Red;
			win.draw(line);

			// gross arrow head math
			if (otherCity->get_center().x - curCity->get_center().x == 0)
			{
				// verticle line
				float distance = otherCity->get_radius();
				if (curCity->get_center().y < otherCity->get_center().y)
					distance *= -1.0;
				int tipX = otherCity->get_center().x;
				int tipY = distance + otherCity->get_center().y;
				float slope1 = 1;
				float slope2 = -1;
				int X1 = (distance * (1 / sqrt(1 + pow(slope1, 2)))) + tipX;
				int Y1 = (distance * (slope1 / sqrt(1 + pow(slope1, 2)))) + tipY;
				int X2 = ((distance * -1) * (1 / sqrt(1 + pow(slope1, 2)))) + tipX;
				int Y2 = ((distance * -1) * (slope1 / sqrt(1 + pow(slope1, 2)))) + tipY;
				int X3 = (distance * (1 / sqrt(1 + pow(slope2, 2)))) + tipX;
				int Y3 = (distance * (slope2 / sqrt(1 + pow(slope2, 2)))) + tipY;
				int X4 = ((distance * -1) * (1 / sqrt(1 + pow(slope2, 2)))) + tipX;
				int Y4 = ((distance * -1) * (slope2 / sqrt(1 + pow(slope2, 2)))) + tipY;
				sf::VertexArray line1(sf::LinesStrip, 2);
				line1[0].position = sf::Vector2f(X1, Y1);
				line1[0].color = sf::Color::Red;
				line1[1].position = sf::Vector2f(X2, Y2);
				line1[1].color = sf::Color::Red;
				win.draw(line1);

				sf::VertexArray line2(sf::LinesStrip, 2);
				line2[0].position = sf::Vector2f(X3, Y3);
				line2[0].color = sf::Color::Red;
				line2[1].position = sf::Vector2f(X4, Y4);
				line2[1].color = sf::Color::Red;
				win.draw(line2);

			}

			float slope = (otherCity->get_center().y - curCity->get_center().y) * 1.0 / (otherCity->get_center().x - curCity->get_center().x);
			float distance = otherCity->get_radius();
			if (curCity->get_center().x < otherCity->get_center().x)
				distance *= -1.0;
			int tipX = (distance * (1 / sqrt(1 + pow(slope, 2)))) + otherCity->get_center().x;
			int tipY = (distance * (slope / sqrt(1 + pow(slope, 2)))) + otherCity->get_center().y;
			float angle = atan(slope);
			float slope1 = tan(angle + (M_PI / 4));
			float slope2 = tan(angle - (M_PI / 4));
			int X1 = (distance * (1 / sqrt(1 + pow(slope1, 2)))) + tipX;
			int Y1 = (distance * (slope1 / sqrt(1 + pow(slope1, 2)))) + tipY;
			int X2 = ((distance * -1) * (1 / sqrt(1 + pow(slope1, 2)))) + tipX;
			int Y2 = ((distance * -1) * (slope1 / sqrt(1 + pow(slope1, 2)))) + tipY;
			int X3 = (distance * (1 / sqrt(1 + pow(slope2, 2)))) + tipX;
			int Y3 = (distance * (slope2 / sqrt(1 + pow(slope2, 2)))) + tipY;
			int X4 = ((distance * -1) * (1 / sqrt(1 + pow(slope2, 2)))) + tipX;
			int Y4 = ((distance * -1) * (slope2 / sqrt(1 + pow(slope2, 2)))) + tipY;
			sf::VertexArray line1(sf::LinesStrip, 2);
			line1[0].position = sf::Vector2f(X1, Y1);
			line1[0].color = sf::Color::Red;
			line1[1].position = sf::Vector2f(X2, Y2);
			line1[1].color = sf::Color::Red;
			win.draw(line1);

			sf::VertexArray line2(sf::LinesStrip, 2);
			line2[0].position = sf::Vector2f(X3, Y3);
			line2[0].color = sf::Color::Red;
			line2[1].position = sf::Vector2f(X4, Y4);
			line2[1].color = sf::Color::Red;
			win.draw(line2);
		}
	}
}

void ssuds::Level::draw_cities(sf::RenderWindow& win, sf::Font& font)
{
	for (int i = 0; i < size(); i++)
	{
		win.draw(*get_city(i));
	}
}

void ssuds::Level::draw_text(sf::RenderWindow& win, sf::Font& font)
{
	// draw edge values
	for (int i = 0; i < size(); i++)
	{
		ssuds::City* cur_city = get_city(i);
		int num_neighbors = get_num_neighbors(cur_city);
		for (int j = 0; j < num_neighbors; j++)
		{
			sf::Text val;
			val.setFont(font);
			ssuds::City* other = get_neighbor(cur_city, j);
			std::string str;
			std::ostringstream convert;
			convert << get_edge(cur_city, other);
			str = convert.str();
			val.setString(str);
			val.setCharacterSize(12);
			val.setFillColor(sf::Color::White);
			val.setOutlineColor(sf::Color::Black);
			val.setOutlineThickness(12 / 10);
			sf::Vector2f midpoint;
			midpoint.x = (cur_city->get_center().x + other->get_center().x) / 2.0;
			midpoint.y = (cur_city->get_center().y + other->get_center().y) / 2.0;
			sf::FloatRect val_rect = val.getLocalBounds();
			val.setOrigin(val_rect.left + (val_rect.width / 2.0f), val_rect.top + (val_rect.height / 2.0f));
			val.setPosition(midpoint);
			win.draw(val);
		}
	}
	// draw city names
	for (int i = 0; i < size(); i++)
	{
		sf::Text name;
		ssuds::City* cur_city = get_city(i);
		float radius = cur_city->get_radius();
		if (radius < 15)
			radius = 15;
		name.setFont(font);
		name.setString(cur_city->get_data());
		name.setCharacterSize(radius);
		name.setFillColor(sf::Color::White);
		name.setOutlineColor(sf::Color::Black);
		name.setOutlineThickness(cur_city->get_radius() / 10);
		sf::Vector2f center = cur_city->get_center();
		sf::FloatRect name_rect = name.getLocalBounds();
		name.setOrigin(name_rect.left + (name_rect.width / 2.0f), name_rect.top + (name_rect.height / 2.0f));
		name.setPosition(center);
		win.draw(name);
	}
}
