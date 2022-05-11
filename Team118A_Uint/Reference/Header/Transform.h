#pragma once
#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final: public CComponent
{
private:
	explicit CTransform(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();


public:			/*		메서드		*/
	virtual CComponent*		Clone() override;
	virtual void			Free() override;
	virtual	void			Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void			Load(HANDLE hFile, DWORD& dwByte) override;
	static CTransform*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	HRESULT					Ready_Transform();
	virtual _int			Update_Component(const _float& fTimeDelta);
	

	const _vec3				Get_Info(E_INFO eType) const;
	const _vec3&			Get_Scale() { return m_vScale; }
	const _vec3&			Get_Angle() { return m_vAngle; }
	const _matrix&			Get_WorldMatrix() { return m_matWorld; }
	const _matrix			Get_WorldMatrix_Billboard();

	void					Set_Pos(_vec3 vPos);
	void					Set_Pos(E_POSITION ePosition, _float fPos);

	const _vec3&			Get_Pos() { return m_vInfo[(_int)E_INFO::INFO_POS]; }

	void					Add_Pos(_vec3 vAddPos);
	void					Add_Pos(E_POSITION ePosition, _float fAddPos);

	void					Move_Pos(const _vec3& pDir, const _float& fSpeed, const _float& fTimeDelta);

	void					Set_Rotation(_vec3 vRots);
	void					Set_Rotation(_float fDegX, _float fDegY, _float fDegZ); 
	void					Set_Rotation(E_ROTATION eRotation, _float fDeg);

	void					Add_Rotation(_vec3 vAddAngle);
	void					Add_Rotation(E_ROTATION eRotation, _float fDeg);

	void					Set_Scale(_vec3 vScale);
	void					Set_Scale(_float fScaleX, _float fScaleY, _float fScaleZ);
	void					Set_Scale(E_SCALE eScale, _float fScale);

	void					Add_Scale(_vec3 vAddScale);
	void					Add_Scale(E_SCALE eScale, _float fAddScale);
	
	void					Set_Look(_vec3 vTargetPos);
	void					Set_Direction(_vec3 vLook);

	void					Set_ParentMatrix(const _matrix* pParent);

protected:		/*		메서드		*/

private:		/*		메서드		*/

protected:		/*		 변수		*/
	_vec3			m_vInfo[(_int)E_INFO::INFO_END];
	_vec3			m_vScale;
	_vec3			m_vAngle;
	_matrix			m_matWorld;

private:		/*		 변수		*/

public:			/*		 변수		*/
};

END

#endif // !__TRANSFORM_H__