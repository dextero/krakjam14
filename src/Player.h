#pragma once

#include <Box2D/Box2D.h>

#include "utils/Logger.h"

enum MoveDirection
{
    MoveLeft = -1,
    MoveStop = 0,
    MoveRight = 1
};

class PlayerContactListener;

class Player
{
public:
    Player();
    Player(b2World* world, float size, const b2Vec2& pos);

    ~Player();

    void Create(b2World* world, float size, const b2Vec2& pos);
    void Clear();

    void Update(float dt);
    void Render();

    inline void Move(MoveDirection direction);
    inline void Jump(bool jump);

private:
    MoveDirection mMoveDirection;
    bool mCanJump;
    bool mDoJump;

    b2Body* mBody;
    b2Fixture* mFootFixture;
    static const float MOVE_FORCE;
    static const float JUMP_FORCE;

    PlayerContactListener* mContactListener;

    friend class PlayerContactListener;
};

inline void Player::Move(MoveDirection direction)
{
    mMoveDirection = direction;
}

inline void Player::Jump(bool jump)
{
    //Log << "jump: " << jump << "\n";
    mDoJump = jump && mCanJump;
}
