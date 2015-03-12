#ifndef DEF_3DPANEL_H
#define DEF_3DPANEL_H

//#include "axLib.h"
#include "irrlicht.h"
#include "wx/wx.h" 
#include "Motion\MotionFile.h"
#include <list>
#include <vector>

typedef irr::SIrrlichtCreationParameters	irr_CreateParam;
typedef irr::video::E_DRIVER_TYPE			irr_DriverType;
typedef irr::IrrlichtDevice					irr_Device;
typedef irr::scene::ISceneManager			irr_SceneManager;
typedef irr::video::IVideoDriver			irr_VideoDriver;
typedef irr::scene::ICameraSceneNode		irr_Camera;
typedef irr::scene::ISceneNode				irr_Node;
typedef irr::video::SColor					irr_Color;
typedef irr::core::dimension2d<irr::s32>	irr_Dimension2D;
typedef irr::core::vector3df				irr_Vector3D;
typedef irr::video::E_MATERIAL_FLAG			irr_MaterialFlag;
typedef irr::s32							irr_S32;
typedef irr::scene::IAnimatedMesh			irr_Mesh;
typedef irr::scene::IAnimatedMeshSceneNode	irr_SceneMesh;
typedef irr::scene::IBoneSceneNode			irr_Bone;
typedef irr::core::quaternion				irr_quaternion;

enum class BoneSide
{
	LEFT,  _FIRST = LEFT,
	RIGHT, _LAST = RIGHT
};
enum class BoneNumber
{
	SHOULDER, _FIRST = SHOULDER,
	UPPER_ARM,
	FOREARM,
	FOREARM_TWIST1,
	FOREARM_TWIST2,
	HAND, _LAST = HAND
};

/// @todo Needs Documentation.
class Device3D : public wxWindow
{
public:
	Device3D(wxWindow* win, const wxWindowID& id,
			 wxPoint point, wxSize size,
			 irr_DriverType type = irr::video::EDT_DIRECT3D9,  //FIXME: We should have the choice between D3D/OpenGL (EDT_DIRECT3D9/EDT_OPENGL); my current configuration has problems with OpenGL>1.4.
			 bool bResizeable = true);

	~Device3D();

	/// @todo Needs Documentation.
	irr_Camera* addCamera(irr_Node* parent = 0,
						  const irr_Vector3D& position = irr_Vector3D(0.0f, 0.0f, 0.0f),
						  const irr_Vector3D& lookat = irr_Vector3D(0.0f, 0.0f, 1.0f),
						  irr_S32 id = 1)
	{
		(id);
		return irrSceneManager_->addCameraSceneNode(parent, position, lookat);
	}

	void mSize(const wxSize& newSize);
	void timerEvent(double ms);

	void SetLeftAlign();
	void SetFrontAlign();
	void SetRightAlign();

protected:
	irr_Device* irrDevice_;
	irr::video::IVideoDriver* irrVideoDriver_;
	irr_SceneManager* irrSceneManager_;
	irr_Camera* camera_;
	double cameraDistance_, cameraHorizontalAngle_, cameraVerticalAngle_;
	irr_SceneMesh* m_node;
	irr_Bone* bones_[int(BoneSide::_LAST) + 1][int(BoneNumber::_LAST) + 1];
	irr_Bone* fingerBones_[int(BoneSide::_LAST) + 1][5][4];
	irr_Bone* eyeRigs_[int(BoneSide::_LAST) + 1];
	irr_Bone* eyeModels_[int(BoneSide::_LAST) + 1];
	irr_Bone* head_;
	VCNQuat shoulderRestOrientations_[int(BoneSide::_LAST) + 1];
	VCNQuat ribsOrientation_;
	irr_Bone*& Device3D::getBone(BoneSide side, BoneNumber bone)
	{
		return bones_[int(side)][int(bone)];
	}
	irr_Bone*& Device3D::getFingerBone(BoneSide side, int finger, int phalanx)
	{
		return fingerBones_[int(side)][finger][phalanx];
	}
	double timeOffsetMs_;
	std::vector<MotionFilePacket> m_motion;
	void updateCamera();

	double theta;
	wxPoint clickPt;

	static const int BONE_LEFT = 10, BONE_RIGHT = 35;

	void OnPaint(wxPaintEvent &event);
	void OnMouseLeftDown(wxMouseEvent& event);
	void OnMouseMotion(wxMouseEvent& event);
	void OnMouseLeftUp(wxMouseEvent& event);
	void OnChar(wxKeyEvent& event);
	void OnSize(wxSizeEvent& event);

	DECLARE_EVENT_TABLE()
};

#endif