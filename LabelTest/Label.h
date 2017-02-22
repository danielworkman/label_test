#pragma once

#include <string>
#include "Geometry.h"

class Label
{
public:
	Label(const size_t letters, const int height, const int width, const int spacing)
		: m_letters(letters),
		  m_height(height),
		  m_width(width),
		  m_spacing(spacing)
	{
		CreateBoundingBox();
	};

	size_t GetLetterCount() const { return m_letters; }
	int GetHeight() const { return m_height; }
	int GetWidth() const { return m_width; }
	int GetSpacing() const { return m_spacing; }

	const Path& GetBoundingBox() const { return m_box; };
	Path GetBoundingBox(const double rotation) const { return m_box.Rotate(Vector2(), rotation); };

	std::vector<Path> GetLetters(const Path& path, const double percentage) const;

private:
	void CreateBoundingBox();

	size_t m_letters;
	int m_height;
	int m_width;
	int m_spacing;

	Path m_box;
};