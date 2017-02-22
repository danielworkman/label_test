#pragma once

#include <string>
#include "Geometry.h"

class Label
{
public:
	Label(const size_t length, const int height, const int width, const int spacing)
		: m_length(length),
		  m_height(height),
		  m_width(width),
		  m_spacing(spacing)
	{
		CreateBoundingBox();
	};

	size_t GetLength() const { return m_length; }
	int GetHeight() const { return m_height; }
	int GetWidth() const { return m_width; }
	int GetSpacing() const { return m_spacing; }

	const Path& GetBoundingBox() const { return m_box; };
	Path GetBoundingBox(const double rotation) const { return m_box.Rotate(Vector2(), rotation); };

	std::vector<Path> GetLetters(const Path& path, const double percentage) const;

private:
	void CreateBoundingBox();

	size_t m_length;
	int m_height;
	int m_width;
	int m_spacing;

	Path m_box;
};