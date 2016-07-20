#include "sp/sp.h"
#include "Physics2DComponent.h"

#include "SpriteComponent.h"

#include "sp/entity/Entity.h"
#include <Box2D.h>

namespace sp { namespace entity { namespace component {

	using namespace graphics;
	using namespace maths;

	Physics2DComponent::Physics2DComponent(Entity* entity)
		: Component(entity), m_Body(nullptr), m_BodyDef(spnew b2BodyDef())
	{
		SpriteComponent* sprite = entity->GetComponent<SpriteComponent>();
		SP_ASSERT(sprite);
		m_EntityBounds = &sprite->sprite->GetBounds();

		const vec2& position = m_EntityBounds->position;

		m_BodyDef->type = b2_dynamicBody;
		m_BodyDef->position.Set(position.x, position.y);
	}

	Physics2DComponent::~Physics2DComponent()
	{
		spdel m_BodyDef;
	}

	void Physics2DComponent::Init(b2World* world)
	{
		m_Body = world->CreateBody(m_BodyDef);

		b2PolygonShape dynamicBox;
		const vec2& size = m_EntityBounds->size;
		dynamicBox.SetAsBox(size.x, size.y);				b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.8f;
		fixtureDef.restitution = 0.4f;
		m_Body->CreateFixture(&fixtureDef);	}

	const maths::vec2& Physics2DComponent::GetPosition() const
	{
		SP_ASSERT(m_Body);
		return *(const maths::vec2*)&m_Body->GetPosition();
	}

	void Physics2DComponent::SetPosition(const maths::vec2& position)
	{
		m_BodyDef->position.Set(position.x, position.y);
		m_EntityBounds->position = position;
	}

	void Physics2DComponent::ApplyForce(const maths::vec2& force)
	{
		SP_ASSERT(m_Body);
		m_Body->ApplyForce(*(b2Vec2*)&force, m_Body->GetWorldCenter(), true);
	}

	void Physics2DComponent::DebugRender(Renderer2D& renderer)
	{
		renderer.DrawRect(*m_EntityBounds);
		return;

		b2AABB aabb;
		aabb.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);
		aabb.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);
		b2Transform t;
		t.SetIdentity();
		b2Fixture* fixture = m_Body->GetFixtureList();
		while (fixture)
		{
			const b2Shape* shape = fixture->GetShape();
			for (int c = 0; c < shape->GetChildCount(); c++)
			{
				b2Vec2 r(shape->m_radius, shape->m_radius);
				b2AABB shapeAABB;
				shape->ComputeAABB(&shapeAABB, t, c);
				shapeAABB.lowerBound = shapeAABB.lowerBound + r;
				shapeAABB.upperBound = shapeAABB.upperBound - r;
				aabb.Combine(shapeAABB);
				Rectangle rec(*(vec2*)&m_Body->GetPosition(), vec2(aabb.upperBound.x - aabb.lowerBound.x, aabb.upperBound.y - aabb.lowerBound.y));
				renderer.DrawRect(rec);
			}
			fixture = fixture->GetNext();
		}
	}

} } }