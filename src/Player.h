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

    void create(b2World* world, float size, const b2Vec2& pos);
    void clear();

    void update(float dt);
    void render();

    inline void move(MoveDirection direction);
    inline void jump(bool jump);

private:
    MoveDirection mMoveDirection;
    bool mCanJump;
    bool mCanWallJump;
    bool mDoJump;
    bool mDoWallJump;
    MoveDirection mJumpDir;

    b2Body* mBody;
    b2Fixture* mLeftEar;
    b2Fixture* mRightEar;
    b2Fixture* mFootFixture;
    static const float MOVE_FORCE;
    static const float JUMP_FORCE;

    PlayerContactListener* mContactListener;

    friend class PlayerContactListener;
};

inline void Player::move(MoveDirection direction)
{
    mMoveDirection = direction;
}

inline void Player::jump(bool jump)
{
    mDoJump = jump && mCanJump;
    //Log << "mDoJump = " << mDoJump << "\n";

    if (!mDoJump)
    {
        mDoWallJump = jump && mCanWallJump;
        //Log << "mDoWallJump = " << mDoWallJump << "\n";
    }
}
