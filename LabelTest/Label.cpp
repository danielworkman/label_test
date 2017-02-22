#include "stdafx.h"
#include "Label.h"

void Label::CreateBoundingBox()
{
	const double halfWidth = m_width / 2.0;
	const double halfHeight = m_height / 2.0;

	Path::VectorList points = {
		Vector2(-halfWidth, -halfHeight),
		Vector2(halfWidth, -halfHeight),
		Vector2(halfWidth, halfHeight),
		Vector2(-halfWidth, halfHeight),
		Vector2(-halfWidth, -halfHeight)
	};

	m_box = Path(points);
}
//-----------------------------------------------------------------------------

double Label::GetAdjustedPercentage(
	const double percentage,
	const double percentageCovered) const
{
	double adjustedPercentage = percentage;

	// Don't allow the label to run off the end of the path
	if (percentage + percentageCovered > 1.0)
	{
		adjustedPercentage = 1.0 - percentageCovered;
	}

	// The entire label is longer than the path so force to beginning of path
	if (percentageCovered > 1.0)
	{
		adjustedPercentage = 0.0;
	}

	// Force first label letter to start on the path
	if (percentage < 0.0)
	{
		adjustedPercentage = 0.0;
	}

	return adjustedPercentage;
}
//-----------------------------------------------------------------------------

std::vector<double> Label::GetLetterCentres(
	const Path& path,
	const double adjustedPercentage,
	const double halfLetterCoverage,
	const double spacingCoverage) const
{
	std::vector<double> centres = { adjustedPercentage + halfLetterCoverage };

	for (size_t i = 1; i < GetLetterCount(); ++i)
	{
		const double centre = centres.back() + (2 * halfLetterCoverage) + spacingCoverage;

		if (centre >= 0.0 && centre <= 1.0)
			centres.push_back(centre);
	}

	return centres;
}
//-----------------------------------------------------------------------------

std::vector<Path> Label::GetLetters(const Path& path, const double percentage) const
{
 	std::vector<Path> letters;

	if (GetLetterCount() == 0)
		return letters;

	const double totalLabelWidth = (GetWidth() + GetSpacing()) * GetLetterCount() - GetSpacing();
	const double percentageCovered = 1.0 - (path.Length() - totalLabelWidth) / path.Length();
	const double letterCoverage = 1.0 - (path.Length() - GetWidth()) / path.Length();
	const double spacingCoverage = 1.0 - (path.Length() - GetSpacing()) / path.Length();

	const double adjustedPercentage = GetAdjustedPercentage(percentage, percentageCovered);
	std::vector<double> centres = GetLetterCentres(path, adjustedPercentage, letterCoverage / 2.0, spacingCoverage);

	for (auto itr = centres.begin(); itr != centres.end(); ++itr)
	{
		Edge edge;
		if (!path.GetEdge(*itr, edge))
		{
			return letters;
		}

		const Vector2 point = path.PointOnPath(*itr);
		const Vector2 tangent = edge.GetTangent(point);

		const double angleBetween = Vector2(0.0, -1.0).GetAngle(tangent);

		// Rotate the letter box around it's centre
		Path newBox = GetBoundingBox().Rotate(Vector2(0.0, 0.0), angleBetween - Utils::DegToRad(90.0));

		// Move to the correct position on the path
		letters.push_back(newBox.Translate(point));
	}

	return letters;
}