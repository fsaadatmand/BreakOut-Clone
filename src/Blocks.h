#ifndef BLOCKS_H
#define BLOCKS_H

#include "../include/olcPixelGameEngine.h"
#include <memory>
#include <vector>

class Blocks {
public:
	using size_type = std::vector<int>::size_type;
	using iterator = std::vector<int>::iterator;
	using const_iterator = std::vector<int>::const_iterator;
	Blocks();
	Blocks(const std::vector<int> &map);
	size_t width() const;
	size_t height() const;
	size_t size() const;
	olc::vi2d blockSize();
	void loadMap(const std::vector<int> &map);
	void drawMap(olc::PixelGameEngine *game);
	int& operator[](const size_type index);
	iterator begin();
	iterator end();
	const_iterator cbegin();
	const_iterator cend();
private:
	static constexpr size_t width_{24};
	static constexpr size_t height_{30};
	olc::vi2d blockSize_{ 16, 16 };
	std::vector<int> tileMap{};
	std::unique_ptr<olc::Sprite> sprTile{nullptr};
	std::unique_ptr<olc::Sprite> sprBreakableTile{nullptr};
};

#endif /* ifndef BLOCKS_H */