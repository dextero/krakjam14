#include "Player.h"

#include "utils/Logger.h"

const float Player::MOVE_FORCE = 300.f;
const float Player::JUMP_FORCE = 500.f;

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

void PlayerContactListener::BeginContact(b2Contact *contact)
{
    if (contact->GetFixtureA() == mPlayer->mFootFixture
        || contact->GetFixtureB() == mPlayer->mFootFixture)
    {
        mPlayer->mCanJump = true;
        Log << "can jump\n";
    }
}

void PlayerContactListener::EndContact(b2Contact *contact)
{
    if (contact->GetFixtureA() == mPlayer->mFootFixture
        || contact->GetFixtureB() == mPlayer->mFootFixture)
    {
        mPlayer->mCanJump = false;
        Log << "cannot jump\n";
    }
}

Player::Player():
    mMoveDirection(MoveStop),
    mCanJump(true),
    mDoJump(false),
    mBody(nullptr),
    mFootFixture(nullptr)
{
}

Player::Player(b2World* world, float size, const b2Vec2& pos):
    Player()
{
    Create(world, size, pos);
}

Player::~Player()
{
    Clear();
}

void Player::Create(b2World* world, float size, const b2Vec2& pos)
{
    Log << "player size = " << size << "\n";

    Clear();

    mMoveDirection = MoveStop;
    mCanJump = true;
    mDoJump = false;
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
    lowerCircle.m_radius = size * 0.3f;

    mBody->CreateFixture(&upperCircle, 0.001f);
    mFootFixture = mBody->CreateFixture(&lowerCircle, 0.001f);

    mContactListener = new PlayerContactListener(this);
    world->SetContactListener(mContactListener);

    Log << "player created\n";
}

void Player::Clear()
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

void Player::Update(float dt)
{
    assert(mBody);

    (void)dt;

    static const float MAX_VELOCITY = MOVE_FORCE / 5.f;

    b2Vec2 force(0.f, mDoJump ? JUMP_FORCE : 0.f);

    //Log << "applying force: " << force.x << ", " << force.y << "\n";
    mBody->ApplyLinearImpulse(force, mBody->GetWorldCenter());

    b2Vec2 v = mBody->GetLinearVelocity();
    v.x = MAX_VELOCITY * (float)mMoveDirection;
    mBody->SetLinearVelocity(v);

    //Log << "player pos: " << mBody->GetPosition().x << ", " << mBody->GetPosition().y << "\n";
}

void Player::Render()
{
}

