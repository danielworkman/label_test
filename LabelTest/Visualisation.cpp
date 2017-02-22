#include "stdafx.h"
#include "Visualisation.h"
#include "Utils.h"
#include <math.h>

Visualisation::Visualisation(CDC* dc) : m_dc(dc)
{
	m_brush = (CBrush*)m_dc->SelectStockObject(NULL_BRUSH);
	m_pen = (CPen*)m_dc->SelectStockObject(BLACK_PEN);
}
//-----------------------------------------------------------------------------

void Visualisation::DrawEdge(const Edge& edge)
{
	if (!m_dc)
		return;

	if (edge.IsArc())
	{
		const Vector2& centre = edge.GetCircleCentre();
		const double radius = edge.GetRadius();

		CRect rect(Utils::Round(centre.x - radius),
				   Utils::Round(centre.y - radius),
				   Utils::Round(centre.x + radius),
				   Utils::Round(centre.y + radius));

		m_dc->SetArcDirection(edge.GetClockwise() ? AD_CLOCKWISE : AD_COUNTERCLOCKWISE);
		m_dc->Arc(rect, edge.GetP2().ToPoint(), edge.GetP1().ToPoint());
	}
	else
	{
		m_dc->MoveTo(edge.GetP1().ToPoint());
		m_dc->LineTo(edge.GetP2().ToPoint());
	}
}
//-----------------------------------------------------------------------------

void Visualisation::DrawPath(const Path& path)
{
	if (!m_dc)
		return;

	for (auto itr = path.begin(); itr != path.end(); ++itr)
	{
		DrawEdge(*itr);
	}
}
//-----------------------------------------------------------------------------

void Visualisation::DrawPaths(const std::vector<Path>& paths)
{
	if (!m_dc)
		return;

	for (auto itr = paths.begin(); itr != paths.end(); ++itr)
	{
		DrawPath(*itr);
	}
}
//-----------------------------------------------------------------------------

void Visualisation::DrawPoint(const Vector2& point)
{
	if (!m_dc)
		return;

	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	CPen* oldPen = (CPen*)m_dc->SelectObject(&pen);

	const POINT& pointp = point.ToPoint();

	m_dc->Rectangle(pointp.x - 3, pointp.y - 3, pointp.x + 3, pointp.y + 3);

	m_dc->SelectObject(oldPen);
}
//-----------------------------------------------------------------------------

void Visualisation::DrawText(const Vector2& point, const std::string& text)
{
	if (!m_dc)
		return;

	RECT rect = {	Utils::Round(point.x),
					Utils::Round(point.y),
					Utils::Round(point.x + 100),
					Utils::Round(point.y + 100) };

	m_dc->DrawText(CString(text.c_str()), &rect, DT_SINGLELINE);
}