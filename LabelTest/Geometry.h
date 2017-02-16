#pragma once

#include <vector>

struct Point
{
	Point() : x(0.0), y(0.0) {};
	Point(const int x, const int y) : x(x), y(y) {};

	double x;
	double y;
};

class Edge
{
public:
	Edge(const Point& p1, const Point& p2)
		: m_p1(p1),
		  m_p2(p2),
		  m_radius(0.0),
		  m_clockwise(false),
		  m_large(false) {};

	Edge(const Point& p1, const Point& p2, const double radius, const bool clockwise, const bool large)
		: m_p1(p1),
		  m_p2(p2),
		  m_radius(radius),
		  m_clockwise(clockwise),
		  m_large(large) {};

private:
	Point m_p1;
	Point m_p2;
	double m_radius;
	double m_clockwise;
	double m_large;
};

class Path
{
public:
	void AddEdge(const Edge& point);

private:
	std::vector<Edge> m_edges;
};