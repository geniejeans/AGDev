#include "StatesEnemy.h"
//#include "SceneData.h"
//#include "PostOffice.h"
//#include "ConcreteMessages.h"

StatePatrol::StatePatrol(const std::string & stateID, CEnemy * go)
	: State(stateID),
	m_go(go)
{
}

StatePatrol::~StatePatrol()
{
}

void StatePatrol::Enter()
{

}

void StatePatrol::Update(double dt)
{
	if ((m_go->GetTarget() - m_go->GetPos()).LengthSquared() < 25.0f)
	{
		CWaypoint* nextWaypoint = m_go->GetNextWaypoint();
		if (nextWaypoint)
			m_go->SetTarget(nextWaypoint->GetPosition());
		else
			m_go->SetTarget(Vector3(0, 0, 0));
	}
}

void StatePatrol::Exit()
{
}


//==========================================

StateChase::StateChase(const std::string & stateID, CEnemy * go)
	: State(stateID),
	m_go(go)
{
}

StateChase::~StateChase()
{
}

void StateChase::Enter()
{
}

void StateChase::Update(double dt)
{
}

void StateChase::Exit()
{
}

//==========================================

StateSearch::StateSearch(const std::string & stateID, CEnemy * go)
	: State(stateID),
	m_go(go)
{
}

StateSearch::~StateSearch()
{
}

void StateSearch::Enter()
{
	

}

void StateSearch::Update(double dt)
{

	
}

void StateSearch::Exit()
{
}

//==========================================

StateAttack::StateAttack(const std::string & stateID, CEnemy * go)
	: State(stateID),
	m_go(go)
{
}

StateAttack::~StateAttack()
{
}

void StateAttack::Enter()
{
}

void StateAttack::Update(double dt)
{
}

void StateAttack::Exit()
{
}