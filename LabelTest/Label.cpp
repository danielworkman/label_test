#include "stdafx.h"
#include "Label.h"

void Label::CreateBoundingBox()
{
	const double halfWidth = m_width / 2.0;
	const double halfHeight = m_height / 2.0;

	Path::VectorRadiusList points = {
		Vector2(-halfWidth, -halfHeight),
		Vector2(halfWidth, -halfHeight),
		Vector2(halfWidth, halfHeight),
		Vector2(-halfWidth, halfHeight),
		Vector2(-halfWidth, -halfHeight)
	};

	m_box = Path(points);
}
//-----------------------------------------------------------------------------

std::vector<Path> Label::GetLetters(const Path& path, double percentage) const
{
 	std::vector<Path> letters;

	if (GetLength() <= 0)
		return letters;

	const double totalMinWidth = (GetWidth() + GetSpacing()) * GetLength();
	const double minPercentage = 1.0 - (path.Length() - totalMinWidth) / path.Length();
	const double percentageSplit = minPercentage / GetLength();

	if (percentage + minPercentage - percentageSplit > 1.0)
	{
		percentage = 1.0 - minPercentage + percentageSplit;
	}

	for (size_t i = 0; i < GetLength(); ++i)
	{
		const double letterPercentage = percentage + (i * percentageSplit);

		Edge edge;
		if (!path.GetEdge(letterPercentage, edge))
		{
			return letters;
		}

		const Vector2 point = path.PointOnPath(letterPercentage);
		const Vector2 tangent = edge.GetTangent(point);

		const double angleBetween = Vector2(0.0, -1.0).GetAngle(tangent);

		Path newBox = GetBoundingBox().Rotate(Vector2(), angleBetween - Utils::DegToRad(90.0));
		letters.push_back(newBox.Translate(point));
	}

	return letters;
}