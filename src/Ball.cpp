#include "Ball.h"
#include "Bat.h"
#include "BreakOut.h"

void
Ball::update(float fElapsedTime)
{
	potentialPos_ = position_ + direction_ * speed_ * fElapsedTime;
	hasHitTile = false;
	hasHitTile |= testResolveCollision(olc::vf2d(0,-1)); // northen
	hasHitTile |= testResolveCollision(olc::vf2d(0,+1)); // southern
	hasHitTile |= testResolveCollision(olc::vf2d(-1,0)); // western
	hasHitTile |= testResolveCollision(olc::vf2d(+1,0)); // eastern

//	// Check Bat position
	if (position_.y >= bat_.position().y
			&& position_.x > bat_.position().x
			&& position_.x < bat_.position().x + bat_.width()
			&& position_.y < bat_.position().y + radius_ / 16 ) {
		/* this is the same as taking the dot product */
		float batCenter = bat_.position().x + (bat_.width() / 2);
		float hitPos = (position_.x + tileRadialDimensions_.x - batCenter) / (bat_.width());
		direction_.y *= -1;
		direction_.x = hitPos;
		// normalize the vector allows us to the direction withou changing the speed
		direction_ = direction_.norm();
	}
	// ball position is out of bound  *** this is buggy ***
	isOutOfBounds = position_.y > blocks_.height();
	if (isOutOfBounds && game_.GetKey(olc::Key::SPACE).bPressed) {
		reset(blocks_.width() / 2.0f, blocks_.height() / 2.0f);
		isOutOfBounds = false;
	}
	position_ += direction_ * speed_ * fElapsedTime; // delta x = detal v * delta t
}

void
Ball::draw()
{
	if (isOutOfBounds) {
		auto msg = std::string("PRESS SPACE BAR");
		game_.DrawString(olc::vi2d((blocks_.width() - msg.size() / 2) / 2, blocks_.height() / 2) * blocks_.blockSize(), msg, olc::WHITE);
	} else
		game_.DrawSprite(position_ * blocks_.blockSize(), sprBall.get());
}

void 
Ball::reset(const olc::vf2d &postion)
{
	reset(postion.x, postion.y);
}

void
Ball::reset(float x, float y)
{
	// change 1.0f to 2.0f to get an angle value between 0 and 2PI (360 degrees) as well.
	angle_ = randomf(0.1f, 0.9f) * 1.0f * M_PI;
	direction_ = { cosf(angle_), sinf(angle_) };
	speed_ = 16.0f;
	position_ = { x, y };
}

bool
Ball::testResolveCollision(const olc::vf2d &point)
{
	// offset to center point of reference
	olc::vf2d offset = { radius_ / blocks_.blockSize().x, radius_ / blocks_.blockSize().y };
	olc::vi2d vTestPoint = offset + potentialPos_ + tileRadialDimensions_ * point;

	auto index = vTestPoint.y * blocks_.width() + vTestPoint.x;
	auto &tile = blocks_[index];
	if (tile == 0 || index > blocks_.size())
		return false;
	bool bTileHit = tile < 10;
	if (bTileHit) {
		hitID = tile;
		hitpos = { float(vTestPoint.x), float(vTestPoint.y) }; 
		--tile;
	}
	// collision response
	if (point.x == 0.0f) direction_.y *= -1.0f;
	if (point.y == 0.0f) direction_.x *= -1.0f;
	return true;
}

bool Ball::intersectBat(Bat *bat)
{
		auto potentialPos = position_ + direction_ * speed_ * game_.GetElapsedTime();
		float testX = potentialPos.x;
		float testY = potentialPos.y;

		if (potentialPos.x < bat->position().x)
			testX = bat->position().x; // test against left edge
		else if (potentialPos.x > bat->position().x + bat->width())
			testX = bat->position().x + bat->width(); // test against right edge;

		if (potentialPos.y < bat->position().y) // top edge
			testY = bat->position().y;
		else if (potentialPos.y > bat->position().y + bat->height()) // bottom edge
			testY = bat->position().y + bat->height();

		float distX = potentialPos.x - testX;
		float distY = potentialPos.y - testY;
		float distance = sqrt((distX * distX) + (distY * distY)); // sqrt is expensive
		return distance <= radius_ / 16; // don't like this
}
