#include "HXInspectorWidget.h"
#include <QLabel.h>
//#include <QSpinBox.h>
#include <QBoxLayout.h>
#include <QFormLayout>
#include "HXFogLinear.h"
#include <QHeaderView.h>
#include "HXGLCamera.h"
#include "HXSceneManager.h"
#include "HXRenderSystem.h"
#include "HXEditorWin.h"
#include "HXHierarchyWidget.h"
#include <QMessageBox.h>

HXInspectorWidget::HXInspectorWidget(QWidget* parent) : QTreeWidget(parent)
, selectedGameObject(NULL)
, selectedLight(NULL)
, fogData(NULL)
, ambientData(NULL)
, selectedCamera(NULL)
{
	// fog
	checkboxFog = new QCheckBox();
	//checkboxFog->setText("use fog");

	comboboxFogType = new QComboBox();
	comboboxFogType->addItem("FOG_Linear");
	comboboxFogType->addItem("FOG_EXP");
	comboboxFogType->addItem("FOG_EXPX");

	spinboxFogColorR = new QSpinBox();
	spinboxFogColorR->setRange(COLOR_MIN, COLOR_MAX);

	spinboxFogColorG = new QSpinBox();
	spinboxFogColorG->setRange(COLOR_MIN, COLOR_MAX);

	spinboxFogColorB = new QSpinBox();
	spinboxFogColorB->setRange(COLOR_MIN, COLOR_MAX);

	spinboxFogStart = new QDoubleSpinBox();
	spinboxFogStart->setRange(MIN, MAX);

	spinboxFogEnd = new QDoubleSpinBox();
	spinboxFogEnd->setRange(MIN, MAX);

	// ambient
	spinboxAmbientColorR = new QSpinBox();
	spinboxAmbientColorR->setRange(COLOR_MIN, COLOR_MAX);

	spinboxAmbientColorG = new QSpinBox();
	spinboxAmbientColorG->setRange(COLOR_MIN, COLOR_MAX);

	spinboxAmbientColorB = new QSpinBox();
	spinboxAmbientColorB->setRange(COLOR_MIN, COLOR_MAX);

	// camera
	editCameraName = new QLineEdit();

	spinboxCameraNear = new QDoubleSpinBox();
	spinboxCameraNear->setRange(MIN, MAX);

	spinboxCameraFar = new QDoubleSpinBox();
	spinboxCameraFar->setRange(MIN, MAX);

	comboboxCullingMask = new QComboBox();
	comboboxCullingMask->addItem("Nothing");
	comboboxCullingMask->addItem("Everything");
	comboboxCullingMask->addItem("Default");
	comboboxCullingMask->addItem("UI");
	comboboxCullingMask->addItem("Editor");
	comboboxCullingMask->addItem("Water");

	pushbuttonCameraTransSync = new QPushButton();
	pushbuttonCameraTransSync->setText("Sync transform");

	spinboxCameraPositionX = new QDoubleSpinBox();
	spinboxCameraPositionX->setRange(MIN, MAX);

	spinboxCameraPositionY = new QDoubleSpinBox();
	spinboxCameraPositionY->setRange(MIN, MAX);

	spinboxCameraPositionZ = new QDoubleSpinBox();
	spinboxCameraPositionZ->setRange(MIN, MAX);

	spinboxCameraRotationX = new QDoubleSpinBox();
	spinboxCameraRotationX->setRange(MIN, MAX);

	spinboxCameraRotationY = new QDoubleSpinBox();
	spinboxCameraRotationY->setRange(MIN, MAX);

	spinboxCameraRotationZ = new QDoubleSpinBox();
	spinboxCameraRotationZ->setRange(MIN, MAX);

	// gameobject
	checkboxActivity = new QCheckBox();

	checkboxStatic = new QCheckBox();

	editGameObjectName = new QLineEdit();

	checkboxCastShadow = new QCheckBox();

	comboboxLayer = new QComboBox();
	comboboxLayer->addItem("Default");
	comboboxLayer->addItem("UI");
	comboboxLayer->addItem("Editor");
	comboboxLayer->addItem("Water");

	spinboxPositionX = new QDoubleSpinBox();
	spinboxPositionX->setRange(MIN, MAX);

	spinboxPositionY = new QDoubleSpinBox();
	spinboxPositionY->setRange(MIN, MAX);

	spinboxPositionZ = new QDoubleSpinBox();
	spinboxPositionZ->setRange(MIN, MAX);

	spinboxRotationX = new QDoubleSpinBox();
	spinboxRotationX->setRange(MIN, MAX);

	spinboxRotationY = new QDoubleSpinBox();
	spinboxRotationY->setRange(MIN, MAX);

	spinboxRotationZ = new QDoubleSpinBox();
	spinboxRotationZ->setRange(MIN, MAX);

	spinboxScaleX = new QDoubleSpinBox();
	spinboxScaleX->setRange(MIN, MAX);
	spinboxScaleX->setDecimals(3);

	spinboxScaleY = new QDoubleSpinBox();
	spinboxScaleY->setRange(MIN, MAX);
	spinboxScaleY->setDecimals(3);

	spinboxScaleZ = new QDoubleSpinBox();
	spinboxScaleZ->setRange(MIN, MAX);
	spinboxScaleZ->setDecimals(3);

	// light
	editLightName = new QLineEdit();
	checkboxLight = new QCheckBox();
	comboboxLightType = new QComboBox();
	comboboxLightType->addItem("LIGHT_AMBIENT");
	comboboxLightType->addItem("LIGHT_DIRECTION");
	comboboxLightType->addItem("LIGHT_POINT");
	comboboxLightType->addItem("LIGHT_SPOT");
	spinboxShininess = new QDoubleSpinBox();
	spinboxShininess->setRange(MIN, MAX);
	spinboxStrength = new QDoubleSpinBox();
	spinboxStrength->setRange(MIN, MAX);
	spinboxConstantAttenuation = new QDoubleSpinBox();
	spinboxConstantAttenuation->setRange(MIN, MAX);
	spinboxLinearAttenuation = new QDoubleSpinBox();
	spinboxLinearAttenuation->setRange(MIN, MAX);
	spinboxQuadraticAttenuation = new QDoubleSpinBox();
	spinboxQuadraticAttenuation->setRange(MIN, MAX);
	spinboxSpotCosCutoff = new QDoubleSpinBox();
	spinboxSpotCosCutoff->setRange(MIN, MAX);
	spinboxSpotExponent = new QDoubleSpinBox();
	spinboxSpotExponent->setRange(MIN, MAX);
	spinboxLightColorR = new QSpinBox();
	spinboxLightColorR->setRange(COLOR_MIN, COLOR_MAX);
	spinboxLightColorG = new QSpinBox();
	spinboxLightColorG->setRange(COLOR_MIN, COLOR_MAX);
	spinboxLightColorB = new QSpinBox();
	spinboxLightColorB->setRange(COLOR_MIN, COLOR_MAX);
	spinboxLightPositionX = new QDoubleSpinBox();
	spinboxLightPositionX->setRange(MIN, MAX);
	spinboxLightPositionY = new QDoubleSpinBox();
	spinboxLightPositionY->setRange(MIN, MAX);
	spinboxLightPositionZ = new QDoubleSpinBox();
	spinboxLightPositionZ->setRange(MIN, MAX);
	spinboxLightDirectionX = new QDoubleSpinBox();
	spinboxLightDirectionX->setRange(MIN, MAX);
	spinboxLightDirectionY = new QDoubleSpinBox();
	spinboxLightDirectionY->setRange(MIN, MAX);
	spinboxLightDirectionZ = new QDoubleSpinBox();
	spinboxLightDirectionZ->setRange(MIN, MAX);
	spinboxConeDirX = new QDoubleSpinBox();
	spinboxConeDirX->setRange(MIN, MAX);
	spinboxConeDirY = new QDoubleSpinBox();
	spinboxConeDirY->setRange(MIN, MAX);
	spinboxConeDirZ = new QDoubleSpinBox();
	spinboxConeDirZ->setRange(MIN, MAX);


	// ------------------------------------Layout--------------------------------------------
	QFont font("Microsoft YaHei", 10, 75);
	QFont subFont("Microsoft YaHei", 8, 75);

	QTreeWidget* treeWidget = this;
	QHeaderView *head = treeWidget->header();
	head->setSectionResizeMode(QHeaderView::ResizeToContents);
	treeWidget->setIndentation(8);
	treeWidget->setColumnCount(2);

	QTreeWidgetItem *title = new QTreeWidgetItem;
	title->setFont(0, font);
	title->setText(0, "Inspector");
	title->setText(1, "");
	treeWidget->setHeaderItem(title);
	
	// fog
	fog = new QTreeWidgetItem;
	fog->setText(0, "Fog");
	treeWidget->addTopLevelItem(fog);
	fog->setHidden(true);
	// use fog
	QTreeWidgetItem *usefog = new QTreeWidgetItem;
	usefog->setText(0, "enable");
	fog->addChild(usefog);
	treeWidget->setItemWidget(usefog, 1, checkboxFog);
	// fog type
	QTreeWidgetItem *fogtype = new QTreeWidgetItem;
	fogtype->setText(0, "type");
	fog->addChild(fogtype);
	treeWidget->setItemWidget(fogtype, 1, comboboxFogType);
	// fog color
	QTreeWidgetItem *fogcolor = new QTreeWidgetItem;
	fogcolor->setText(0, "color");
	fog->addChild(fogcolor);
		// fog color R
	QTreeWidgetItem *fogcolorR = new QTreeWidgetItem;
	fogcolorR->setText(0, "R");
	fogcolor->addChild(fogcolorR);
	treeWidget->setItemWidget(fogcolorR, 1, spinboxFogColorR);
		// fog color G
	QTreeWidgetItem *fogcolorG = new QTreeWidgetItem;
	fogcolorG->setText(0, "G");
	fogcolor->addChild(fogcolorG);
	treeWidget->setItemWidget(fogcolorG, 1, spinboxFogColorG);
		// fog color B
	QTreeWidgetItem *fogcolorB = new QTreeWidgetItem;
	fogcolorB->setText(0, "B");
	fogcolor->addChild(fogcolorB);
	treeWidget->setItemWidget(fogcolorB, 1, spinboxFogColorB);
	// fog start
	QTreeWidgetItem *fogstart = new QTreeWidgetItem;
	fogstart->setText(0, "start");
	fog->addChild(fogstart);
	treeWidget->setItemWidget(fogstart, 1, spinboxFogStart);
	// fog end
	QTreeWidgetItem *fogend = new QTreeWidgetItem;
	fogend->setText(0, "end");
	fog->addChild(fogend);
	treeWidget->setItemWidget(fogend, 1, spinboxFogEnd);

	//ambient
	ambient = new QTreeWidgetItem;
	ambient->setText(0, "AmbientLight");
	treeWidget->addTopLevelItem(ambient);
	ambient->setHidden(true);
	// ambient color
	QTreeWidgetItem *ambientcolor = new QTreeWidgetItem;
	ambientcolor->setText(0, "color");
	ambient->addChild(ambientcolor);
	// ambient color R
	QTreeWidgetItem *ambientcolorR = new QTreeWidgetItem;
	ambientcolorR->setText(0, "R");
	ambientcolor->addChild(ambientcolorR);
	treeWidget->setItemWidget(ambientcolorR, 1, spinboxAmbientColorR);
	// ambient color G
	QTreeWidgetItem *ambientcolorG = new QTreeWidgetItem;
	ambientcolorG->setText(0, "G");
	ambientcolor->addChild(ambientcolorG);
	treeWidget->setItemWidget(ambientcolorG, 1, spinboxAmbientColorG);
	// ambient color B
	QTreeWidgetItem *ambientcolorB = new QTreeWidgetItem;
	ambientcolorB->setText(0, "B");
	ambientcolor->addChild(ambientcolorB);
	treeWidget->setItemWidget(ambientcolorB, 1, spinboxAmbientColorB);

	// camera
	camera = new QTreeWidgetItem;
	camera->setText(0, "Camera");
	treeWidget->addTopLevelItem(camera);
	camera->setHidden(true);
	// camera name
	QTreeWidgetItem *cameraname = new QTreeWidgetItem;
	cameraname->setText(0, "name");
	camera->addChild(cameraname);
	treeWidget->setItemWidget(cameraname, 1, editCameraName);
	// camera near
	QTreeWidgetItem *nearz = new QTreeWidgetItem;
	nearz->setText(0, "near z");
	camera->addChild(nearz);
	treeWidget->setItemWidget(nearz, 1, spinboxCameraNear);
	// camera far
	QTreeWidgetItem *farz = new QTreeWidgetItem;
	farz->setText(0, "far z");
	camera->addChild(farz);
	treeWidget->setItemWidget(farz, 1, spinboxCameraFar);

	// culling mask
	QTreeWidgetItem *cullingMask = new QTreeWidgetItem;
	cullingMask->setText(0, "culling mask");
	camera->addChild(cullingMask);
	treeWidget->setItemWidget(cullingMask, 1, comboboxCullingMask);

	// camera transform
	QTreeWidgetItem *camTrans = new QTreeWidgetItem;
	camTrans->setText(0, "transform");
	camera->addChild(camTrans);
	treeWidget->setItemWidget(camTrans, 1, pushbuttonCameraTransSync);
	// camera position
	QTreeWidgetItem *camPos = new QTreeWidgetItem;
	camPos->setText(0, "position");
	camTrans->addChild(camPos);
	// position X
	QTreeWidgetItem *camPosX = new QTreeWidgetItem;
	camPosX->setText(0, "X");
	camPos->addChild(camPosX);
	treeWidget->setItemWidget(camPosX, 1, spinboxCameraPositionX);
	// position Y
	QTreeWidgetItem *camPosY = new QTreeWidgetItem;
	camPosY->setText(0, "Y");
	camPos->addChild(camPosY);
	treeWidget->setItemWidget(camPosY, 1, spinboxCameraPositionY);
	// position Z
	QTreeWidgetItem *camPosZ = new QTreeWidgetItem;
	camPosZ->setText(0, "Z");
	camPos->addChild(camPosZ);
	treeWidget->setItemWidget(camPosZ, 1, spinboxCameraPositionZ);

	// camera rotation
	QTreeWidgetItem *camRot = new QTreeWidgetItem;
	camRot->setText(0, "rotation");
	camTrans->addChild(camRot);
	// rotation X
	QTreeWidgetItem *camRotX = new QTreeWidgetItem;
	camRotX->setText(0, "X");
	camRot->addChild(camRotX);
	treeWidget->setItemWidget(camRotX, 1, spinboxCameraRotationX);
	// rotation Y
	QTreeWidgetItem *camRotY = new QTreeWidgetItem;
	camRotY->setText(0, "Y");
	camRot->addChild(camRotY);
	treeWidget->setItemWidget(camRotY, 1, spinboxCameraRotationY);
	// rotation Z
	QTreeWidgetItem *camRotZ = new QTreeWidgetItem;
	camRotZ->setText(0, "Z");
	camRot->addChild(camRotZ);
	treeWidget->setItemWidget(camRotZ, 1, spinboxCameraRotationZ);

	// gameobject
	gameobject = new QTreeWidgetItem;
	gameobject->setText(0, "GameObject");
	treeWidget->addTopLevelItem(gameobject);
	treeWidget->setItemWidget(gameobject, 1, checkboxActivity);
	gameobject->setHidden(true);
	// gameobject name
	QTreeWidgetItem *goname = new QTreeWidgetItem;
	goname->setText(0, "name");
	gameobject->addChild(goname);
	treeWidget->setItemWidget(goname, 1, editGameObjectName);

	// static
	QTreeWidgetItem *isStatic = new QTreeWidgetItem;
	isStatic->setText(0, "static");
	gameobject->addChild(isStatic);
	treeWidget->setItemWidget(isStatic, 1, checkboxStatic);

	// castshadow
	QTreeWidgetItem *castshadow = new QTreeWidgetItem;
	castshadow->setText(0, "cast shadow");
	gameobject->addChild(castshadow);
	treeWidget->setItemWidget(castshadow, 1, checkboxCastShadow);

	// layer
	QTreeWidgetItem *layer = new QTreeWidgetItem;
	layer->setText(0, "layer");
	gameobject->addChild(layer);
	treeWidget->setItemWidget(layer, 1, comboboxLayer);

	// gameobject transform
	QTreeWidgetItem *transform = new QTreeWidgetItem;
	transform->setText(0, "transform");
	gameobject->addChild(transform);
	// gameobject position
	QTreeWidgetItem *position = new QTreeWidgetItem;
	position->setText(0, "position");
	transform->addChild(position);
		// position X
	QTreeWidgetItem *positionX = new QTreeWidgetItem;
	positionX->setText(0, "X");
	position->addChild(positionX);
	treeWidget->setItemWidget(positionX, 1, spinboxPositionX);
		// position Y
	QTreeWidgetItem *positionY = new QTreeWidgetItem;
	positionY->setText(0, "Y");
	position->addChild(positionY);
	treeWidget->setItemWidget(positionY, 1, spinboxPositionY);
		// position Z
	QTreeWidgetItem *positionZ = new QTreeWidgetItem;
	positionZ->setText(0, "Z");
	position->addChild(positionZ);
	treeWidget->setItemWidget(positionZ, 1, spinboxPositionZ);

	// gameobject rotation
	QTreeWidgetItem *rotation = new QTreeWidgetItem;
	rotation->setText(0, "rotation");
	transform->addChild(rotation);
		// rotation X
	QTreeWidgetItem *rotationX = new QTreeWidgetItem;
	rotationX->setText(0, "X");
	rotation->addChild(rotationX);
	treeWidget->setItemWidget(rotationX, 1, spinboxRotationX);
		// rotation Y
	QTreeWidgetItem *rotationY = new QTreeWidgetItem;
	rotationY->setText(0, "Y");
	rotation->addChild(rotationY);
	treeWidget->setItemWidget(rotationY, 1, spinboxRotationY);
		// rotation Z
	QTreeWidgetItem *rotationZ = new QTreeWidgetItem;
	rotationZ->setText(0, "Z");
	rotation->addChild(rotationZ);
	treeWidget->setItemWidget(rotationZ, 1, spinboxRotationZ);

	// gameobject scale
	QTreeWidgetItem *scale = new QTreeWidgetItem;
	scale->setText(0, "scale");
	transform->addChild(scale);
		// scale X
	QTreeWidgetItem *scaleX = new QTreeWidgetItem;
	scaleX->setText(0, "X");
	scale->addChild(scaleX);
	treeWidget->setItemWidget(scaleX, 1, spinboxScaleX);
		// scale Y
	QTreeWidgetItem *scaleY = new QTreeWidgetItem;
	scaleY->setText(0, "Y");
	scale->addChild(scaleY);
	treeWidget->setItemWidget(scaleY, 1, spinboxScaleY);
		// scale Z
	QTreeWidgetItem *scaleZ = new QTreeWidgetItem;
	scaleZ->setText(0, "Z");
	scale->addChild(scaleZ);
	treeWidget->setItemWidget(scaleZ, 1, spinboxScaleZ);

	
	// light
	light = new QTreeWidgetItem;
	light->setText(0, "Light");
	treeWidget->addTopLevelItem(light);
	light->setHidden(true);
	// light name
	QTreeWidgetItem *lightname = new QTreeWidgetItem;
	lightname->setText(0, "name");
	light->addChild(lightname);
	treeWidget->setItemWidget(lightname, 1, editLightName);
	// use light
	QTreeWidgetItem *uselight = new QTreeWidgetItem;
	uselight->setText(0, "enable");
	light->addChild(uselight);
	treeWidget->setItemWidget(uselight, 1, checkboxLight);
	// light type
	QTreeWidgetItem *lighttype = new QTreeWidgetItem;
	lighttype->setText(0, "type");
	light->addChild(lighttype);
	treeWidget->setItemWidget(lighttype, 1, comboboxLightType);
	// shininess
	QTreeWidgetItem *shininess = new QTreeWidgetItem;
	shininess->setText(0, "shininess");
	light->addChild(shininess);
	treeWidget->setItemWidget(shininess, 1, spinboxShininess);
	// strength
	QTreeWidgetItem *strength = new QTreeWidgetItem;
	strength->setText(0, "strength");
	light->addChild(strength);
	treeWidget->setItemWidget(strength, 1, spinboxStrength);
	// constantAttenuation
	QTreeWidgetItem *constantAttenuation = new QTreeWidgetItem;
	constantAttenuation->setText(0, "constantAttenuation");
	light->addChild(constantAttenuation);
	treeWidget->setItemWidget(constantAttenuation, 1, spinboxConstantAttenuation);
	// linearAttenuation
	QTreeWidgetItem *linearAttenuation = new QTreeWidgetItem;
	linearAttenuation->setText(0, "linearAttenuation");
	light->addChild(linearAttenuation);
	treeWidget->setItemWidget(linearAttenuation, 1, spinboxLinearAttenuation);
	// quadraticAttenuation
	QTreeWidgetItem *quadraticAttenuation = new QTreeWidgetItem;
	quadraticAttenuation->setText(0, "quadraticAttenuation");
	light->addChild(quadraticAttenuation);
	treeWidget->setItemWidget(quadraticAttenuation, 1, spinboxQuadraticAttenuation);
	// spotCosCutoff
	QTreeWidgetItem *spotCosCutoff = new QTreeWidgetItem;
	spotCosCutoff->setText(0, "spotCosCutoff");
	light->addChild(spotCosCutoff);
	treeWidget->setItemWidget(spotCosCutoff, 1, spinboxSpotCosCutoff);
	// spotExponent
	QTreeWidgetItem *spotExponent = new QTreeWidgetItem;
	spotExponent->setText(0, "spotExponent");
	light->addChild(spotExponent);
	treeWidget->setItemWidget(spotExponent, 1, spinboxSpotExponent);
	// light color
	QTreeWidgetItem *lightcolor = new QTreeWidgetItem;
	lightcolor->setText(0, "color");
	light->addChild(lightcolor);
	// light color R
	QTreeWidgetItem *lightcolorR = new QTreeWidgetItem;
	lightcolorR->setText(0, "R");
	lightcolor->addChild(lightcolorR);
	treeWidget->setItemWidget(lightcolorR, 1, spinboxLightColorR);
	// light color G
	QTreeWidgetItem *lightcolorG = new QTreeWidgetItem;
	lightcolorG->setText(0, "G");
	lightcolor->addChild(lightcolorG);
	treeWidget->setItemWidget(lightcolorG, 1, spinboxLightColorG);
	// light color B
	QTreeWidgetItem *lightcolorB = new QTreeWidgetItem;
	lightcolorB->setText(0, "B");
	lightcolor->addChild(lightcolorB);
	treeWidget->setItemWidget(lightcolorB, 1, spinboxLightColorB);
	// light position
	QTreeWidgetItem *lightposition = new QTreeWidgetItem;
	lightposition->setText(0, "position");
	light->addChild(lightposition);
	// position X
	QTreeWidgetItem *lightpositionX = new QTreeWidgetItem;
	lightpositionX->setText(0, "X");
	lightposition->addChild(lightpositionX);
	treeWidget->setItemWidget(lightpositionX, 1, spinboxLightPositionX);
	// position Y
	QTreeWidgetItem *lightpositionY = new QTreeWidgetItem;
	lightpositionY->setText(0, "Y");
	lightposition->addChild(lightpositionY);
	treeWidget->setItemWidget(lightpositionY, 1, spinboxLightPositionY);
	// position Z
	QTreeWidgetItem *lightpositionZ = new QTreeWidgetItem;
	lightpositionZ->setText(0, "Z");
	lightposition->addChild(lightpositionZ);
	treeWidget->setItemWidget(lightpositionZ, 1, spinboxLightPositionZ);
	// light direction
	QTreeWidgetItem *direction = new QTreeWidgetItem;
	direction->setText(0, "direction");
	light->addChild(direction);
	// direction X
	QTreeWidgetItem *directionX = new QTreeWidgetItem;
	directionX->setText(0, "X");
	direction->addChild(directionX);
	treeWidget->setItemWidget(directionX, 1, spinboxLightDirectionX);
	// direction Y
	QTreeWidgetItem *directionY = new QTreeWidgetItem;
	directionY->setText(0, "Y");
	direction->addChild(directionY);
	treeWidget->setItemWidget(directionY, 1, spinboxLightDirectionY);
	// direction Z
	QTreeWidgetItem *directionZ = new QTreeWidgetItem;
	directionZ->setText(0, "Z");
	direction->addChild(directionZ);
	treeWidget->setItemWidget(directionZ, 1, spinboxLightDirectionZ);
	// light ConeDirection
	QTreeWidgetItem *conDir = new QTreeWidgetItem;
	conDir->setText(0, "ConeDirection");
	light->addChild(conDir);
	// conDir X
	QTreeWidgetItem *conDirX = new QTreeWidgetItem;
	conDirX->setText(0, "X");
	conDir->addChild(conDirX);
	treeWidget->setItemWidget(conDirX, 1, spinboxConeDirX);
	// conDir Y
	QTreeWidgetItem *conDirY = new QTreeWidgetItem;
	conDirY->setText(0, "Y");
	conDir->addChild(conDirY);
	treeWidget->setItemWidget(conDirY, 1, spinboxConeDirY);
	// conDir Z
	QTreeWidgetItem *conDirZ = new QTreeWidgetItem;
	conDirZ->setText(0, "Z");
	conDir->addChild(conDirZ);
	treeWidget->setItemWidget(conDirZ, 1, spinboxConeDirZ);

	// ------------------------------------QFormLayout-------------------------------------
	//QFormLayout* inspectorLayout = new QFormLayout();
	//QFont font("Microsoft YaHei", 10, 75);
	//QFont subFont("Microsoft YaHei", 8, 75);

	//// fog
	//QLabel* fog = new QLabel(tr("Fog"));
	//fog->setFont(font);
	//inspectorLayout->addRow(fog);
	//inspectorLayout->addRow(checkboxFog);
	//inspectorLayout->addRow(QStringLiteral("fog type"), comboboxFogType);

	//inspectorLayout->addRow(QStringLiteral("Color R"), spinboxFogColorR);
	//inspectorLayout->addRow(QStringLiteral("Color G"), spinboxFogColorG);
	//inspectorLayout->addRow(QStringLiteral("Color B"), spinboxFogColorB);

	//inspectorLayout->addRow(QStringLiteral("Color Start"), spinboxFogStart);
	//inspectorLayout->addRow(QStringLiteral("Color End"), spinboxFogEnd);

	//inspectorLayout->addRow(new QLabel(tr("-----------------------------")));

	//// gameobject
	//QLabel* gameobject = new QLabel(tr("GameObject"));
	//gameobject->setFont(font);
	//inspectorLayout->addRow(gameobject);
	//inspectorLayout->addRow(editGameObjectName);

	////inspectorLayout->addRow(new QLabel(tr("-----------------------------")));

	//QLabel* transform = new QLabel(tr("Transform"));
	//transform->setFont(subFont);
	//inspectorLayout->addRow(transform);
	//inspectorLayout->addRow(QStringLiteral("Position X"), spinboxPositionX);
	//inspectorLayout->addRow(QStringLiteral("Position Y"), spinboxPositionY);
	//inspectorLayout->addRow(QStringLiteral("Position Z"), spinboxPositionZ);
	//inspectorLayout->addRow(new QLabel(tr("")));
	//inspectorLayout->addRow(QStringLiteral("Rotation X"), spinboxRotationX);
	//inspectorLayout->addRow(QStringLiteral("Rotation Y"), spinboxRotationY);
	//inspectorLayout->addRow(QStringLiteral("Rotation Z"), spinboxRotationZ);
	//inspectorLayout->addRow(new QLabel(tr("")));
	//inspectorLayout->addRow(QStringLiteral("Scale X"), spinboxScaleX);
	//inspectorLayout->addRow(QStringLiteral("Scale Y"), spinboxScaleY);
	//inspectorLayout->addRow(QStringLiteral("Scale Z"), spinboxScaleZ);
	//inspectorLayout->addRow(new QLabel(tr("-----------------------------")));

	//setLayout(inspectorLayout);


	// fog
	connect(checkboxFog, SIGNAL(toggled(bool)), this, SLOT(FogToggled(bool)));
	connect(comboboxFogType, SIGNAL(activated(int)), this, SLOT(FogTypeActivated(int)));

	connect(spinboxFogColorR, SIGNAL(valueChanged(int)), this, SLOT(FogColorRChanged(int)));
	connect(spinboxFogColorG, SIGNAL(valueChanged(int)), this, SLOT(FogColorGChanged(int)));
	connect(spinboxFogColorB, SIGNAL(valueChanged(int)), this, SLOT(FogColorBChanged(int)));

	connect(spinboxFogStart, SIGNAL(valueChanged(double)), this, SLOT(FogStartChanged(double)));
	connect(spinboxFogEnd, SIGNAL(valueChanged(double)), this, SLOT(FogEndChanged(double)));

	// ambient
	connect(spinboxAmbientColorR, SIGNAL(valueChanged(int)), this, SLOT(AmbientColorRChanged(int)));
	connect(spinboxAmbientColorG, SIGNAL(valueChanged(int)), this, SLOT(AmbientColorGChanged(int)));
	connect(spinboxAmbientColorB, SIGNAL(valueChanged(int)), this, SLOT(AmbientColorBChanged(int)));

	// camera
	connect(editCameraName, &QLineEdit::textChanged, this, &HXInspectorWidget::CameraNameChanged);

	connect(spinboxCameraNear, SIGNAL(valueChanged(double)), this, SLOT(CameraNearChanged(double)));
	connect(spinboxCameraFar, SIGNAL(valueChanged(double)), this, SLOT(CameraFarChanged(double)));
	connect(comboboxCullingMask, SIGNAL(activated(int)), this, SLOT(CullingMaskActivated(int)));

	connect(pushbuttonCameraTransSync, SIGNAL(clicked()), this, SLOT(TransSyncOnClick()));

	connect(spinboxCameraPositionX, SIGNAL(valueChanged(double)), this, SLOT(CameraPositionXValueChanged(double)));
	connect(spinboxCameraPositionY, SIGNAL(valueChanged(double)), this, SLOT(CameraPositionYValueChanged(double)));
	connect(spinboxCameraPositionZ, SIGNAL(valueChanged(double)), this, SLOT(CameraPositionZValueChanged(double)));

	connect(spinboxCameraRotationX, SIGNAL(valueChanged(double)), this, SLOT(CameraRotationXValueChanged(double)));
	connect(spinboxCameraRotationY, SIGNAL(valueChanged(double)), this, SLOT(CameraRotationYValueChanged(double)));
	connect(spinboxCameraRotationZ, SIGNAL(valueChanged(double)), this, SLOT(CameraRotationZValueChanged(double)));

	// gameobject
	connect(checkboxActivity, SIGNAL(toggled(bool)), this, SLOT(ActivityToggled(bool)));

	connect(checkboxStatic, SIGNAL(toggled(bool)), this, SLOT(StaticToggled(bool)));

	connect(editGameObjectName, &QLineEdit::textChanged, this, &HXInspectorWidget::GameObjectNameChanged);

	connect(checkboxCastShadow, SIGNAL(toggled(bool)), this, SLOT(CastShadowToggled(bool)));

	connect(comboboxLayer, SIGNAL(activated(int)), this, SLOT(LayerActivated(int)));

	connect(spinboxPositionX, SIGNAL(valueChanged(double)), this, SLOT(PositionXValueChanged(double)));
	connect(spinboxPositionY, SIGNAL(valueChanged(double)), this, SLOT(PositionYValueChanged(double)));
	connect(spinboxPositionZ, SIGNAL(valueChanged(double)), this, SLOT(PositionZValueChanged(double)));

	connect(spinboxRotationX, SIGNAL(valueChanged(double)), this, SLOT(RotationXValueChanged(double)));
	connect(spinboxRotationY, SIGNAL(valueChanged(double)), this, SLOT(RotationYValueChanged(double)));
	connect(spinboxRotationZ, SIGNAL(valueChanged(double)), this, SLOT(RotationZValueChanged(double)));

	connect(spinboxScaleX, SIGNAL(valueChanged(double)), this, SLOT(ScaleXValueChanged(double)));
	connect(spinboxScaleY, SIGNAL(valueChanged(double)), this, SLOT(ScaleYValueChanged(double)));
	connect(spinboxScaleZ, SIGNAL(valueChanged(double)), this, SLOT(ScaleZValueChanged(double)));

	// light
	connect(editLightName, &QLineEdit::textChanged, this, &HXInspectorWidget::LightNameChanged);
	connect(checkboxLight, SIGNAL(toggled(bool)), this, SLOT(LightToggled(bool)));
	connect(comboboxLightType, SIGNAL(activated(int)), this, SLOT(LightTypeActivated(int)));
	connect(spinboxShininess, SIGNAL(valueChanged(double)), this, SLOT(ShininessValueChanged(double)));
	connect(spinboxStrength, SIGNAL(valueChanged(double)), this, SLOT(StrengthValueChanged(double)));
	connect(spinboxConstantAttenuation, SIGNAL(valueChanged(double)), this, SLOT(ConstantAttenuationValueChanged(double)));
	connect(spinboxLinearAttenuation, SIGNAL(valueChanged(double)), this, SLOT(LinearAttenuationValueChanged(double)));
	connect(spinboxQuadraticAttenuation, SIGNAL(valueChanged(double)), this, SLOT(QuadraticAttenuationValueChanged(double)));
	connect(spinboxSpotCosCutoff, SIGNAL(valueChanged(double)), this, SLOT(SpotCosCutoffValueChanged(double)));
	connect(spinboxSpotExponent, SIGNAL(valueChanged(double)), this, SLOT(SpotExponentValueChanged(double)));
	connect(spinboxLightColorR, SIGNAL(valueChanged(int)), this, SLOT(LightColorRChanged(int)));
	connect(spinboxLightColorG, SIGNAL(valueChanged(int)), this, SLOT(LightColorGChanged(int)));
	connect(spinboxLightColorB, SIGNAL(valueChanged(int)), this, SLOT(LightColorBChanged(int)));
	connect(spinboxLightPositionX, SIGNAL(valueChanged(double)), this, SLOT(LightPositionXValueChanged(double)));
	connect(spinboxLightPositionY, SIGNAL(valueChanged(double)), this, SLOT(LightPositionYValueChanged(double)));
	connect(spinboxLightPositionZ, SIGNAL(valueChanged(double)), this, SLOT(LightPositionZValueChanged(double)));
	connect(spinboxLightDirectionX, SIGNAL(valueChanged(double)), this, SLOT(LightDirectionXValueChanged(double)));
	connect(spinboxLightDirectionY, SIGNAL(valueChanged(double)), this, SLOT(LightDirectionYValueChanged(double)));
	connect(spinboxLightDirectionZ, SIGNAL(valueChanged(double)), this, SLOT(LightDirectionZValueChanged(double)));
	connect(spinboxConeDirX, SIGNAL(valueChanged(double)), this, SLOT(ConeDirXValueChanged(double)));
	connect(spinboxConeDirY, SIGNAL(valueChanged(double)), this, SLOT(ConeDirYValueChanged(double)));
	connect(spinboxConeDirZ, SIGNAL(valueChanged(double)), this, SLOT(ConeDirZValueChanged(double)));
}

HXInspectorWidget::~HXInspectorWidget()
{

}

void HXInspectorWidget::SetGameObjectInfo(HXGameObject* pGameObject)
{
	selectedGameObject = NULL;
	if (pGameObject)
	{
		gameobject->setHidden(false);

		if (pGameObject->GetActivity())
		{
			checkboxActivity->setCheckState(Qt::Checked);
		}
		else
		{
			checkboxActivity->setCheckState(Qt::Unchecked);
		}

		if (pGameObject->GetStatic())
		{
			checkboxStatic->setCheckState(Qt::Checked);
		}
		else
		{
			checkboxStatic->setCheckState(Qt::Unchecked);
		}

		comboboxLayer->setCurrentIndex(pGameObject->GetLayer());
		
		editGameObjectName->setText(pGameObject->GetName().c_str());

		if (pGameObject->GetCastShadow())
		{
			checkboxCastShadow->setCheckState(Qt::Checked);
		}
		else
		{
			checkboxCastShadow->setCheckState(Qt::Unchecked);
		}

		HXITransform* pTransform = pGameObject->GetTransform();

		spinboxPositionX->setValue(pTransform->GetPosition().x);
		spinboxPositionY->setValue(pTransform->GetPosition().y);
		spinboxPositionZ->setValue(pTransform->GetPosition().z);

		spinboxRotationX->setValue(pTransform->GetRotation().x);
		spinboxRotationY->setValue(pTransform->GetRotation().y);
		spinboxRotationZ->setValue(pTransform->GetRotation().z);

		spinboxScaleX->setValue(pTransform->GetScale().x);
		spinboxScaleY->setValue(pTransform->GetScale().y);
		spinboxScaleZ->setValue(pTransform->GetScale().z);
	}
	else
	{
		gameobject->setHidden(true);
	}

	selectedGameObject = pGameObject;
}

void HXInspectorWidget::SetLightInfo(HXLight* pLight)
{
	selectedLight = pLight;
	if (selectedLight)
	{
		light->setHidden(false);
		editLightName->setText(selectedLight->name.c_str());
		if (selectedLight->enable)
		{
			checkboxLight->setCheckState(Qt::Checked);
		}
		else
		{
			checkboxLight->setCheckState(Qt::Unchecked);
		}
		comboboxLightType->setCurrentIndex(selectedLight->lightType);
		spinboxShininess->setValue(selectedLight->shininess);
		spinboxStrength->setValue(selectedLight->strength);
		spinboxConstantAttenuation->setValue(selectedLight->constantAttenuation);
		spinboxLinearAttenuation->setValue(selectedLight->LinearAttenuation);
		spinboxQuadraticAttenuation->setValue(selectedLight->QuadraticAttenuation);
		spinboxSpotCosCutoff->setValue(selectedLight->SpotCosCutoff);
		spinboxSpotExponent->setValue(selectedLight->SpotExponent);
		spinboxLightColorR->setValue(selectedLight->color.r);
		spinboxLightColorG->setValue(selectedLight->color.g);
		spinboxLightColorB->setValue(selectedLight->color.b);
		spinboxLightPositionX->setValue(selectedLight->position.x);
		spinboxLightPositionY->setValue(selectedLight->position.y);
		spinboxLightPositionZ->setValue(selectedLight->position.z);
		spinboxLightDirectionX->setValue(selectedLight->direct.x);
		spinboxLightDirectionY->setValue(selectedLight->direct.y);
		spinboxLightDirectionZ->setValue(selectedLight->direct.z);
		spinboxConeDirX->setValue(selectedLight->ConeDirection.x);
		spinboxConeDirY->setValue(selectedLight->ConeDirection.y);
		spinboxConeDirZ->setValue(selectedLight->ConeDirection.z);
	}
	else
	{
		light->setHidden(true);
	}
}

void HXInspectorWidget::SetFogInfo(HXFogBase* pFog)
{
	fogData = pFog;
	if (pFog)
	{
		fog->setHidden(false);
		////checkboxFog->setCheckable(pFog->useFog);
		////checkboxFog->toggled(pFog->useFog);
		//checkboxFog->setChecked(true);
		if (pFog->useFog)
		{
			checkboxFog->setCheckState(Qt::Checked);
		}
		else
		{
			checkboxFog->setCheckState(Qt::Unchecked);
		}

		spinboxFogColorR->setValue(pFog->fogColor.r);
		spinboxFogColorG->setValue(pFog->fogColor.g);
		spinboxFogColorB->setValue(pFog->fogColor.b);
		if (pFog->fogType == HXFogType::FOG_Linear)
		{
			comboboxFogType->setCurrentIndex(0);
			HXFogLinear* pFogLinear = (HXFogLinear*)pFog;
			spinboxFogStart->setValue(pFogLinear->fogStart);
			spinboxFogEnd->setValue(pFogLinear->fogEnd);
		}
	}
	else
	{
		fog->setHidden(true);
	}
}

void HXInspectorWidget::SetAmbientInfo(HXCOLOR* pAmbient)
{
	ambientData = pAmbient;
	if (ambientData)
	{
		ambient->setHidden(false);
		spinboxAmbientColorR->setValue(pAmbient->r);
		spinboxAmbientColorG->setValue(pAmbient->g);
		spinboxAmbientColorB->setValue(pAmbient->b);
	}
	else
	{
		ambient->setHidden(true);
	}
}

void HXInspectorWidget::SetCameraInfo(HXICamera* pCamera)
{
	selectedCamera = (HXGLCamera*)pCamera;
	if (selectedCamera)
	{
		camera->setHidden(false);

		editCameraName->setText(selectedCamera->name.c_str());

		spinboxCameraNear->setValue(selectedCamera->mNear);
		spinboxCameraFar->setValue(selectedCamera->mFar);

		comboboxCullingMask->setCurrentIndex(selectedCamera->cullingMask);

		spinboxCameraPositionX->setValue(selectedCamera->transform->mPostion.x);
		spinboxCameraPositionY->setValue(selectedCamera->transform->mPostion.y);
		spinboxCameraPositionZ->setValue(selectedCamera->transform->mPostion.z);

		spinboxCameraRotationX->setValue(selectedCamera->transform->mEulerDegree.x);
		spinboxCameraRotationY->setValue(selectedCamera->transform->mEulerDegree.y);
		spinboxCameraRotationZ->setValue(selectedCamera->transform->mEulerDegree.z);
	}
	else
	{
		camera->setHidden(true);
	}
}

void HXInspectorWidget::ActivityToggled(bool activity)
{
	if (selectedGameObject)
	{
		selectedGameObject->SetActivity(activity);
	}
}

void HXInspectorWidget::StaticToggled(bool bStatic)
{
	if (selectedGameObject)
	{
		if (selectedGameObject->GetChildren().size() > 0)
		{
			if (QMessageBox::Yes == QMessageBox::question(this,
				tr("Change Static Flags"),
				tr("Do you want to change the static flags for all the child objects as well ?"),
				QMessageBox::Yes | QMessageBox::No,
				QMessageBox::Yes))
			{
				selectedGameObject->SetStaticRecurve(bStatic);
			}
			else
			{
				selectedGameObject->SetStatic(bStatic);
			}
		}
		else
		{
			selectedGameObject->SetStatic(bStatic);
		}
	}
}

void HXInspectorWidget::GameObjectNameChanged(const QString& name)
{
	if (selectedGameObject)
	{
		selectedGameObject->SetName(name.toStdString());

		QTreeWidgetItem * pTWI = HXEditorWin::GetInstance()->m_pHierarchyWidget->currentItem();
		if (pTWI)
		{
			pTWI->setText(0, name);
		}

		//HXEditorWin::GetInstance()->m_pHierarchyWidget->currentItem()->setText(0, name);
	}
}

void HXInspectorWidget::CastShadowToggled(bool castShadow)
{
	if (selectedGameObject)
	{
		if (selectedGameObject->GetChildren().size() > 0)
		{
			if (QMessageBox::Yes == QMessageBox::question(this,
				tr("Change CastShadow Flags"),
				tr("Do you want to change the castshadow flags for all the child objects as well ?"),
				QMessageBox::Yes | QMessageBox::No,
				QMessageBox::Yes))
			{
				selectedGameObject->SetCastShadowRecurve(castShadow);
			}
			else
			{
				selectedGameObject->SetCastShadow(castShadow);
			}
		}
		else
		{
			selectedGameObject->SetCastShadow(castShadow);
		}
	}
}

void HXInspectorWidget::LayerActivated(int index)
{
	if (selectedGameObject)
	{
		if (selectedGameObject->GetChildren().size() > 0)
		{
			if (QMessageBox::Yes == QMessageBox::question(this,
				tr("Change Layer Flags"),
				tr("Do you want to change the layer flags for all the child objects as well ?"),
				QMessageBox::Yes | QMessageBox::No,
				QMessageBox::Yes))
			{
				selectedGameObject->SetLayerRecurve((ELayer)index);
			}
			else
			{
				selectedGameObject->SetLayer((ELayer)index);
			}
		}
		else
		{
			selectedGameObject->SetLayer((ELayer)index);
		}
	}
}

void HXInspectorWidget::PositionXValueChanged(double value)
{
	if (selectedGameObject)
	{
		if (!selectedGameObject->GetStatic())
		{
			HXVector3D pos = selectedGameObject->GetTransform()->GetPosition();
			selectedGameObject->GetTransform()->SetPosition(HXVector3D(value, pos.y, pos.z));
		}
		else
		{
			spinboxPositionX->setValue(selectedGameObject->GetTransform()->GetPosition().x);
		}
	}
}
void HXInspectorWidget::PositionYValueChanged(double value)
{
	if (selectedGameObject)
	{
		if (!selectedGameObject->GetStatic())
		{
			HXVector3D pos = selectedGameObject->GetTransform()->GetPosition();
			selectedGameObject->GetTransform()->SetPosition(HXVector3D(pos.x, value, pos.z));
		}
		else
		{
			spinboxPositionY->setValue(selectedGameObject->GetTransform()->GetPosition().y);
		}
	}
}
void HXInspectorWidget::PositionZValueChanged(double value)
{
	if (selectedGameObject)
	{
		if (!selectedGameObject->GetStatic())
		{
			HXVector3D pos = selectedGameObject->GetTransform()->GetPosition();
			selectedGameObject->GetTransform()->SetPosition(HXVector3D(pos.x, pos.y, value));
		}
		else
		{
			spinboxPositionZ->setValue(selectedGameObject->GetTransform()->GetPosition().z);
		}
	}
}

void HXInspectorWidget::RotationXValueChanged(double value)
{
	if (selectedGameObject)
	{
		if (!selectedGameObject->GetStatic())
		{
			HXVector3D rotate = selectedGameObject->GetTransform()->GetRotation();
			selectedGameObject->GetTransform()->SetRotation(HXVector3D(value, rotate.y, rotate.z));
		}
		else
		{
			spinboxRotationX->setValue(selectedGameObject->GetTransform()->GetRotation().x);
		}
	}
}
void HXInspectorWidget::RotationYValueChanged(double value)
{
	if (selectedGameObject)
	{
		if (!selectedGameObject->GetStatic())
		{
			HXVector3D rotate = selectedGameObject->GetTransform()->GetRotation();
			selectedGameObject->GetTransform()->SetRotation(HXVector3D(rotate.x, value, rotate.z));
		}
		else
		{
			spinboxRotationY->setValue(selectedGameObject->GetTransform()->GetRotation().y);
		}
	}
}
void HXInspectorWidget::RotationZValueChanged(double value)
{
	if (selectedGameObject)
	{
		if (!selectedGameObject->GetStatic())
		{
			HXVector3D rotate = selectedGameObject->GetTransform()->GetRotation();
			selectedGameObject->GetTransform()->SetRotation(HXVector3D(rotate.x, rotate.y, value));
		}
		else
		{
			spinboxRotationZ->setValue(selectedGameObject->GetTransform()->GetRotation().z);
		}
	}
}

void HXInspectorWidget::ScaleXValueChanged(double value)
{
	if (selectedGameObject)
	{
		if (!selectedGameObject->GetStatic())
		{
			HXVector3D scale = selectedGameObject->GetTransform()->GetScale();
			selectedGameObject->GetTransform()->SetScale(HXVector3D(value, scale.y, scale.z));
		}
		else
		{
			spinboxScaleX->setValue(selectedGameObject->GetTransform()->GetScale().x);
		}
	}
}
void HXInspectorWidget::ScaleYValueChanged(double value)
{
	if (selectedGameObject)
	{
		if (!selectedGameObject->GetStatic())
		{
			HXVector3D scale = selectedGameObject->GetTransform()->GetScale();
			selectedGameObject->GetTransform()->SetScale(HXVector3D(scale.x, value, scale.z));
		}
		else
		{
			spinboxScaleY->setValue(selectedGameObject->GetTransform()->GetScale().y);
		}
	}
}
void HXInspectorWidget::ScaleZValueChanged(double value)
{
	if (selectedGameObject)
	{
		if (!selectedGameObject->GetStatic())
		{
			HXVector3D scale = selectedGameObject->GetTransform()->GetScale();
			selectedGameObject->GetTransform()->SetScale(HXVector3D(scale.x, scale.y, value));
		}
		else
		{
			spinboxScaleZ->setValue(selectedGameObject->GetTransform()->GetScale().z);
		}
	}
}

void HXInspectorWidget::FogToggled(bool useFog)
{
	if (fogData)
	{
		fogData->useFog = useFog;
	}
}

void HXInspectorWidget::FogTypeActivated(int index)
{
	// TODO: 暂时只实现了线性雾
}

void HXInspectorWidget::FogColorRChanged(int value)
{
	if (fogData)
	{
		fogData->fogColor.r = value;
	}
}
void HXInspectorWidget::FogColorGChanged(int value)
{
	if (fogData)
	{
		fogData->fogColor.g = value;
	}
}
void HXInspectorWidget::FogColorBChanged(int value)
{
	if (fogData)
	{
		fogData->fogColor.b = value;
	}
}

void HXInspectorWidget::FogStartChanged(double value)
{
	if (fogData && fogData->fogType == HXFogType::FOG_Linear)
	{
		HXFogLinear* fogLinear = (HXFogLinear*)fogData;
		fogLinear->fogStart = value;
	}
}

void HXInspectorWidget::FogEndChanged(double value)
{
	if (fogData && fogData->fogType == HXFogType::FOG_Linear)
	{
		HXFogLinear* fogLinear = (HXFogLinear*)fogData;
		fogLinear->fogEnd = value;
	}
}

void HXInspectorWidget::AmbientColorRChanged(int value)
{
	if (ambientData)
	{
		ambientData->r = value;
	}
}
void HXInspectorWidget::AmbientColorGChanged(int value)
{
	if (ambientData)
	{
		ambientData->g = value;
	}
}
void HXInspectorWidget::AmbientColorBChanged(int value)
{
	if (ambientData)
	{
		ambientData->b = value;
	}
}

void HXInspectorWidget::CameraNameChanged(const QString& name)
{
	if (selectedCamera)
	{
		selectedCamera->name = name.toStdString();

		QTreeWidgetItem * pTWI = HXEditorWin::GetInstance()->m_pHierarchyWidget->currentItem();
		if (pTWI)
		{
			pTWI->setText(0, name);
		}

		//HXEditorWin::GetInstance()->m_pHierarchyWidget->currentItem()->setText(0, name);
	}
}

void HXInspectorWidget::CameraNearChanged(double value)
{
	if (selectedCamera)
	{
		selectedCamera->mNear = value;
		float gAspect = (float)HXRenderSystem::gCurScreenWidth / (float)HXRenderSystem::gCurScreenHeight;
		selectedCamera->UpdateProjectionMatrix(-1, 1, -gAspect, gAspect, selectedCamera->mNear, selectedCamera->mFar);
	}
}

void HXInspectorWidget::CameraFarChanged(double value)
{
	if (selectedCamera)
	{
		selectedCamera->mFar = value;
		float gAspect = (float)HXRenderSystem::gCurScreenWidth / (float)HXRenderSystem::gCurScreenHeight;
		selectedCamera->UpdateProjectionMatrix(-1, 1, -gAspect, gAspect, selectedCamera->mNear, selectedCamera->mFar);
	}
}

void HXInspectorWidget::CullingMaskActivated(int index)
{
	if (selectedCamera)
	{
		selectedCamera->cullingMask = (ECullingMask)index;
	}
}

void HXInspectorWidget::TransSyncOnClick()
{
	SetCameraInfo(HXSceneManager::GetInstance()->GetMainCamera());
}

void HXInspectorWidget::CameraPositionXValueChanged(double value)
{
	if (selectedCamera)
	{
		HXVector3D pos = selectedCamera->transform->GetPosition();
		selectedCamera->transform->SetPosition(HXVector3D(value, pos.y, pos.z));
	}
}
void HXInspectorWidget::CameraPositionYValueChanged(double value)
{
	if (selectedCamera)
	{
		HXVector3D pos = selectedCamera->transform->GetPosition();
		selectedCamera->transform->SetPosition(HXVector3D(pos.x, value, pos.z));
	}
}
void HXInspectorWidget::CameraPositionZValueChanged(double value)
{
	if (selectedCamera)
	{
		HXVector3D pos = selectedCamera->transform->GetPosition();
		selectedCamera->transform->SetPosition(HXVector3D(pos.x, pos.y, value));
	}
}

void HXInspectorWidget::CameraRotationXValueChanged(double value)
{
	if (selectedCamera)
	{
		HXVector3D rotate = selectedCamera->transform->GetRotation();
		selectedCamera->transform->SetRotation(HXVector3D(value, rotate.y, rotate.z));
	}
}
void HXInspectorWidget::CameraRotationYValueChanged(double value)
{
	if (selectedCamera)
	{
		HXVector3D rotate = selectedCamera->transform->GetRotation();
		selectedCamera->transform->SetRotation(HXVector3D(rotate.x, value, rotate.z));
	}
}
void HXInspectorWidget::CameraRotationZValueChanged(double value)
{
	if (selectedCamera)
	{
		HXVector3D rotate = selectedCamera->transform->GetRotation();
		selectedCamera->transform->SetRotation(HXVector3D(rotate.x, rotate.y, value));
	}
}


void HXInspectorWidget::LightNameChanged(const QString& name)
{
	if (selectedLight)
	{
		selectedLight->name = name.toStdString();

		QTreeWidgetItem * pTWI = HXEditorWin::GetInstance()->m_pHierarchyWidget->currentItem();
		if (pTWI)
		{
			pTWI->setText(0, name);
		}

		//HXEditorWin::GetInstance()->m_pHierarchyWidget->currentItem()->setText(0, name);
	}
}

void HXInspectorWidget::LightToggled(bool useLight)
{
	if (selectedLight)
	{
		selectedLight->enable = useLight;
	}
}

void HXInspectorWidget::LightTypeActivated(int index)
{
	if (selectedLight)
	{
		selectedLight->lightType = (LIGHT_TYPE)index;
	}
}

void HXInspectorWidget::ShininessValueChanged(double value)
{
	if (selectedLight)
	{
		selectedLight->shininess = value;
	}
}

void HXInspectorWidget::StrengthValueChanged(double value)
{
	if (selectedLight)
	{
		selectedLight->strength = value;
	}
}

void HXInspectorWidget::ConstantAttenuationValueChanged(double value)
{
	if (selectedLight)
	{
		selectedLight->constantAttenuation= value;
	}
}

void HXInspectorWidget::LinearAttenuationValueChanged(double value)
{
	if (selectedLight)
	{
		selectedLight->LinearAttenuation = value;
	}
}

void HXInspectorWidget::QuadraticAttenuationValueChanged(double value)
{
	if (selectedLight)
	{
		selectedLight->QuadraticAttenuation = value;
	}
}

void HXInspectorWidget::SpotCosCutoffValueChanged(double value)
{
	if (selectedLight)
	{
		selectedLight->SpotCosCutoff = value;
	}
}

void HXInspectorWidget::SpotExponentValueChanged(double value)
{
	if (selectedLight)
	{
		selectedLight->SpotExponent = value;
	}
}

void HXInspectorWidget::LightColorRChanged(int value)
{
	if (selectedLight)
	{
		selectedLight->color.r = value;
	}
}
void HXInspectorWidget::LightColorGChanged(int value)
{
	if (selectedLight)
	{
		selectedLight->color.g = value;
	}
}
void HXInspectorWidget::LightColorBChanged(int value)
{
	if (selectedLight)
	{
		selectedLight->color.b = value;
	}
}

void HXInspectorWidget::LightPositionXValueChanged(double value)
{
	if (selectedLight)
	{
		selectedLight->position.x = value;
	}
}
void HXInspectorWidget::LightPositionYValueChanged(double value)
{
	if (selectedLight)
	{
		selectedLight->position.y = value;
	}
}
void HXInspectorWidget::LightPositionZValueChanged(double value)
{
	if (selectedLight)
	{
		selectedLight->position.z = value;
	}
}

void HXInspectorWidget::LightDirectionXValueChanged(double value)
{
	if (selectedLight)
	{
		selectedLight->direct.x = value;
	}
}
void HXInspectorWidget::LightDirectionYValueChanged(double value)
{
	if (selectedLight)
	{
		selectedLight->direct.y = value;
	}
}
void HXInspectorWidget::LightDirectionZValueChanged(double value)
{
	if (selectedLight)
	{
		selectedLight->direct.z = value;
	}
}

void HXInspectorWidget::ConeDirXValueChanged(double value)
{
	if (selectedLight)
	{
		selectedLight->ConeDirection.x = value;
	}
}
void HXInspectorWidget::ConeDirYValueChanged(double value)
{
	if (selectedLight)
	{
		selectedLight->ConeDirection.y = value;
	}
}
void HXInspectorWidget::ConeDirZValueChanged(double value)
{
	if (selectedLight)
	{
		selectedLight->ConeDirection.z = value;
	}
}

void HXInspectorWidget::OnDeleteGameObject()
{
	selectedGameObject = NULL;
	gameobject->setHidden(true);
}

void HXInspectorWidget::OnDeleteLight()
{
	selectedLight = NULL;
	light->setHidden(true);
}