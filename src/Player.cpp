#include "Player.h"

#include "utils/Logger.h"
#include "utils/Utils.h"

class PlayerContactListener: public b2ContactListener
{
public:
    PlayerContactListener(Player* player);

    virtual void BeginContact(b2Contact *contact);
    virtual void EndContact(b2Contact *contact);

private:
    Player* mPlayer;
};

PlayerContactListener::PlayerContactListener(Player* player):
    mPlayer(player)
{
}

#define CONTACT_HAS_FIXTURE(contact, fixture) \
    ((contact)->GetFixtureA() == (fixture) \
     || (contact)->GetFixtureB() == (fixture))

void PlayerContactListener::BeginContact(b2Contact *contact)
{
    if (CONTACT_HAS_FIXTURE(contact, mPlayer->mFootFixture))
    {
        mPlayer->mCanJump = true;
        Log << "can jump\n";
    }
    else if (CONTACT_HAS_FIXTURE(contact, mPlayer->mLeftEar))
    {
        mPlayer->mCanWallJump = true;
        mPlayer->mJumpDir = MoveLeft;
        Log << "can wall jump (left)\n";
    }
    else if (CONTACT_HAS_FIXTURE(contact, mPlayer->mRightEar))
    {
        mPlayer->mCanWallJump = true;
        mPlayer->mJumpDir = MoveRight;
        Log << "can wall jump (right)\n";
    }
}

void PlayerContactListener::EndContact(b2Contact *contact)
{
    if (CONTACT_HAS_FIXTURE(contact, mPlayer->mFootFixture))
    {
        mPlayer->mCanJump = false;
        Log << "cannot jump\n";
    }
    else if (CONTACT_HAS_FIXTURE(contact, mPlayer->mLeftEar)
        || CONTACT_HAS_FIXTURE(contact, mPlayer->mRightEar))
    {
        mPlayer->mCanWallJump = false;
        Log << "cannot walljump\n";
    }
}

Player::Player():
    mMoveDirection(MoveStop),
    mCanJump(true),
    mDoJump(false),
    mDoWallJump(false),
    mBody(nullptr),
    mFootFixture(nullptr)
{
}

Player::Player(b2World* world, float size, const b2Vec2& pos):
    Player()
{
    create(world, size, pos);
}

Player::~Player()
{
    clear();
}

void Player::create(b2World* world, float size, const b2Vec2& pos)
{
    Log << "player size = " << size << "\n";

    clear();

    mMoveDirection = MoveStop;
    mCanJump = true;
    mDoJump = false;
    mDoWallJump = false;
    mBody = nullptr;
    mFootFixture = nullptr;

    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(pos.x, pos.y);
    bodyDef.fixedRotation = true;

    mBody = world->CreateBody(&bodyDef);
    mBody->SetType(b2_dynamicBody);

    b2CircleShape upperCircle;
    upperCircle.m_p = b2Vec2(0.f, 0.6f * size);
    upperCircle.m_radius = size * 0.4f;

    b2CircleShape lowerCircle;
    lowerCircle.m_p = b2Vec2(0.f, 0.3f * size);
    lowerCircle.m_radius = size * 0.4f;

    b2PolygonShape leftBox;
    leftBox.SetAsBox(
        0.5f,
        0.2f * size,
        b2Vec2(-upperCircle.m_radius + 0.5, upperCircle.m_p.y),
        0.f);

    b2PolygonShape rightBox;
    rightBox.SetAsBox(
        0.5f,
        0.2f * size,
        b2Vec2(upperCircle.m_radius - 0.5, upperCircle.m_p.y),
        0.f);

    b2FixtureDef upperCircleDef;
    upperCircleDef.shape = &upperCircle;
    upperCircleDef.friction = 2.f;
    upperCircleDef.density = 0.0001f;

    b2FixtureDef lowerCircleDef;
    lowerCircleDef.shape = &lowerCircle;
    lowerCircleDef.friction = 2.f;
    lowerCircleDef.density = 0.0001f;

    b2FixtureDef leftBoxDef;
    leftBoxDef.shape = &leftBox;
    leftBoxDef.friction = 0.05f;
    leftBoxDef.density = 0.0001f;

    b2FixtureDef rightBoxDef;
    rightBoxDef.shape = &rightBox;
    rightBoxDef.friction = 0.05f;
    rightBoxDef.density = 0.0001f;

    mBody->CreateFixture(&upperCircleDef);
    mLeftEar = mBody->CreateFixture(&leftBoxDef);
    mRightEar = mBody->CreateFixture(&rightBoxDef);
    mFootFixture = mBody->CreateFixture(&lowerCircleDef);

    mContactListener = new PlayerContactListener(this);
    world->SetContactListener(mContactListener);

    Log << "player created\n";
}

void Player::clear()
{
    if (mBody)
    {
        Log << "player destroyed\n";

        mBody->GetWorld()->DestroyBody(mBody);
        mBody->GetWorld()->SetContactListener(nullptr);

        delete mContactListener;
        mContactListener = nullptr;
    }
}

void Player::update(float dt)
{
    assert(mBody);

    (void)dt;

    static const float MAX_VELOCITY = 100.f;
    static const float MAX_JUMP_VELOCITY = 100.f;
    static const float MAX_WALL_JUMP_VELOCITY = 0.4f * MAX_JUMP_VELOCITY;

    b2Vec2 v = mBody->GetLinearVelocity();

    if (mDoJump)
    {
        v.y = mDoJump
              ? MAX_JUMP_VELOCITY * Utils::clamp(fabsf(v.x) * 0.3f,  0.7f, 1.3f)
              : v.y;
    }
    else if (mDoWallJump)
    {
        switch (mJumpDir)
        {
        case MoveLeft:
            v.x = MAX_VELOCITY;
            v.y = MAX_WALL_JUMP_VELOCITY;
            break;
        case MoveRight:
            v.x = -MAX_VELOCITY;
            v.y = MAX_WALL_JUMP_VELOCITY;
            break;
        default:
            Log << "derp\n";
            break;
        }
    }

    if (v.x <= MAX_VELOCITY && v.x >= -MAX_VELOCITY)
    {
        v.x += Utils::clamp(MAX_VELOCITY - fabsf(v.x), 0.5f, 2.f)
               * MAX_VELOCITY * (float)mMoveDirection * dt;
    }

    mBody->SetLinearVelocity(v);

    //Log << "player pos: " << mBody->GetPosition().x << ", " << mBody->GetPosition().y << "\n";
}

void Player::render()
{
}

