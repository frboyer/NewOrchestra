/// Pannel where the 3D model is shown.
/// \file Panel3D.cpp
/// \author Alexandre Arsenault and Francois-R.Boyer@PolyMtl.ca
/// \date 2014-03
/// Creation date 2013-01

#define IRR_TEST_BROKEN_QUATERNION_USE 0 //NOTE: A note in Irrlicht 1.8 sais the matrix to quaternion got fixe between 1.7 and 1.8, but it is still broken.  We use our own code instead (I lost too much time searching why my quaternion had problems!). By broken I mean that setting the rotation to the difference in absolute orientations between parent and child (using multiplication of quaternions got from absolute transformation matrix) does not give the correct rotation.


#include "Panel3D.h"
#include <windows.h>
#include <math.h>
#include "Range.h"
#define NO_CMT
#include "Motion\myAngles.h"
#undef NO_CMT
#include "Motion\simpleFileIO.hpp"
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

static const bool useMs3d = false; //NOTE: Obsolete; not currently working in useMs3d=true mode.

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

static const string sides[] = { "L", "R" };
string getBoneName(BoneSide boneSide, BoneNumber boneNumber)
{
	static const string boneNames[] = { "shoulder", "upper_arm", "forearm", "forearm.twist1", "forearm.twist2", "hand" };
	string result = boneNames[int(boneNumber)] + '.' + sides[int(boneSide)];
	return getBoneName(result);
}
string getFingerBoneName(BoneSide boneSide, int finger, int phalanx)
{
	static const string fingerNames[] = { "thumb", "index", "middle", "ring", "pinky" };
	string fingerBoneName;
	if (finger > 0)
		phalanx--;
	if (phalanx < 0)
		fingerBoneName = string("palm.0") + char('0' + finger);
	else
		fingerBoneName = ((finger != 0) ? "finger." : "") + fingerNames[finger] + ".0" + char('1' + phalanx);
	return getBoneName(fingerBoneName + '.' + sides[int(boneSide)]);
}

#pragma region Angle utility functions //{

static void setAbsoluteOrientation(irr_Node* bone, const VCNQuat& absoluteOrientation, const VCNQuat& absoluteOrientationOfParent)
{
	VCNQuat relative = ~absoluteOrientationOfParent * absoluteOrientation;
	irr_Vector3D eaRel; toIrrQuat(relative).toEuler(eaRel);
	bone->setRotation(toDegrees(eaRel));
}
static VCNQuat getAbsoluteOrientation(irr_Node* bone)
{
	return MatrixToQuaternion(toVcnMatrix(bone->getAbsoluteTransformation()));
}
static void setAbsoluteOrientation(irr_Node* bone, const VCNQuat& absoluteOrientation, bool doUpdate = true)
{
	if (doUpdate)
		bone->getParent()->updateAbsolutePosition(); //NOTE: This does not update the hierarchy, thus if parent absolute position is not updated it will not update correctly.
	setAbsoluteOrientation(bone, absoluteOrientation, getAbsoluteOrientation(bone->getParent()));
}

static void test_setAbsoluteOrientation(irr_Bone* bone, double angle)
{
	setAbsoluteOrientation(bone, axisAngleToQuaternion({ 0, 0, 1 }, angle));
	bone->updateAbsolutePosition();
	irr_Vector3D eaAbs; toIrrQuat(getAbsoluteOrientation(bone)).toEuler(eaAbs);
	cout << angle << "==" << eaAbs.Z << endl; // Absolute orientation is equal to the requested absolute orientation
}

static const VCNQuat modelToMotionFileReferencePosition = axisAngleToQuaternion({ 1, 0, 0 }, M_PI / 2) * axisAngleToQuaternion({ 0, 0, 1 }, -M_PI / 2); // Rotates model arm pointing down (+Y) with hand palm front (-Z) to motion arm reference position (zero orientation) pointing left of model (+X) with hand palm down (+Y).  (Axes in model coordinates, not motion file coordinates.)

// Model axes are Y down, Z back, while motion file axes are Z up, Y back.  This converts quaterions ftom motion file to model axes.
static VCNQuat motionFileToModelQuaternion(const VCNQuat& motionFileQuaternion)
{
	return VCNQuat(motionFileQuaternion.x, -motionFileQuaternion.z, motionFileQuaternion.y, motionFileQuaternion.w);
}

// In "hand coordinates", model fingers are pointing +Y with thumbs +X/-X, while motion file fingers are pointing +X with thumbs -Y/+Y.
static VCNQuat motionFileToModelFingerRelativeOrientation(const VCNQuat& motionFileRelativeFingerOrientation)
{
	return VCNQuat(-motionFileRelativeFingerOrientation.y, motionFileRelativeFingerOrientation.x, motionFileRelativeFingerOrientation.z, motionFileRelativeFingerOrientation.w);
}

// The reference arm orientation of model is down with palm forward, but is to left side with palm face down in motion file. 
static VCNQuat motionFileToModelArmOrientation(const VCNQuat& motionFileArmOrientation)
{
	return motionFileToModelQuaternion(motionFileArmOrientation) * modelToMotionFileReferencePosition;
}

// rotation is as given by getRotation (thus in degrees).
static VCNQuat rotationToAbsoluteOrientation(const irr_Vector3D& rotation, const VCNQuat& absoluteOrientationOfParent)
{
	return absoluteOrientationOfParent * toVcnQuat(irr_quaternion(toRadians(rotation)));
}

static VCNVector limbDirection(const VCNQuat& orientation)
{
	return orientation.Rotate(VCNVector(0.0, 1.0, 0.0));
}

#pragma endregion //}

void testRecursiveSetOrientation(irr_Bone* bone, const VCNQuat& parentOrientation = VCNQuat()) {
	bone->updateAbsolutePosition();
	//{ Something as simple as this, should not move any part: (it works)
	auto orientationA = rotationToAbsoluteOrientation(bone->getRotation(), parentOrientation);
	setAbsoluteOrientation(bone, orientationA, parentOrientation);
	bone->updateAbsolutePosition();
	//}
	//{ Something as simple as this, should not either move any part: (it does NOT work, maybe because of mirroring?)
	//auto orientationB = toVcnQuat(irr_quaternion(bone->getAbsoluteTransformation()));
	//setAbsoluteOrientation(bone, orientationB, parentOrientation);
	//bone->updateAbsolutePosition();
	//}
	//{ And then done with our own code: (it works)
	auto orientationC = getAbsoluteOrientation(bone);
	setAbsoluteOrientation(bone, orientationC, parentOrientation);
	bone->updateAbsolutePosition();
	//}

	//auto name = bone->getName();
	//auto parentOrientation = getAbsoluteOrientation(bone->getParent());
	//auto parentOrientationV = MatrixToQuaternion(toVcnMatrix(bone->getParent()->getAbsoluteTransformation()));
	//auto rotationA = bone->getRotation();
	//auto orientationA = getAbsoluteOrientation(bone);
	//auto orientationAV = MatrixToQuaternion(toVcnMatrix(bone->getAbsoluteTransformation()));
	//auto orientationB = rotationToAbsoluteOrientation(rotationA, parentOrientation);  orientationB.Normalize();
	////setAbsoluteOrientation(bone, orientationB, false);
	//auto relative = ~parentOrientation * orientationB;
	//irr_Vector3D rotationB; toIrrQuat(relative).toEuler(rotationB); rotationB = toDegrees(rotationB);
	//bone->setRotation(rotationB);
	//bone->updateAbsolutePosition();
	//auto orientationC = getAbsoluteOrientation(bone); orientationC.Normalize();

	//setAbsoluteOrientation(bone, toVcnQuat(bone->getAbsoluteTransformation()), true);

	for (auto&& child : bone->getChildren()) {
		auto childBone = dynamic_cast<irr_Bone*>(child);
		if (childBone != nullptr)
			testRecursiveSetOrientation(childBone, orientationC);
	}
}

struct vector2d { double x, y; };
static inline vector2d polarToCartesian(double radius, double angle)
{
	return { cos(angle)*radius, sin(angle)*radius };
}
static irr_Vector3D sphericalToCartesian(double radius, double azimuth, double elevation)
{
	auto vAzimuth = polarToCartesian(1.0, azimuth), vElevation = polarToCartesian(radius, elevation);
	return irr_Vector3D(vAzimuth.y * vElevation.x, vElevation.y, -vAzimuth.x * vElevation.x);
}

static void tests();

Device3D::Device3D(wxWindow* win, const wxWindowID& id,
				   wxPoint point, wxSize size,
				   irr_DriverType type, bool bResizeable) :
				   // Heritage
				   wxWindow(win, id, point, size, wxWANTS_CHARS)
{
	tests();

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
	camera_ = nullptr;
	cameraDistance_ = 23.0;
	SetFrontAlign();

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
	for (auto side : enumerate<BoneSide>()) {
		for (auto bone : enumerate<BoneNumber>())
			getBone(side, bone) = m_node->getJointNode(getBoneName(side, bone).c_str());
		for (auto finger : range(0, 5)) {
			for (auto phalanx : range(0, 4)) {
				auto boneName = getFingerBoneName(side, finger, phalanx);
				auto bone = m_node->getJointNode(boneName.c_str());
				getFingerBone(side, finger, phalanx) = bone;
			}
		}
		eyeRigs_[int(side)] = m_node->getJointNode(getBoneName(string("eye.") + sides[int(side)]).c_str());
		eyeModels_[int(side)] = m_node->getJointNode((string("eye_") + sides[int(side)]).c_str());
	}
	head_ = m_node->getJointNode(getBoneName("head").c_str());

	//FIXME: Why are hips incorrectly aligned on import?
	irr_Bone* hips = m_node->getJointNode(getBoneName("hips").c_str());
	if (hips)
	{
		const irr_Vector3D& r = hips->getRotation();
		DEBUG_STREAM << "Hips rotation: " << r.X << "," << r.Y << "," << r.Z << endl;
		hips->setRotation(irr_Vector3D(0, 0, 0));
	}
	//TODO: Can we suppose that node 0 is always root node?
	//testRecursiveSetOrientation(m_node->getJointNode(0U));
	m_node->getJointNode(0U)->updateAbsolutePositionOfAllChildren();
	for (auto side : enumerate<BoneSide>())
		shoulderRestOrientations_[int(side)] = getAbsoluteOrientation(getBone(side, BoneNumber::SHOULDER));
	ribsOrientation_ = getAbsoluteOrientation(getBone(BoneSide::LEFT, BoneNumber::SHOULDER)->getParent()); //NOTE: Both shoulders have same parent.

	// Display all bones.
	DEBUG_STREAM << "N BONES : " << m_node->getJointCount() << endl;
	for (unsigned i = 0; i < m_node->getJointCount(); ++i)
	{
		irr_Bone* bone = m_node->getJointNode(i);
		const irr_Vector3D& r = bone->getRotation();
		DEBUG_STREAM << "BONE NAME : " << i << " " << bone->getName() << " : " << moduloAngleDegrees(r.X) << "," << moduloAngleDegrees(r.Y) << "," << moduloAngleDegrees(r.Z) << endl;
	}

	// Display hierarchy from one hand.
	DEBUG_STREAM << "Hierarchy for hand : {" << endl;
	for (irr_Bone* bone = getBone(BoneSide::RIGHT, BoneNumber::HAND); bone != nullptr; bone = dynamic_cast<irr_Bone*>(bone->getParent()))
		DEBUG_STREAM << "  " << bone->getName() << endl;
	DEBUG_STREAM << "}" << endl;

	// Display hierarchy from one eye, and children of head. (Eyes are not moving with the head!?)
	DEBUG_STREAM << "Hierarchy for eye (rig) : {" << endl;
	for (irr_Bone* bone = m_node->getJointNode(getBoneName("eye.R").c_str()); bone != nullptr; bone = dynamic_cast<irr_Bone*>(bone->getParent()))
		DEBUG_STREAM << "  " << bone->getName() << endl;
	DEBUG_STREAM << "}" << endl;
	DEBUG_STREAM << "Hierarchy for eye : {" << endl;
	for (irr_Bone* bone = m_node->getJointNode("eye_R"); bone != nullptr; bone = dynamic_cast<irr_Bone*>(bone->getParent()))
		DEBUG_STREAM << "  " << bone->getName() << endl;
	DEBUG_STREAM << "}" << endl;
	DEBUG_STREAM << "Children of head : {" << endl;
	for (auto&& bone : m_node->getJointNode(getBoneName("head").c_str())->getChildren())
		DEBUG_STREAM << "  " << bone->getName() << endl;
	DEBUG_STREAM << "}" << endl;


	// Load motion.
	//TODO: Should load on demand when selecting menu item.
	{
		static const std::string baseDir = "Ressources/";
		static const std::string fileName = baseDir + "Varese Arcana.mtn"; // "Stravinsky Sacre 1avant142 a 149_export.mtn"; // "exampleFile_export.mtn"; // 
		std::ifstream f(fileName, std::ios::binary);
		readVector(f, m_motion, getFileSize(f) / sizeof(MotionFilePacket));
	}

	//Compute(hand_, (irr_Bone*) upper_arm_L->getParent(), hand_->getPosition());

	//TODO: Get T-stand "standard" rotations.
	//getBone(BoneSide::RIGHT, BoneNumber::UPPER_ARM)->getRotation();
	//getBone(BoneSide::RIGHT, BoneNumber::FOREARM)->getRotation();
	//getBone(BoneSide::RIGHT, BoneNumber::HAND)->getRotation();

	//getBone(BoneSide::LEFT, BoneNumber::SHOULDER)->setRotation(irr_Vector3D(0, 90, 90));
	//getBone(BoneSide::RIGHT, BoneNumber::SHOULDER)->setRotation(irr_Vector3D(0, -90, -90));
	//for (auto side : enumerate<BoneSide>()) {
	//	getBone(side, BoneNumber::UPPER_ARM)->setRotation(irr_Vector3D(0, 0, 0));
	//	getBone(side, BoneNumber::FOREARM_TWIST1)->setRotation(irr_Vector3D(0, 0, 0));
	//	getBone(side, BoneNumber::FOREARM_TWIST2)->setRotation(irr_Vector3D(0, 0, 0));
	//	getBone(side, BoneNumber::FOREARM)->setRotation(irr_Vector3D(0, 0, 0));
	//	getBone(side, BoneNumber::HAND)->setRotation(irr_Vector3D(0, 0, 0));
	//}
	timerEvent(0.0);
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
	static const double minimumRatio = 1.2;
	double rectRatio = double(size.x) / size.y;
	camera_->setAspectRatio(irr::f32(rectRatio));  // This will keep vertical FOV fixed but change horizontal FOV to keep correct pixel apect ratio.
	cameraDistance_ = 23.0 * std::max(1.0, minimumRatio/rectRatio);

	//scaleImage(_markerData[_selectedMarker].numImg);
	//_currentMarkerImg = createSelectedMarkerImage(_selectedMarker,
	//	wxColor(180, 180, 180));
	//Refresh();

	irrVideoDriver_->OnResize(irr::core::dimension2d<irr::u32>(size.x, size.y));
	//SetSize(newSize);
	updateCamera();
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
		static const double pixelToAngleFactor = 0.01;
		cameraHorizontalAngle_ -= (x - lastX) * pixelToAngleFactor;  cameraHorizontalAngle_ = moduloAngle(cameraHorizontalAngle_);
		cameraVerticalAngle_ += (y - lastY) * pixelToAngleFactor;  clamp(cameraVerticalAngle_, -M_PI / 2 * .9, M_PI / 2 * .99); //NOTE: Don't allow pi/2 or there will be a problem with the up vector.
		updateCamera();
		lastX = x; lastY = y;

		Refresh();
	}
	else lastX = lastY = 0;
}

void Device3D::OnMouseLeftDown(wxMouseEvent& event)
{
	clickPt = event.GetPosition();
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

static void activateTranslucentLineMaterial(irr::video::IVideoDriver* irrVideoDriver_)
{
	irr::video::SMaterial material;
	material.Lighting = false;
	material.MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	irrVideoDriver_->setMaterial(material);
}

static void drawAxes(irr::video::IVideoDriver* irrVideoDriver_)
{
	static const irr::f32 axesLength = 2;
	static const irr::u32 axesIntensity = 255, axesAlpha = 100;
	static const irr_Vector3D axesCenter(-5, -12, 0);
	static const struct { irr_Vector3D direction; irr_Color color; } axes[] = {
			{ { axesLength, 0, 0 }, { axesAlpha, axesIntensity, 0, 0 } }, // +X red
			{ { 0, axesLength, 0 }, { axesAlpha, 0, axesIntensity, 0 } }, // +Y green
			{ { 0, 0, axesLength }, { axesAlpha, 0, 0, axesIntensity } }, // +Z blue
	};
	activateTranslucentLineMaterial(irrVideoDriver_);
	// irrVideoDriver_->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix); //NOTE: We currently draw axes in model space; we could set the transform to draw axes in view world space.
	for (auto&& axis : axes)
		irrVideoDriver_->draw3DLine(axesCenter, axesCenter + axis.direction, axis.color);
}

static void drawCenteredLine(irr::video::IVideoDriver* irrVideoDriver_, const irr_Vector3D& center, const irr_Vector3D& halfLength, const irr_Color& color)
{
	irrVideoDriver_->draw3DLine(center - halfLength, center + halfLength, color);
}

static void drawGrid(irr::video::IVideoDriver* irrVideoDriver_)
{
	static const irr::f32 gridSpacing = 1.0;
	static const int gridLineCount = 50;
	static const irr_Vector3D gridCenter(0, -10, 0);

	static const irr::f32 gridRadius = gridSpacing * gridLineCount / 2.0;
	static const irr_Vector3D frontBackLine(0, 0, gridRadius);
	static const irr_Vector3D leftRightLine(gridRadius, 0, 0);
	activateTranslucentLineMaterial(irrVideoDriver_);
	for (auto i : range(0, gridLineCount))
	{
		irr::f32 pos = -gridRadius + i*gridSpacing;
		drawCenteredLine(irrVideoDriver_, gridCenter + irr_Vector3D(pos, 0, 0), frontBackLine, irr_Color(70, 130, 130, 130));
		drawCenteredLine(irrVideoDriver_, gridCenter + irr_Vector3D(0, 0, pos), leftRightLine, irr_Color(250 - i * 230 / gridLineCount, 130, 130, 130));
	}

}

void Device3D::OnPaint(wxPaintEvent &event)
{
	(event);
	wxPaintDC dc(this);

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

			//drawGrid(irrVideoDriver_);
			//drawAxes(irrVideoDriver_);

			irrVideoDriver_->endScene();
		}
	}
}

void Device3D::timerEvent(double ms)
{
	// NOTE: getAbsoluteTransformation is model space, not world space; the axes here are thus model space.
	// Motion files have  +X left of model, +Y back of model, +Z up            (right handed).
	// Here, the axes are +X left of model, +Y *down*,        +Z back of model (right handed).
	// As the model is usually looking at us, left of model is right of screen, and back of model is into screen.
	// The absolute rotations are also relative to what is the "zero" rotation for a given model part:
	// Motion files have arms zero rotation as pointing +X (left of model) with palm -Z (down).
	// Here, arms zero rotation are pointing +Y (down) with palm -Z (front of model).

	//double angle = moduloAngleRadians(ms * 2 * M_PI / 1000.0);
	//test_setAbsoluteOrientation(getBone(BoneSide::RIGHT, BoneNumber::UPPER_ARM), angle); // Test absolute to relative rotations.
	//setAbsoluteOrientation(getBone(BoneSide::RIGHT, BoneNumber::UPPER_ARM), motionFileToModelArmOrientation(axisAngleToQuaternion({ 0, 0, 1 }, -M_PI/4)));
	//setAbsoluteOrientation(getFingerBone(BoneSide::LEFT, 1, 1), motionFileToModelArmOrientation(VCNQuat()));

	static const double motionFileFrameRate = 60.0;
	auto& frame = m_motion[irr::core::clamp(unsigned(ms * (motionFileFrameRate / 1000.0) + .5), 0U, m_motion.size() - 1)];

	static const BoneNumber bonesOrder[] = { BoneNumber::UPPER_ARM, BoneNumber::FOREARM, BoneNumber::HAND };
	static const BoneSide sidesOrder[] = { BoneSide::RIGHT, BoneSide::LEFT };
	for (int sideIndex : range(0, 2)) {
		auto& arm = frame.armsDirections[sideIndex];
		BoneSide side = sidesOrder[sideIndex];
		VCNQuat partOrientationsInMotionFileReference[] = { arm.armOrientation[0], arm.armOrientation[1], arm.handOrientation };

		VCNQuat parentOrientation;
		for (int i : range(0, 3)) {
			auto bone = getBone(side, bonesOrder[i]);
			VCNQuat orientation = motionFileToModelArmOrientation(partOrientationsInMotionFileReference[i]);
			if (i == 0) { // Do some inverse kinematics for shoulders.
				static const double shoulderFrontBackMotionFactor = 0.3;
				static const double shoulderUpMotionFactor = 0.5;
				auto shoulder = getBone(side, BoneNumber::SHOULDER);
				auto upperArmDirection = limbDirection(orientation);
				double armHeading = (side == BoneSide::LEFT) ? atan2(-upperArmDirection.z, upperArmDirection.x) : atan2(upperArmDirection.z, -upperArmDirection.x);
				double armAttitude = asin(-upperArmDirection.y);
				auto shoulderOrientation =
					axisAngleToQuaternion({ 0, 1, 0 }, armHeading * shoulderFrontBackMotionFactor)
					* axisAngleToQuaternion({ 0, 0, (side == BoneSide::RIGHT ? 1.0 : -1.0) }, std::max(0.0, armAttitude) * shoulderUpMotionFactor)
					* shoulderRestOrientations_[int(side)];
				setAbsoluteOrientation(shoulder, shoulderOrientation, ribsOrientation_);
				parentOrientation = shoulderOrientation;

				if (side == BoneSide::RIGHT) { // Give a little beat to the head, so it looks less static.
					head_->setRotation(irr_Vector3D(toDegrees(armAttitude)*.1 + 13.5, 0.0, 0.0));
					head_->updateAbsolutePosition();

					// Update eyes positions, which do not follow head!
					auto parent = eyeModels_[0]->getParent();
					auto inverseParentOrientation = ~getAbsoluteOrientation(parent);
					auto parentTranslation = parent->getAbsoluteTransformation().getTranslation();
					for (auto eyeSide : enumerate<BoneSide>()) {
						auto eyeRig = eyeRigs_[int(eyeSide)];
						eyeRig->updateAbsolutePosition();
						auto eyeRigTransformation = eyeRig->getAbsoluteTransformation();
						auto eyeRigOrientation = getAbsoluteOrientation(eyeRig);
						auto eyeRigTranslation = eyeRig->getAbsoluteTransformation().getTranslation();
						// To simplify, we know the scale is 1 (except we have a mirror).

						auto eye = eyeModels_[int(eyeSide)];
						auto parentOrientation = getAbsoluteOrientation(parent);
						auto parentTranslation = parent->getAbsoluteTransformation().getTranslation();
						auto relativePosition = (~parentOrientation).Rotate(toVcnVector(eyeRigTranslation - parentTranslation));

						// first slow implementation:
						//auto inverseParentTransformation = eye->getParent()->getAbsoluteTransformation(); inverseParentTransformation.makeInverse();
						//auto relativeTransformation = inverseParentTransformation * eyeRigTransformation;
						//auto relativePosition = relativeTransformation.getTranslation();

						relativePosition.x = -relativePosition.x;
						auto eyePos = eye->getPosition();
						eye->setPosition(toIrrVector(relativePosition));
						eye->setRotation(irr_Vector3D(5, 0, 0));
						//m_node->getJointNode("eye_R");
					}
				}
			}
			setAbsoluteOrientation(bone, orientation, parentOrientation);
			if (i == 1) {
				VCNQuat startOrientation = quaternionBetweenVectors(limbDirection(parentOrientation), limbDirection(orientation)) * parentOrientation;
				VCNQuat twist1 = VCNQuat::Slerp(startOrientation, orientation, 0.5);
				VCNQuat twist2 = VCNQuat::Slerp(startOrientation, orientation, 0.75);
				setAbsoluteOrientation(getBone(side, BoneNumber::FOREARM_TWIST1), twist1, parentOrientation);
				setAbsoluteOrientation(getBone(side, BoneNumber::FOREARM_TWIST2), twist2, twist1);
			}
			parentOrientation = orientation;
		}
		VCNQuat handOrientation = parentOrientation;

		for (auto fingerIndex : range(1, 5)) {//TODO: Thumb.
			auto& finger = arm.fingers[fingerIndex];
			parentOrientation = rotationToAbsoluteOrientation(getFingerBone(side, fingerIndex, 0)->getRotation(), handOrientation); // Finger is relative to corresponding palm bone in model.
			//VCNQuat orientation = handOrientation * motionFileToModelFingerRelativeOrientation(arm.fingers[fingerIndex].relativeOrientation); // Finger is relative to hand in motion file.
			VCNQuat orientation = handOrientation * axisAngleToQuaternion({ 1, 0, 0 }, toRadians(-100 * finger.flexion)); //TODO: Fix fingers so the look correct; currently using only the second joint flexion looks better than using all sensors.
			setAbsoluteOrientation(getFingerBone(side, fingerIndex, 1), orientation, parentOrientation);

			for (auto phalanxIndex : range(2, 4)) {
				getFingerBone(side, fingerIndex, phalanxIndex)->setRotation(irr_Vector3D(-100 * finger.flexion, 0, 0));
			}
		}
	}
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

static irr_Vector3D toVector3D(const VCNVector& v) { return irr_Vector3D(irr::f32(v.x), irr::f32(v.y), irr::f32(v.z)); }

static void tests() {
	VCNVector axes[] = {
		{1,0,0},
		{0,1,0},
		{0,0,1}
	};
	struct {
		irr_quaternion irrQuat;
		VCNQuat vcnQuat;                  // Quaternions are equal for same axis/angle rotation.
		irr_Vector3D irrEuler;            // X=bank(roll), Y=attitude(pitch), Z=heading(yaw) (from Irrlicht quaternion.h comments); Irrlicht comments are for an airplane facing X with Z up.
		double vcnPitch, vcnYaw, vcnRoll; // X=pitch, Y=yaw, Z=roll; VCN names are for an airplane facing Z with Y up.
		                                  // irrEuler has same values, in same order, as euler angles in VCN (VCNQuat's SetFromEuler and GetEulers); even the wrap arounds are the same (Y, the second value, are in [-pi/2,+pi/2], the others wrap at -pi/+pi) thus VCN names are incorrect.
	} rotations[3];
	for (auto i : range(0,3)) {
		auto& rotation = rotations[i];
		rotation.irrQuat.fromAngleAxis(M_PI / 4, toVector3D(axes[i]));
		rotation.vcnQuat = axisAngleToQuaternion(axes[i], M_PI / 4);
		rotation.irrQuat.toEuler(rotation.irrEuler);
		rotation.vcnQuat.GetEulers(&rotation.vcnPitch, &rotation.vcnYaw, &rotation.vcnRoll);
	}
	for (double y = 0; y < 3 * M_PI; y += M_PI / 4) {
		VCNQuat vcnQuat; vcnQuat.SetFromEuler(0.1, y, 0.5);
		irr_quaternion irrQuat = toIrrQuat(vcnQuat);
		irr_Vector3D irrEuler; irrQuat.toEuler(irrEuler); // X in [-pi,+pi], Y in [-pi/2,+pi/2], Z in [-pi,+pi]
		double vcnPitch, vcnYaw, vcnRoll;
		vcnQuat.GetEulers(&vcnPitch, &vcnYaw, &vcnRoll); // vcnPitch in [-pi,+pi], vcnYaw in [-pi/2,+pi/2], vcnRoll in [-pi,+pi]; names in VCN are thus incorrect, as standard is that pitch is [-pi/2,+pi/2], not yaw.
	}
	VCNQuat x = axisAngleToQuaternion({ 1, 0, 0 }, M_PI / 4), y = axisAngleToQuaternion({ 0, 1, 0 }, M_PI / 8);
	VCNQuat vcnMult = y * x;
	irr_quaternion irrMult = toIrrQuat(x) * toIrrQuat(y); // Irrlicht multiply in reverse order of "standard".
}

void Device3D::updateCamera()
{

	static const irr_Vector3D cameraLookAt(0.0, 18.2, -1.0);

	irr_Vector3D cameraPosition = cameraLookAt + sphericalToCartesian(cameraDistance_, cameraHorizontalAngle_, cameraVerticalAngle_);
	if (camera_ == nullptr) {
		camera_ = addCamera(0, cameraPosition, cameraLookAt);
		camera_->setFOV(toRadians(27.0f)); // Vertical field of view of a 50mm lens on 35mm film.
	}
	else
		camera_->setPosition(cameraPosition);
	Refresh();
}

void Device3D::SetLeftAlign()
{
	cameraHorizontalAngle_ = toRadians(90.0);
	cameraVerticalAngle_ = toRadians(5.0f);
	updateCamera();
}
void Device3D::SetFrontAlign()
{
	cameraHorizontalAngle_ = toRadians(0.0);
	cameraVerticalAngle_ = toRadians(5.0f);
	updateCamera();
}
void Device3D::SetRightAlign()
{
	cameraHorizontalAngle_ = toRadians(-90.0);
	cameraVerticalAngle_ = toRadians(5.0f);
	updateCamera();
}