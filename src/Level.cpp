#include "Level.h"

#include "utils/Utils.h"
#include "utils/Logger.h"

#include <SFML/Graphics/Shape.hpp>

Level::Level()
{
    Load("");
}

Level::~Level()
{
}

bool Level::Load(const std::string& filename)
{
    (void)filename;

    static const b2Vec2 WINDOW_SIZE(1024.f, 768.f);
    static const float PLAYER_SIZE = 20.f;
    static const float MARGIN = 10.f;
    static const b2Vec2 INITIAL_PLAYER_POS(5.f * MARGIN, 2.f * MARGIN);
    static const b2Vec2 GRAVITY(0.f, -100.f);

    mWorld.reset(new b2World(GRAVITY));

    b2AABB platforms[] = {
        { b2Vec2(MARGIN, MARGIN), b2Vec2(WINDOW_SIZE.x - MARGIN, 2.f * MARGIN) },
        { b2Vec2(MARGIN, MARGIN), b2Vec2(2.f * MARGIN, WINDOW_SIZE.y - MARGIN) },
        { b2Vec2(WINDOW_SIZE.x - 2.f * MARGIN, MARGIN), b2Vec2(WINDOW_SIZE.x - MARGIN, WINDOW_SIZE.y - MARGIN) },
        { b2Vec2(MARGIN, WINDOW_SIZE.y - 2.f * MARGIN), b2Vec2(WINDOW_SIZE.x - MARGIN, WINDOW_SIZE.y - MARGIN) }
    };

    for (size_t i = 0; i < ARRAY_SIZE(platforms); ++i)
    {
        AddPlatform(platforms[i]);
        Log << "adding " << platforms[i].lowerBound.x << ", "
                         << platforms[i].lowerBound.y << " -> "
                         << platforms[i].upperBound.x << ", "
                         << platforms[i].upperBound.y << "\n";
    }

    mPlayer.Create(mWorld.get(), PLAYER_SIZE, INITIAL_PLAYER_POS);

    return true;
}


void Level::Update(float dt)
{
    mPlayer.Update(dt);
    mWorld->Step(dt, 5, 5);
}

void Level::Draw(sf::RenderWindow& window)
{
    DebugDrawBodies(window);
}

void Level::DebugDrawBodies(sf::RenderWindow& window)
{
    static const sf::Color BODY_COLOR(255, 0, 0, 128);

    for (b2Body* b = mWorld->GetBodyList(); b; b = b->GetNext())
    {
        b2Vec2 pos = b->GetPosition();

        for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
        {
            b2Shape* s = f->GetShape();

            switch (s->GetType())
            {
            case b2Shape::e_circle:
                {
                    b2CircleShape* c = dynamic_cast<b2CircleShape*>(s);

                    sf::Shape circle = sf::Shape::Circle(
                        pos.x + c->m_p.x,
                        pos.y + c->m_p.y,
                        c->m_radius,
                        BODY_COLOR);

                    window.Draw(circle);
                }
                break;
            case b2Shape::e_polygon:
                {
                    b2PolygonShape* p = dynamic_cast<b2PolygonShape*>(s);
                    sf::Shape poly = sf::Shape();

                    for (int32 i = 0; i < p->GetVertexCount(); ++i)
                    {
                        b2Vec2 v = p->GetVertex(i);
                        poly.AddPoint(
                            pos.x + v.x,
                            pos.y + v.y,
                            BODY_COLOR);
                    }

                    window.Draw(poly);
                }
                break;
            default:
                assert(false);
                break;
            }
        }
    }
}

void Level::AddPlatform(const b2AABB& platform)
{
    b2BodyDef platformBodyDef;
    platformBodyDef.position = platform.GetCenter(); 
    b2Body* platformBody = mWorld->CreateBody(&platformBodyDef);
    platformBody->SetType(b2_staticBody);

    b2PolygonShape platformBox;
    b2Vec2 platformHalfSize = platform.GetExtents();
    platformBox.SetAsBox(platformHalfSize.x, platformHalfSize.y);

    platformBody->CreateFixture(&platformBox, 0.f);
}
