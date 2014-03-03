/// @todo Needs Documentation Header.
#include "Panel3D.h"
#include <windows.h>
#include <cmath>
#include "Debug.h"

BEGIN_EVENT_TABLE(Device3D, wxWindow)
	EVT_SIZE(Device3D::OnSize)
	EVT_PAINT(Device3D::OnPaint)
	EVT_LEFT_UP(Device3D::OnMouseLeftUp)
	EVT_MOTION(Device3D::OnMouseMotion)
	EVT_LEFT_DOWN(Device3D::OnMouseLeftDown)
END_EVENT_TABLE()

Device3D::Device3D(wxWindow* win, wxPoint point, 
				   wxSize size, irr_DriverType type, 
				   bool bResizeable) :
				   // Heritage
				   wxWindow(win, wxID_ANY, point, size)
{
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	//SetBackgroundColour(wxColor(0, 100, 0));
	irr_CreateParam param;

	//Window Only
	param.WindowId = reinterpret_cast<void *> ((HWND) this->GetHandle());
	param.DriverType = type;
	param.Doublebuffer = true;
	param.WindowSize = irr_Dimension2D(size.x, size.y);

	irrDevice = irr::createDeviceEx(param);

	theta = M_PI * 0.5;

	float v1_x = 0.0,
		  v1_y = 20.0,
		  v1_z = -12,

		  v2_x = 0.0,
		  v2_y = 18.0,
		  v2_z = 0.0;

	camera_ = AddCamera(0, irr_Vector3D(v1_x, v1_y, v1_z), 
						irr_Vector3D(v2_x, v2_y, v2_z));

	GetSceneManager()->addLightSceneNode(0, irr_Vector3D(15, 0, 15));

	irr_Mesh* mesh = GetSceneManager()->getMesh("Ressources/male.x");
	GetSceneManager()->getMeshManipulator()->flipSurfaces(mesh);

	m_node = GetSceneManager()->addAnimatedMeshSceneNode(mesh);

	if (m_node)
	{
		m_node->setPosition(irr_Vector3D(0, 0, 0));
		m_node->setJointMode(irr::scene::EJUOR_CONTROL);
		m_node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
		m_node->setMaterialFlag(irr::video::EMF_TRILINEAR_FILTER, true);
		m_node->setMaterialFlag(irr::video::EMF_ANTI_ALIASING, true);
		m_node->setRotation(irr_Vector3D(180, 180, 0));
		m_node->setScale(irr_Vector3D(1.3, 1.3, 1.3));
		m_node->setMaterialTexture(0, irrDevice->getVideoDriver()->getTexture("Ressources/male_c.png"));

		//leftBoneNumber = 
		// Afficher tous les bones.
		irr_Bone* bone;
		for(int i = 0; i < m_node->getJointCount(); ++i)
		{
			bone = m_node->getJointNode(i);
			DEBUG_STREAM << "BONE NAME : " << i << " " << bone->getName() << endl;
		}

		m_node->animateJoints();
		m_node->getJointNode("pc_rig_hips")->setRotation(irr_Vector3D(0, 0, 0));
		m_node->getJointNode("jaw")->setRotation(irr_Vector3D(0, 0, 0));

		//Compute(hand_, (irr_Bone*) upper_arm_L->getParent(), hand_->getPosition());
		//hips_->setRotation(irr_Vector3D(0, 0, 0));
		//upper_arm_L->setRotation(upper_arm_L->getRotation() + irr_Vector3D(20, 20, 40));
		//hand_->setRotation(hand_->getRotation() + irr_Vector3D(-5, -10, -10));
	}

	////window->SetDevice(this);
	if (irrDevice) irrDevice->setResizable(bResizeable);
}

Device3D::~Device3D()
{
	if (irrDevice)
	{
		irrDevice->closeDevice();
		irrDevice->drop();
	}
}

void Device3D::OnSize(wxSizeEvent& event)
{
	wxSize size(event.GetSize());
	SetSize(size);

	//scaleImage(_markerData[_selectedMarker].numImg);
	//_currentMarkerImg = createSelectedMarkerImage(_selectedMarker,
	//	wxColor(180, 180, 180));
	//Refresh();

	irrDevice->getVideoDriver()->OnResize(irr::core::dimension2d<irr::u32>(size.x, size.y));
	//SetSize(newSize);
	Refresh();
}

irr_Camera* Device3D::AddCamera(irr_Node* parent, 
								irr_Vector3D& position, 
								irr_Vector3D& lookat, 
								irr_S32 id)
{
	irr_SceneManager* irrSceneMgr = GetSceneManager();

	if (irrSceneMgr) 
		return (irr_Camera*)irrSceneMgr->addCameraSceneNode(parent, position, lookat);

	else return NULL;
}

void Device3D::mSize(const wxSize& newSize)
{
	irrDevice->getVideoDriver()->OnResize(irr::core::dimension2d<irr::u32>(newSize.x, newSize.y));
	SetSize(newSize);
	Refresh();
}

void Device3D::OnMouseLeftUp(wxMouseEvent& event)
{
	if (HasCapture())
	{
		ReleaseMouse();
	}
}

////template<typename T>
////inline void clamp(T& x, T min, T max)
////{
////	x < min ? x = min : (x > max ? x = max);
////}

void Device3D::OnMouseMotion(wxMouseEvent& event)
{
	int x = event.GetX() - clickPt.x;
	int y = event.GetY() - clickPt.y;

	if (event.Dragging() && HasCapture() && event.LeftIsDown())
	{
		irr_Bone* bone = getBone(BONE_LEFT, UPPER_ARM);
		irr_Vector3D rot = bone->getRotation() + irr_Vector3D(x, y, 0);

		if (rot.X < 280)	rot.X = 280;	
		if (rot.X > 405)	rot.X = 405;	

		if (rot.Y < -30)	rot.Y = -30;	
		if (rot.Y > 66)		rot.Y = 66;

		//m_node->getJointNode("pc_rig_jaw")->setRotation(rot);
		bone->setRotation(rot);

		Refresh();

	}
}

irr_Bone* Device3D::getBone(const int& side, const BonesList& bones)
{
	/// @todo Block value.
	return m_node->getJointNode(side + bones);
}

void Device3D::OnMouseLeftDown(wxMouseEvent& event)
{
	clickPt = event.GetPosition();
	//_DEBUG_ DSTREAM << "LEFT DOWN" << endl;
	CaptureMouse();
}

void Device3D::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);

	//_DEBUG_ DSTREAM << "PAINT TEST" << endl;

	if (irrDevice)
	{
		irr_VideoDriver* driver = irrDevice->getVideoDriver();
		irr_SceneManager* scenemgr = irrDevice->getSceneManager();

		//forearm_L->setRotation(forearm_L->getRotation() + irr_Vector3D(0, 0, 0));
		//hand_->setRotation(hand_->getRotation() + irr_Vector3D(0, 0, 0));
		//upper_arm_L->updateAbsolutePosition();
		//upper_arm_L->updateAbsolutePositionOfAllChildren();

		//_DEBUG_ DSTREAM << "UPPER   (Parent) : " << upper_arm_L->getParent()->getName() << endl;
		//_DEBUG_ DSTREAM << "FOREARM (Parent) : " << forearm_L->getParent()->getName() << endl;
		//_DEBUG_ DSTREAM << "HAND    (Parent) : " << hand_->getParent()->getName() << endl;

		if (driver)
		{
			driver->beginScene(true, true, irr_Color(200, 0, 0, 0));

			if (scenemgr) scenemgr->drawAll();

			for (int i = -25; i < 25; ++i)
			{
				driver->draw3DLine(irr_Vector3D(i, -10, -25), 
								   irr_Vector3D(i, -10, 25), 
								   irr_Color(200, 130, 130, 130));

				driver->draw3DLine(irr_Vector3D(-25, -10, i), 
								   irr_Vector3D(25, -10, i), 
								   irr_Color(255 - (i + 25) * 2, 130, 130, 130));
			}

			driver->endScene();
		}
	}
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
//                irr_quatornion rotation;
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