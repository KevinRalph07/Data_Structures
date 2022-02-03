#pragma once
#include <graph.h>
#include <city.h>

# define M_PI           3.14159265358979323846  /* pi */


namespace ssuds
{

	class Level
	{

	protected:

		// attributes
		std::string mFname;
		ssuds::Graph<ssuds::City*, float> mGraph;
		ssuds::City* mActiveCity;

	public:

		// constructor
		Level(std::string fname);

		// file methods
		void parse_file();

		// setter
		void set_values();

		void change_active(ssuds::City* city);

		// getters
		int size();

		int get_num_neighbors(City* city);

		ssuds::City* get_city(int index);

		ssuds::City* get_neighbor(City* city, int index);

		float get_edge(City* start, City* end);

		// draw
		void draw_edges(sf::RenderWindow& win, sf::Font& font);

		void draw_cities(sf::RenderWindow& win, sf::Font& font);

		void draw_text(sf::RenderWindow& win, sf::Font& font);

	};

}