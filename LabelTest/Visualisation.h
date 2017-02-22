#pragma once

#include "stdafx.h"
#include "Geometry.h"


class Visualisation
{
public:
	Visualisation() : m_dc(nullptr), m_pen(nullptr), m_brush(nullptr) {};
	Visualisation(CDC* dc);

	void SelectPen(CPen* pen) { m_pen = pen; m_dc->SelectObject(pen); }
	void SelectBrush(CBrush* brush) { m_brush = brush; m_dc->SelectObject(brush); }

	CPen* GetPen() { return m_pen; }
	CBrush* GetBrush() { return m_brush; }

	void DrawPath(const Path& path);
	void DrawPaths(const std::vector<Path>& path);

	void DrawEdge(const Edge& edge);
	void DrawPoint(const Vector2& point);
	void DrawMidPoint(const Edge& edge);

	void DrawText(const Vector2& point, const std::string& text);

private:
	CDC* m_dc;

	CPen* m_pen;
	CBrush* m_brush;
};