#pragma once

#include <vector>
#include "Utils.h"

struct Vector2
{
	Vector2() : x(0.0), y(0.0) {};
	Vector2(const double x, const double y) : x(x), y(y) {};

	bool Equals(const Vector2& rhs) const
	{
		return (Utils::DoubleCompare(x, rhs.x) && Utils::DoubleCompare(y, rhs.y));
	}

	Vector2 operator*(const double& rhs) const
	{
		return Vector2(x * rhs, y * rhs);
	}

	Vector2 operator+(const Vector2& rhs) const
	{
		return Vector2(x + rhs.x, y + rhs.y);
	}

	Vector2 operator-(const Vector2& rhs) const
	{
		return Vector2(x - rhs.x, y - rhs.y);
	}

	double DotProduct(const Vector2& v) const
	{
		return ((x * v.x) + (y * v.y));
	}

	Vector2 Normalise() const
	{
		double length = GetMagnitude();
		if (length == 0.0)
			return *this;
		return Vector2(x / length, y / length);
	}

	double GetMagnitude() const
	{
		return sqrt(DotProduct(*this));
	}

	Vector2 Rotate(const Vector2& origin, const double& theta) const
	{
		const double costheta = cos(theta);
		const double sintheta = sin(theta);

		Vector2 r = *this - origin;
		Vector2 result = { (r.x * costheta) - (r.y * sintheta), (r.x * sintheta) + (r.y * costheta) };

		return result + origin;
	}

	Vector2 GetPerpendicular() const
	{
		return Vector2(-y, x);
	}

	Vector2 Inverse() const
	{
		return Vector2(-x, -y);
	}

	double DistanceTo(const Vector2& other) const
	{
		return (*this - other).GetMagnitude();
	}

	double GetAngle(const Vector2& v) const
	{
		double val = DotProduct(v) / (GetMagnitude() * v.GetMagnitude());
		if (val >= -1.0 && val <= 1.0)
		{
			return acos(val);
		}
		return 0.0;
	}

	POINT ToPoint() const
	{
		return { Utils::Round(x), Utils::Round(y) };
	}

	double x;
	double y;
};

class Edge
{
public:
	Edge()
		: m_radius(0.0),
		  m_clockwise(false),
		  m_large(false) {};

	Edge(const Vector2& p1, const Vector2& p2)
		: m_p1(p1),
		  m_p2(p2),
		  m_radius(0.0),
		  m_clockwise(false),
		  m_large(false) {};

	Edge(const Vector2& p1, const Vector2& p2, const double radius, const bool clockwise=true, const bool large=false)
		: m_p1(p1),
		  m_p2(p2),
		  m_radius(radius),
		  m_clockwise(clockwise),
		  m_large(large)
	{
		m_circleCentre = CalcCircleCentre();
	};

	const Vector2& GetP1() const { return m_p1; }
	const Vector2& GetP2() const { return m_p2; }
	double GetRadius() const { return m_radius; }
	bool GetClockwise() const { return m_clockwise; }
	bool GetLarge() const { return m_large; }

	bool IsArc() const { return m_radius > Utils::DOUBLE_EPSILON; }

	Vector2 GetNormal(Vector2 pt) const;
	Vector2 GetTangent(Vector2 pt) const;
	Edge Translate(const Vector2& transform) const;

	Vector2 GetCircleCentre() const { return m_circleCentre; }
	double Length() const;
	double GetSweep() const;

	Vector2 PointOnEdge(const double offset) const;

	bool Equals(const Edge& other) const;
	Vector2 MidPoint() const;

	Edge Rotate(const Vector2& origin, const double rotation) const;

private:
	Vector2 CalcCircleCentre() const;

	Vector2 m_p1;
	Vector2 m_p2;
	Vector2 m_circleCentre;
	double m_radius;
	bool m_clockwise;
	bool m_large;
};

class Path
{
public:
	struct RadiusInfo
	{
		RadiusInfo(const Vector2& point) : point(point), radius(0.0), clockwise(false), large(false) {};
		RadiusInfo(const Vector2& point, const double radius, const bool clockwise = true, const bool large = false)
			: point(point),
			  radius(radius),
			  clockwise(clockwise),
			  large(large) {};

		Vector2 point;
		double radius;
		bool clockwise;
		bool large;
	};

	using EdgeList = std::vector<Edge>;
	using const_iterator = EdgeList::const_iterator;

	using VectorList = std::vector<Vector2>;
	using VectorRadiusList = std::vector<RadiusInfo>;

	Path() = default;
	Path(const EdgeList& edges) : m_edges(edges) {};
	Path(const VectorList& vecList) { AddEdges(vecList); };
	Path(const VectorRadiusList& vecList) { AddEdges(vecList); };

	const Edge& front() const { return m_edges.front(); }
	const Edge& back() const { return m_edges.back(); }
	const_iterator begin() const { return m_edges.begin(); }
	const_iterator end() const { return m_edges.end(); }

	size_t size() const { return m_edges.size(); }
	bool empty() const { return m_edges.empty(); }

	double Length() const;

	void AddEdge(const Edge& edge) { m_edges.push_back(edge); };
	bool AddEdge(const Vector2& vec);

	bool AddEdges(const VectorList& vecList);
	bool AddEdges(const VectorRadiusList& vecList);

	Path Translate(const Vector2& translation) const;
	Path Rotate(const Vector2& origin, const double rotation) const;

	bool GetEdge(const double position, Edge& outEdge) const;
	Vector2 PointOnPath(const double position) const;

private:
	EdgeList m_edges;
};