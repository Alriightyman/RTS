#ifndef _RTS_UNIT_
#define _RTS_UNIT_

#include "skinnedmesh.h"
#include "mapObject.h"

void LoadUnitResources(IDirect3DDevice9* Device);
void UnloadUnitResources();

class UNIT : public MAPOBJECT
{
	friend class APPLICATION;
	public:
		UNIT(int _type, int _team, INTPOINT mp, TERRAIN *_terrain, IDirect3DDevice9* Dev);
		~UNIT();

		//Abstract functions declared in MAPOBJECT
		void Render();
		void Update(float deltaTime);
		BBOX GetBoundingBox();
		D3DXMATRIX GetWorldMatrix();

		//Specific UNIT functions
		void Goto(INTPOINT mp);	//Order unit to mp
		void MoveUnit(INTPOINT to);
		bool CheckCollision(INTPOINT mp);
		D3DXVECTOR3 GetDirection(INTPOINT p1, INTPOINT p2);
		void SetAnimation(char name[]);
		void SetAnimation(int index);

	private:

		//Animation variables
		float m_time;					//This units animation time
		float m_speed;					//Movement & animation speed
		int m_animation;				//Current animation, Run, Still, attack etc.
		D3DXVECTOR3 m_rotation, m_scale;//Used to build the world matrix
		ID3DXAnimationController* m_pAnimControl;	//Animation control

		//Movement variables
		std::vector<INTPOINT> m_path;	//The active path 
		D3DXVECTOR3 m_lastWP, m_nextWP;	//last & next waypoint
		int m_activeWP;				//active waypoint
		bool m_moving;
		float m_movePrc;				// 0.0 - 1.0, used to interpolate between m_lastWP & m_nextWP
};

#endif