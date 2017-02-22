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

	if (GetLetterCount() == 0)
		return letters;

	const double totalLabelWidth = (GetWidth() + GetSpacing()) * GetLetterCount();
	const double percentageCovered = 1.0 - (path.Length() - totalLabelWidth) / path.Length();
	const double letterPercentage = percentageCovered / GetLetterCount();
	const double letterOffset = letterPercentage / 2.0;

	// Force first label letter to start on the path
	if (percentage - letterOffset <= 0.0)
	{
		percentage = letterOffset;
	}

	// Don't allow the label to run off the end of the path
	if (percentage + percentageCovered - letterOffset > 1.0)
	{
		percentage = 1.0 - percentageCovered + letterOffset;
	}

	for (size_t i = 0; i < GetLetterCount(); ++i)
	{
		const double currentPercentage = percentage + (i * letterPercentage);

		Edge edge;
		if (!path.GetEdge(currentPercentage, edge))
		{
			return letters;
		}

		const Vector2 point = path.PointOnPath(currentPercentage);
		const Vector2 tangent = edge.GetTangent(point);

		const double angleBetween = Vector2(0.0, -1.0).GetAngle(tangent);

		// Rotate the letter box around it's centre
		Path newBox = GetBoundingBox().Rotate(Vector2(0.0, 0.0), angleBetween - Utils::DegToRad(90.0));

		letters.push_back(newBox.Translate(point));
	}

	return letters;
}