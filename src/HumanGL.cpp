#include "NotRealEngine.hpp"
#include "mft/mft.hpp"
#include "InitBobby.hpp"
#include "Bindings.hpp"
#include "HumanGL.hpp"
#include "UI.hpp"

using namespace notrealengine;

//	Set of essential global variables
mft::vec2i		screenSize;
RenderingMode	renderingMode;
bool 			renderBones;
SDLEvents		events;
bool			running;
bool			mustUpdateModelPannel = false;
Scene			scene;
uint32_t		timeSinceLastFrame;
uint32_t		timeOfLastFrame;
std::string		modelPath;
unsigned int	selectedBone;
uint32_t		fps;
uint32_t		lastFpsUpdate;
std::shared_ptr<GLObject> selectedObject;
std::shared_ptr<Mesh> selectedMesh;
std::shared_ptr<Animation> selectedAnimation;
std::vector<std::shared_ptr<Animation>> bobbyAnimations;
std::vector<std::shared_ptr<Animation>> bobbyPlusAnimations;
std::vector<std::shared_ptr<Animation>> skeletalAnimations;
Armature rootArmature;
UIManager ui;

//	Load/init all the wanted resources

void	InitResources(int ac, char **av)
{
	AssetManager& assetManager = AssetManager::getInstance();

	//	Objects

	std::shared_ptr<GLObject> bobby = InitBobby();
	std::shared_ptr<GLObject> bobbyPlus = InitBobbyPlus();
	std::shared_ptr<GLObject> roundy = InitRoundy();
	std::shared_ptr<GLObject> roundyPlus = InitRoundyPlus();
	bobbyPlus->visible = false;
	roundy->visible = false;
	roundyPlus->visible = false;

	std::shared_ptr<GLObject> obj = nullptr;

	//	Import animation given in the arguments
	std::vector<std::shared_ptr<Animation>> readAnimations;
	if (ac >= 2)
	{
		obj = assetManager.loadAsset<GLObject>(av[1]);
		for (int i = 1; i < ac; i++)
		{
			std::vector<std::shared_ptr<Animation>> anims = LoadAnimations(av[i]);
			readAnimations.insert(readAnimations.end(), anims.begin(), anims.end());
		}
	}

	//	Copy read animations into our global skeletal animations array
	skeletalAnimations.insert(skeletalAnimations.end(),
		readAnimations.begin(), readAnimations.end());
	selectedBone = 0;

	if (obj != nullptr)
	{
		//obj->transform.rotate(mft::quat::rotation(mft::vec3(0.0f, 1.0f, 0.0f), mft::radians(180.0f)));
		obj->visible = false;
		if (!skeletalAnimations.empty())
			obj->setAnimation(skeletalAnimations[0]);
	}


	std::shared_ptr<GLObject> rock =
		assetManager.loadAsset<GLObject>("resources/objects/Rock/rock.dae");
	if (rock != nullptr)
	{
		rock->transform.move(mft::vec3(5.0f, 0.0f, -5.0f));
		rock->visible = false;
	}

	//	Animations

	std::shared_ptr<Animation> bobbyWalking = InitBobbyWalking();
	std::shared_ptr<Animation> bobbyJumping = InitBobbyJumping();
	std::shared_ptr<Animation> bobbyIdle = InitBobbyIdle();
	std::shared_ptr<Animation> bobbyBackflip = InitBobbyBackflip();
	std::shared_ptr<Animation> bobbyDance = InitBobbyDance();
	std::shared_ptr<Animation> bobbyMuayThai = InitBobbyMuayThai();

	std::shared_ptr<Animation> bobbyPlusWalking = InitBobbyPlusWalking();
	std::shared_ptr<Animation> bobbyPlusJumping = InitBobbyPlusJumping();
	std::shared_ptr<Animation> bobbyPlusIdle = InitBobbyPlusIdle();
	std::shared_ptr<Animation> bobbyPlusBackflip = InitBobbyPlusBackflip();
	std::shared_ptr<Animation> bobbyPlusDance = InitBobbyPlusDance();
	std::shared_ptr<Animation> bobbyPlusMuayThai = InitBobbyPlusMuayThai();
	selectedAnimation = bobbyIdle;
	selectedObject = bobby;
	selectedMesh = bobby->getMeshes()[0];
	PopulateArmature();
	bobby->setAnimation(bobbyIdle);

	bobbyAnimations.push_back(bobbyIdle);
	bobbyAnimations.push_back(bobbyWalking);
	bobbyAnimations.push_back(bobbyJumping);
	bobbyAnimations.push_back(bobbyBackflip);
	bobbyAnimations.push_back(bobbyDance);
	bobbyAnimations.push_back(bobbyMuayThai);

	bobbyPlusAnimations.push_back(bobbyPlusIdle);
	bobbyPlusAnimations.push_back(bobbyPlusWalking);
	bobbyPlusAnimations.push_back(bobbyPlusJumping);
	bobbyPlusAnimations.push_back(bobbyPlusBackflip);
	bobbyPlusAnimations.push_back(bobbyPlusDance);
	bobbyPlusAnimations.push_back(bobbyPlusMuayThai);

	//	Fonts

	#ifdef USING_EXTERNAL_LIBS
		std::shared_ptr<GLFont>	font =
			assetManager.loadAsset<GLFont>("resources/fonts/arial.ttf");
	#else
		std::shared_ptr<GLFont>	font =
			assetManager.loadAsset<GLFont>("resources/fonts/Times-New-Roman-16.bff");
	#endif
}

void UpdateTimers(uint32_t& fpsCount)
{
	uint32_t newTime = SDL_GetTicks();
	timeSinceLastFrame = newTime - timeOfLastFrame;
	timeOfLastFrame = newTime;
	fpsCount++;
	if (newTime - lastFpsUpdate >= 1000)
	{
		lastFpsUpdate = newTime;
		fps = fpsCount;
		ui.elements[1]->texts[0].text = std::string("FPS: " + std::to_string(fps));
		fpsCount = 0;
	}
}

void RenderLoop(GLContext_SDL& context)
{
	//	Timers

	uint32_t	fpsCount = 0;

	timeSinceLastFrame = SDL_GetTicks();
	lastFpsUpdate = 0;
	timeOfLastFrame = 0;

	while (running)
	{
		UpdateTimers(fpsCount);

		SDL_GetMouseState(&events.mousePos.x, &events.mousePos.y);
		SDL_GetGlobalMouseState(&events.mouseGlobalPos.x, &events.mouseGlobalPos.y);
		//	Invert y-axis (SDL is Y- and OpenGL is Y+)
		events.mousePos.y = WINDOW_HEIGHT - events.mousePos.y;
		events.mouseGlobalPos.y = screenSize.y - events.mouseGlobalPos.y;

		if (mustUpdateModelPannel == true)
			UpdateModelPannel();
		if (selectedObject != nullptr
			&& selectedObject->getAnimationState() == AnimationState::Playing)
			UpdateAnimationTimeText();
		if (renderingMode != ThirdPersonBobby && renderingMode != ThirdPersonBobbyPlus
			&& renderingMode != ThirdPersonRoundy && renderingMode != ThirdPersonRoundyPlus)
			ui.update(events.mousePos, events.mouseState);
		if (events.handle() == NRE_QUIT)
			break ;

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scene.render();
		if (renderBones == true)
			scene.renderBones();

		if (renderingMode != ThirdPersonBobby && renderingMode != ThirdPersonBobbyPlus
			&& renderingMode != ThirdPersonRoundy && renderingMode != ThirdPersonRoundyPlus)
			ui.draw();

		context.swapWindow();
	}
}

void	Render(GLContext_SDL& context)
{
	AssetManager& assetManager = AssetManager::getInstance();

	//	Scene setup
	scene = Scene("Scene");

	scene.drawGrid = true;

	std::shared_ptr<GLObject> obj =
		assetManager.getAsset<GLObject>(modelPath);
	if (obj != nullptr)
		scene.addObject(obj);
	std::shared_ptr<GLObject> rock =
		assetManager.getAsset<GLObject>("resources/objects/Rock/rock.dae");
	if (rock != nullptr)
		scene.addObject(rock);
	scene.addObject(assetManager.getAssetByName<GLObject>("Bobby")); // Bobby
	scene.addObject(assetManager.getAssetByName<GLObject>("Bobby Plus")); // Bobby plus
	scene.addObject(assetManager.getAssetByName<GLObject>("Roundy")); // Roundy
	scene.addObject(assetManager.getAssetByName<GLObject>("Roundy Plus")); // Roundy plus

	//	Light

	std::shared_ptr<Light>	light1 = std::make_shared<Light>(LightType::Point);
	assetManager.addAsset(light1);
	light1->move(mft::vec3(0.0f, 3.0f, 5.0f));

	scene.addLight(light1);
	scene.setLightingMode(LightingMode::Unlit);

	//	Skybox

	std::string paths[] =
	{
		"resources/images/skybox/right.png",
		"resources/images/skybox/left.png",
		"resources/images/skybox/top.png",
		"resources/images/skybox/bottom.png",
		"resources/images/skybox/front.png",
		"resources/images/skybox/back.png"
	};
	std::shared_ptr<Skybox> skybox =
		assetManager.loadAsset<Skybox>("resources/images/skybox/right.png", paths);
	if (skybox != nullptr)
	{
		scene.setSkybox(skybox);
		scene.drawSkybox = true;
	}

	std::cout << "Asset manager content:" << std::endl;
	assetManager.printContent();

	RenderLoop(context);
}

/**	Release OpenGL pointers before releasing the OpenGL windows and context
*/
void ReleasePointers()
{
	AssetManager::getInstance().clear();
	scene.clear();
	selectedObject.reset();
	selectedMesh.reset();
	selectedAnimation.reset();
	bobbyAnimations.clear();
	bobbyPlusAnimations.clear();
	skeletalAnimations.clear();
	ui.elements.clear();
	ClearArmature(rootArmature);
}

int		LaunchHumanGL(int ac, char **av, SDLWindow& window, GLContext_SDL& context)
{
	if (ac < 2)
		modelPath = "";
	else
		modelPath = av[1];

	//	Write OpenGL version

	const char* glVersion = (char*)GLCallThrow(glGetString, GL_VERSION);
	std::cout << "GL Version: " << glVersion << std::endl;

	//	Enable some OpenGL params

	GLCallThrow(glEnable, GL_DEPTH_TEST);
	GLCallThrow(glEnable, GL_BLEND);
	GLCallThrow(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//	Global variables init

	renderingMode = Bobby;
	renderBones = false;
	events = SDLEvents();
	running = true;

	InitResources(ac, av);
	InitBindings();
	InitUI();

	Render(context);

	ReleasePointers();
	return 0;
}

int		HumanGL(int ac, char** av)
{

	SDLWindow window("HumanGL", std::pair<int, int>(WINDOW_WIDTH, WINDOW_HEIGHT));
	GLContext_SDL context(window.getContext(), window.getWindow());
	context.makeCurrent();

	SDL_DisplayMode dm;
	SDL_GetCurrentDisplayMode(0, &dm);
	screenSize = mft::vec2i(dm.w, dm.h);

	try
	{
		LaunchHumanGL(ac, av, window, context);
	}
	catch (notrealengine::GLException& e)
	{
		std::cerr << std::endl << "GL Exception: " << e.what() << std::endl;
		ReleasePointers();
		return -1;
	}
	catch (std::exception& e)
	{
		std::cerr << std::endl << "STD Exception: " << e.what() << std::endl;
		ReleasePointers();
		return -1;
	}
	return 0;
}
