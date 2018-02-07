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
	if (m_go->GetDistanceFromPlayer() < 100.0f)
		m_go->sm->SetNextState("CHASE");
	else if ((m_go->GetPreviousWaypoint() - m_go->GetPos()).LengthSquared() < 25.0f)
	{
		CWaypoint* nextWaypoint = m_go->GetNextWaypoint();
		if (nextWaypoint)
			m_go->SetTarget(nextWaypoint->GetPosition());
		else
			m_go->SetTarget(Vector3(0, 0, 0));
		m_go->SetPreviousWaypoint(m_go->GetTarget());
	}
	else
		m_go->SetTarget(m_go->GetPreviousWaypoint());
//	std::cout << "PATROLLING" << std::endl;
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
//	std::cout << "CHASING" << std::endl;
	if (m_go->GetDistanceFromPlayer() < 50.0f)
		m_go->sm->SetNextState("ATTACK");
	else if (m_go->GetDistanceFromPlayer() < 100.0f)
		m_go->SetTarget(m_go->GetPositionOfPlayer());
	else
		m_go->sm->SetNextState("PATROL");
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
//	std::cout << "ATTACKING" << std::endl;
	if (m_go->GetDistanceFromPlayer() > 50.0f)
		m_go->sm->SetNextState("CHASE");
}

void StateAttack::Exit()
{
}