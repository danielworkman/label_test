#include "stdafx.h"
#include "Geometry.h"

Vector2 Edge::GetNormal(Vector2 pt) const
{
	if (IsArc())
	{
		return (GetCircleCentre() - pt).Normalise();
	}

	return GetTangent(pt).GetPerpendicular();
}
//-----------------------------------------------------------------------------

Vector2 Edge::GetTangent(Vector2 pt) const
{
	if (IsArc())
	{
		return GetClockwise() ? GetNormal(pt).GetPerpendicular() :
								GetNormal(pt).GetPerpendicular().Inverse();
	}

	return (GetP2() - GetP1()).Normalise();
}
//-----------------------------------------------------------------------------

Edge Edge::Translate(const Vector2& transform) const
{
	return Edge(GetP1() + transform,
				GetP2() + transform,
				GetRadius(),
				GetClockwise(),
				GetLarge());
}
//-----------------------------------------------------------------------------

Vector2 Edge::CalcCircleCentre() const
{
	// Algorithm taken from
	// http://mathforum.org/library/drmath/view/53027.html

	const double radius = GetRadius();

	if (Utils::DoubleCompare(radius, 0.0))
		return MidPoint();

	const double length = GetP1().DistanceTo(GetP2());

	const Vector2& mid = Edge(GetP1(), GetP2()).MidPoint();

	const double x = sqrt(pow(radius, 2) - pow((length / 2), 2))*(GetP1().y - GetP2().y) / length;
	const double y = sqrt(pow(radius, 2) - pow((length / 2), 2))*(GetP2().x - GetP1().x) / length;

	if (GetLarge())
	{
		return Vector2(mid.x + x, mid.y + y);
	}

	return Vector2(mid.x - x, mid.y - y);
}
//-----------------------------------------------------------------------------

double Edge::Length() const
{
	if (IsArc())
	{
		return GetRadius() * GetSweep();
	}

	return m_p1.DistanceTo(m_p2);
}
//-----------------------------------------------------------------------------

Vector2 Edge::MidPoint() const
{
	if (GetP1().Equals(GetP2()))
	{
		return GetP1();
	}

	if (!IsArc())
	{
		return GetP1() + ((GetP2() - GetP1()) * 0.5);
	}

	const Vector2& centre = GetCircleCentre();
	double sweep = GetSweep();
	double angle = 0.5 * sweep;

	// clockwise winding wants negative sweep
	if (GetClockwise())
	{
		angle *= -1.0;
	}

	return Vector2(centre.x + GetRadius() * cos(angle), centre.y + GetRadius() * sin(angle));
}
//-----------------------------------------------------------------------------

double Edge::GetSweep() const
{
	if (GetP1().Equals(GetP2()))
	{
		return 0.0;
	}

	const Vector2& centre = GetCircleCentre();

	Vector2 v1Dash = GetP1() - centre;
	Vector2 v2Dash = GetP2() - centre;
	
	if (GetClockwise())
	{
		v1Dash.y = -v1Dash.y;
		v2Dash.y = -v2Dash.y;
	}

	double extent = atan2(v2Dash.y, v2Dash.x) - atan2(v1Dash.y, v1Dash.x);

	if (extent < -Utils::DOUBLE_EPSILON)
	{
		extent += 2 * Utils::M_PI;
	}

	return extent;
}
//-----------------------------------------------------------------------------

Vector2 Edge::PointOnEdge(const double offset) const
{
	if (GetP1().Equals(GetP2()))
		return GetP1();

	if (IsArc())
	{
		const double sweep = GetSweep();

		double t = sweep * offset;
		if (GetClockwise())
		{
			t = -t;
		}

		return GetP1().Rotate(GetCircleCentre(), t);
	}

	return GetP1() + GetTangent(GetP1()) * (offset * Length());
}
//-----------------------------------------------------------------------------

bool Edge::Equals(const Edge& other) const
{
	if (!GetP1().Equals(other.GetP1())) return false;
	if (!GetP2().Equals(other.GetP2())) return false;
	if (!Utils::DoubleCompare(GetRadius(), other.GetRadius())) return false;
	if (GetClockwise() != other.GetClockwise()) return false;
	if (GetLarge() != other.GetLarge()) return false;

	return true;
}
//-----------------------------------------------------------------------------

Edge Edge::Rotate(const Vector2& origin, const double rotation) const
{
	return Edge(GetP1().Rotate(origin, rotation), GetP2().Rotate(origin, rotation));
}
//-----------------------------------------------------------------------------

double Path::Length() const
{
	double length = 0.0;

	for (auto itr = begin(); itr != end(); ++itr)
	{
		length += itr->Length();
	}

	return length;
}
//-----------------------------------------------------------------------------

bool Path::AddEdge(const Vector2& vec)
{
	if (empty())
		return false;

	AddEdge(Edge(back().GetP2(), vec));
	return true;
}
//-----------------------------------------------------------------------------

bool Path::AddEdges(const VectorList& vecList)
{
	if (vecList.size() < 2)
		return false;

	Vector2 p1 = vecList.front();

	for (auto itr = vecList.begin() + 1; itr != vecList.end(); ++itr)
	{
		AddEdge(Edge(p1, *itr));
		p1 = *itr;
	}

	return true;
}
//-----------------------------------------------------------------------------

bool Path::AddEdges(const VectorArcList& vecList)
{
	if (vecList.size() < 2)
		return false;

 	ArcInfo p1 = vecList.front();
 
 	for (auto itr = vecList.begin() + 1; itr != vecList.end(); ++itr)
 	{
 		AddEdge(Edge(p1.point, itr->point, itr->radius, itr->clockwise, itr->large));
 		p1 = *itr;
 	}

	return true;
}
//-----------------------------------------------------------------------------

bool Path::GetEdge(const double position, Edge& outEdge) const
{
	if (empty())
		return false;

	const double desiredDistance = Length() * position;

	double total = 0.0;

	for (auto itr = begin(); itr != end(); ++itr)
	{
		const double edgeLength = itr->Length();
		total += edgeLength;

		if (total > desiredDistance)
		{
			outEdge = *itr;
			return true;
		}
	}

	if (Utils::DoubleCompare(total, desiredDistance))
	{
		outEdge = back();
		return true;
	}

	outEdge = front();
	return true;
}
//-----------------------------------------------------------------------------

Vector2 Path::PointOnPath(const double position) const
{
	if (empty())
		return Vector2();

	const double desiredDistance = Length() * position;

	double total = 0.0;

	for (auto itr = begin(); itr != end(); ++itr)
	{
		const double edgeLength = itr->Length();
		total += edgeLength;

		if (total > desiredDistance)
		{
			const double difference = desiredDistance - (total - edgeLength);
			const double percentage = (edgeLength - (edgeLength - difference)) / edgeLength;
			return itr->PointOnEdge(percentage);
		}
	}

	if (Utils::DoubleCompare(total, desiredDistance))
	{
		return back().GetP2();
	}

	return Vector2();
}
//-----------------------------------------------------------------------------

Path Path::Translate(const Vector2& translation) const
{
	Path path;
	for (const_iterator edge = begin(); edge != end(); ++edge)
	{
		path.AddEdge(edge->Translate(translation));
	}
	return path;
}
//-----------------------------------------------------------------------------

Path Path::Rotate(const Vector2& origin, const double rotation) const
{
	if (Utils::DoubleCompare(rotation, 0.0))
		return *this;

	Path path;
	for (const_iterator edge = begin(); edge != end(); ++edge)
	{
		path.AddEdge(edge->Rotate(origin, rotation));
	}
	return path;
}