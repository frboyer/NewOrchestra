/// Pannel where the 3D model is shown.
/// \file Panel3D.cpp
/// \author Alexandre Arsenault and Francois-R.Boyer@PolyMtl.ca
/// \date 2014-03
/// Creation date 2013-01



#include "Panel3D.h"
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Range.h"
#include "Debug.h"


ITERABLE_ENUM(BoneSide)
ITERABLE_ENUM(BoneNumber)

BEGIN_EVENT_TABLE(Device3D, wxWindow)
	EVT_SIZE(Device3D::OnSize)
	EVT_PAINT(Device3D::OnPaint)
	EVT_LEFT_UP(Device3D::OnMouseLeftUp)
	EVT_MOTION(Device3D::OnMouseMotion)
	EVT_LEFT_DOWN(Device3D::OnMouseLeftDown)
	EVT_CHAR(Device3D::OnChar)
END_EVENT_TABLE()

static const bool useMs3d = false;

string getBoneName(const string& s)
{
	if (useMs3d)
		return s;

	string result = "pc_rig_" + s;
	for (auto& c : result) {
		if (c == '.')
			c = '_';
	}
	return result;
}

string getBoneName(BoneSide boneSide, BoneNumber boneNumber)
{
	static const string boneNames[] = { "shoulder", "upper_arm", "forearm", "forearm.twist1", "forearm.twist2", "hand" };
	static const string sides[] = { "L", "R" };
	string result = boneNames[int(boneNumber)] + '.' + sides[int(boneSide)];
	return getBoneName(result);
}

static const irr::f32 DEG_TO_RAD = irr::f32(M_PI / 180.0);

Device3D::Device3D(wxWindow* win, const wxWindowID& id,
				   wxPoint point, wxSize size,
				   irr_DriverType type, bool bResizeable) :
				   // Heritage
				   wxWindow(win, id, point, size, wxWANTS_CHARS)
{
	// Set background style so wxWidgets does not fill the region, we will handle it using Irrlicht.
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	// Create the Irrlicht device inside wxWindow.
	irr_CreateParam param;
	param.WindowId = this->GetHandle();
	param.DriverType = type;
	param.Doublebuffer = true;
	param.WindowSize = irr_Dimension2D(size.x, size.y);
	irrDevice_ = irr::createDeviceEx(param);
	irrVideoDriver_  = irrDevice_->getVideoDriver();
	irrSceneManager_ = irrDevice_->getSceneManager();
	irrDevice_->setResizable(bResizeable);

	// Set the camera.
	irr_Vector3D cameraPosition(0.0, 20.0, -24.0);
	irr_Vector3D cameraLookAt  (0.0, 18.0,   0.0);
	camera_ = addCamera(0, cameraPosition, cameraLookAt);
	camera_->setFOV(27.0f * DEG_TO_RAD); // Vertical field of view of a 50mm lens on 35mm film.

	// Set lighting.
	irrSceneManager_->addLightSceneNode(0, irr_Vector3D(20, 20, -50), irr_Color(255, 200, 200, 200));
	irrSceneManager_->addLightSceneNode(0, irr_Vector3D(-20,  50, -0), irr_Color(255, 127, 127, 127));
	irrSceneManager_->addLightSceneNode(0, irr_Vector3D(-20, -40, -10), irr_Color(255, 122, 116, 110)); // 255, 241, 229
	
	// Load model.
	irr_Mesh* mesh = irrSceneManager_->getMesh(useMs3d ? "Ressources/male_test.ms3d" : "Ressources/male_test.x");
	//irr::scene::IMeshManipulator *manipulator = irrSceneManager_->getMeshManipulator();
	//manipulator->recalculateNormals(mesh, true, true);
	//irr_Mesh* tangentMesh = dynamic_cast<irr_Mesh*>(manipulator->createMeshWithTangents(mesh->getMesh(0)));
	m_node = irrSceneManager_->addAnimatedMeshSceneNode(mesh);

	if (!m_node) return; //FIXME: Not sure how we should handle failed model loading.

	// We need to control the joints.
	m_node->setJointMode(irr::scene::EJUOR_CONTROL);
	//m_node->animateJoints();

	// Set material properties.
	//m_node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	//m_node->setMaterialFlag(irr::video::EMF_TRILINEAR_FILTER, true);
	//m_node->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, true);
	// Add normal mapping (does not give the expected result...  Irrlicht requires tangents to compute normal mapping)
	//m_node->setMaterialTexture(0, irrVideoDriver_->getTexture("Ressources/male_c.png"));
	//m_node->setMaterialTexture(1, irrVideoDriver_->getTexture("Ressources/normal_uniform.png"));
	//m_node->setMaterialType(irr::video::EMT_NORMAL_MAP_SOLID);

	// Align model to our coordinate system: +x right of screen, +y up, +z into screen.
	if (useMs3d) {
		m_node->setRotation(irr_Vector3D(0, -90, 0)); //Ms3d blender exporter / irrlicht importer has +x pointing into screen instead right ?? 
		m_node->setScale(irr_Vector3D(1.3f, 1.3f, 1.3f));
	}
	else
		m_node->setScale(irr_Vector3D(1.3f, -1.3f, 1.3f)); //X blender exporter / irrlicht importer has +y pointing down instead of up ??  (the .x is correct in AssimpView, and if exporting the default cube it is not reversed)

	// Get pointers to bones we need to control.
	for (auto side : enumerate<BoneSide>())
		for (auto bone : enumerate<BoneNumber>())
			getBone(side, bone) = m_node->getJointNode(getBoneName(side, bone).c_str());

	//FIXME: Why are hips incorrectly aligned on import?
	irr_Bone* hips = m_node->getJointNode(getBoneName("hips").c_str());
	if (hips)
	{
		const irr_Vector3D& r = hips->getRotation();
		DEBUG_STREAM << "Hips rotation: " << r.X << "," << r.Y << "," << r.Z << endl;
		hips->setRotation(irr_Vector3D(0, 0, 0));
	}

	// Display all bones.
	DEBUG_STREAM << "N BONES : " << m_node->getJointCount() << endl;
	for (unsigned i = 0; i < m_node->getJointCount(); ++i)
	{
		irr_Bone* bone = m_node->getJointNode(i);
		const irr_Vector3D& r = bone->getRotation();
		DEBUG_STREAM << "BONE NAME : " << i << " " << bone->getName() << " : " << r.X << "," << r.Y << "," << r.Z << endl;
	}

	// Display hierarchy from one hand.
	DEBUG_STREAM << "Hierarchy : {" << endl;
	for (irr_Bone* bone = getBone(BoneSide::RIGHT, BoneNumber::HAND); bone != nullptr; bone = dynamic_cast<irr_Bone*>(bone->getParent()))
		DEBUG_STREAM << "  " << bone->getName() << endl;
	DEBUG_STREAM << "}" << endl;

	//Compute(hand_, (irr_Bone*) upper_arm_L->getParent(), hand_->getPosition());

	//getBone(BoneSide::LEFT, BoneNumber::SHOULDER)->setRotation(irr_Vector3D(0, 90, 90));
	//getBone(BoneSide::RIGHT, BoneNumber::SHOULDER)->setRotation(irr_Vector3D(0, -90, -90));
	//for (auto side : enumerate<BoneSide>()) {
	//	getBone(side, BoneNumber::UPPER_ARM)->setRotation(irr_Vector3D(0, 0, 0));
	//	getBone(side, BoneNumber::FOREARM_TWIST1)->setRotation(irr_Vector3D(0, 0, 0));
	//	getBone(side, BoneNumber::FOREARM_TWIST2)->setRotation(irr_Vector3D(0, 0, 0));
	//	getBone(side, BoneNumber::FOREARM)->setRotation(irr_Vector3D(0, 0, 0));
	//	getBone(side, BoneNumber::HAND)->setRotation(irr_Vector3D(0, 0, 0));
	//}
}

Device3D::~Device3D()
{
	irrDevice_->closeDevice();
	irrDevice_->drop();
	irrDevice_ = nullptr; irrVideoDriver_ = nullptr; irrSceneManager_ = nullptr;
}

void Device3D::OnSize(wxSizeEvent& event)
{
	wxSize size(event.GetSize());
	SetSize(size);
	camera_->setAspectRatio(irr::f32(size.x) / size.y);  // This will keep vertical FOV fixed but change horizontal FOV to keep correct pixel apect ratio.

	//scaleImage(_markerData[_selectedMarker].numImg);
	//_currentMarkerImg = createSelectedMarkerImage(_selectedMarker,
	//	wxColor(180, 180, 180));
	//Refresh();

	irrVideoDriver_->OnResize(irr::core::dimension2d<irr::u32>(size.x, size.y));
	//SetSize(newSize);
	Refresh();
}

void Device3D::mSize(const wxSize& newSize)
{
	irrVideoDriver_->OnResize(irr::core::dimension2d<irr::u32>(newSize.x, newSize.y));
	SetSize(newSize);
	Refresh();
}

void Device3D::OnMouseLeftUp(wxMouseEvent& event)
{
	/*UNREFERENCED_PARAMETER*/ (event);
	if (HasCapture())
	{
		ReleaseMouse();
	}
}

template <typename T>
void clamp(T& v, T min, T max) {
	if (v > max) v = max;
	else if (v < min) v = min;
}

void Device3D::OnMouseMotion(wxMouseEvent& event)
{
	static int lastX = 0, lastY = 0;
	int x = event.GetX() - clickPt.x;
	int y = event.GetY() - clickPt.y;

	if (event.Dragging() && HasCapture() && event.LeftIsDown())
	{
		irr_Bone* bone = getBone(BoneSide::RIGHT, BoneNumber::UPPER_ARM);
		irr_Vector3D rot = bone->getRotation() + irr_Vector3D(x - lastX, y - lastY, 0);
		lastX = x; lastY = y;
		clamp(rot.X, 260.0f, 365.0f);
		clamp(rot.Y, -45.0f, 80.0f);
		bone->setRotation(rot);
		//getBone(side, FOREARM)->setRotation(rot);

		Refresh();
	}
	else lastX = lastY = 0;
}

void Device3D::OnMouseLeftDown(wxMouseEvent& event)
{
	clickPt = event.GetPosition();
	//_DEBUG_ DSTREAM << "LEFT DOWN" << endl;
	CaptureMouse();
	SetFocus();
	SetFocusFromKbd();
}

void Device3D::OnChar(wxKeyEvent& event)
{
	auto* node = m_node;
	irr_Vector3D v = node->getPosition();
	auto* bone = getBone(BoneSide::RIGHT, BoneNumber::UPPER_ARM); //m_node->getJointNode("pc_rig_finger_index_01_R"); // pc_rig_upper_arm_R");
	irr_Vector3D rot = bone->getRotation();
	switch (event.GetKeyCode()) {
	case 'q': v.X -= 1; break;
	case 'w': v.X += 1; break;
	case 'a': v.Y -= 1; break;
	case 's': v.Y += 1; break;
	case 'z': v.Z -= 1; break;
	case 'x': v.Z += 1; break;

	case 'e': rot.X -= 5; break;
	case 'r': rot.X += 5; break;
	case 'd': rot.Y -= 5; break;
	case 'f': rot.Y += 5; break;
	case 'c': rot.Z -= 5; break;
	case 'v': rot.Z += 5; break;

	default:
		event.Skip();
		return;
	}
	node->setPosition(v);
	bone->setRotation(rot);
	Refresh();
}

void Device3D::OnPaint(wxPaintEvent &event)
{
	/*UNREFERENCED_PARAMETER*/ (event);
	wxPaintDC dc(this);

	//_DEBUG_ DSTREAM << "PAINT TEST" << endl;

	if (irrDevice_)
	{
		irr_SceneManager* scenemgr = irrDevice_->getSceneManager();

		//forearm_L->setRotation(forearm_L->getRotation() + irr_Vector3D(0, 0, 0));
		//hand_->setRotation(hand_->getRotation() + irr_Vector3D(0, 0, 0));
		//upper_arm_L->updateAbsolutePosition();
		//upper_arm_L->updateAbsolutePositionOfAllChildren();

		//_DEBUG_ DSTREAM << "UPPER   (Parent) : " << upper_arm_L->getParent()->getName() << endl;
		//_DEBUG_ DSTREAM << "FOREARM (Parent) : " << forearm_L->getParent()->getName() << endl;
		//_DEBUG_ DSTREAM << "HAND    (Parent) : " << hand_->getParent()->getName() << endl;

		if (irrVideoDriver_)
		{
			irrVideoDriver_->beginScene(true, true, irr_Color(200, 0, 0, 0));

			if (scenemgr) scenemgr->drawAll();

			for (int i = -25; i < 25; ++i)
			{
				irrVideoDriver_->draw3DLine(irr_Vector3D(i, -10, -25),
										    irr_Vector3D(i, -10, 25), 
										    irr_Color(200, 130, 130, 130));

				irrVideoDriver_->draw3DLine(irr_Vector3D(-25, -10, i),
										    irr_Vector3D(25, -10, i), 
										    irr_Color(255 - (i + 25) * 2, 130, 130, 130));
			}

			irrVideoDriver_->endScene();
		}
	}
}

void Device3D::timerEvent(double ms)
{
	//static double lastMs = 0.0;
	//if (lastMs > 100.0 && ms != lastMs) {
	//	DEBUG_STREAM << ms - lastMs << endl;
	//}
	//lastMs = ms;
	auto* bone = getBone(BoneSide::RIGHT, BoneNumber::UPPER_ARM);
	irr_Vector3D rot = bone->getRotation();
	rot.X = 360.0 * ms / 1000.0;
	bone->setRotation(rot);
	Refresh();
}

//
//// http://irrlicht.sourceforge.net/forum/viewtopic.php?t=43514
//bool Device3D::Compute(irr_Bone* boneNode, irr_Bone* rootBoneNode, irr_Vector3D target)
//{
//    irr_Vector3D rootPos, curEnd, targetVector, desiredEnd, curVector, crossResult, endPos = target;
//    double cosAngle,turnAngle;
//    irr_Bone* link;
//    irr_Bone* end;
//
//    // start at the last link in the chain
//    end = boneNode;
//    link = (irr_Bone*)end->getParent();
//    int tries = 0;
//    double norm = 2;
//
//    int maxTries = 1000;
//    double error = 0.1;
//    double maxAngleChange = M_PI / 12.0;
//
//    while (++tries < maxTries && norm > 0.1)
//    {
//        link->updateAbsolutePosition();
//        end->updateAbsolutePosition();
//
//        rootPos = link->getAbsolutePosition();
//        curEnd = end->getAbsolutePosition();
//        desiredEnd = endPos;
//        norm = curEnd.getDistanceFromSQ(desiredEnd);
//
//        //std::cout << norm << std::endl;
//
//        // see if i'm already close enough
//        if (norm > error)
//        {
//            // create the vector to the current effector pos
//            curVector = curEnd - rootPos;
//            // create the desired effector position vector
//            targetVector = endPos - rootPos;
//
//            // normalize the vectors (expensive, requires a sqrt)
//            curVector.normalize();
//            targetVector.normalize();
//
//            // the dot product gives me the cosine of the desired angle
//            cosAngle = curVector.dotProduct(targetVector);
//            // if the dot product returns 1.0, i don't need to rotate as it is 0 degrees
//            if (cosAngle < 0.9999999)
//            {
//                // use the cross product to check which way to rotate
//                crossResult = curVector.crossProduct(targetVector);
//
//                crossResult.normalize();
//                turnAngle = acos(cosAngle);   // get the angle
//
//                if(turnAngle < maxAngleChange)
//                {
//                    turnAngle = maxAngleChange;
//                }
//
//                irr_quaternion rotation;
//                rotation = rotation.fromAngleAxis(turnAngle, crossResult);
//                rotation.normalize();
//                irr_Vector3D euRotation;
//                rotation.toEuler(euRotation);
//                link->setRotation(link->getRotation() + irr_Vector3D(euRotation.Z, euRotation.X, euRotation.Y));
//
//                //TODO: check DOF restrictions for link joint here.
//            }
//
//            if (link == rootBoneNode)
//            {
//                link = (irr_Bone*)end->getParent();   // start of the chain, restart
//            }
//            else
//            {
//                link = (irr_Bone*)link->getParent();
//            }
//        }
//    }
//
//    if (tries == maxTries)
//    {
//        return false;
//    }
//
//    return true;
//}