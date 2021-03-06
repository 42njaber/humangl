#include "HumanGL.hpp"
#include "UI/Button.class.hpp"
#include "UI.hpp"

void UpdateAnimationSpeedText( void )
{
    if (selectedObject == nullptr)
        return ;
    std::shared_ptr<UIElement> speedSlot =
		ui.elements[3]->getChild(0)->getChild(1)->getChild(4)->getChild(0);
    std::stringstream str;
    str << std::fixed << std::setprecision(1) << selectedObject->animationSpeed;
    speedSlot->texts[0] = UIText(str.str(), mft::vec2i(20, 6), 20.0f);
}

void IncreaseAnimationSpeed( void )
{
    if (selectedObject == nullptr)
        return ;
    if (selectedObject->animationSpeed < 2.0)
        selectedObject->animationSpeed += 0.1;
    UpdateAnimationSpeedText();
}

void DecreaseAnimationSpeed( void )
{
    if (selectedObject == nullptr)
        return ;
    if (selectedObject->animationSpeed > 0.11)
        selectedObject->animationSpeed -= 0.1;
    UpdateAnimationSpeedText();
}

void InitAnimationPannel( void )
{
    AssetManager& assetManager = AssetManager::getInstance();

    std::vector<std::shared_ptr<GLFont>> fonts =
        assetManager.getAssetsOfType<GLFont>();
#ifdef USING_EXTERNAL_LIBS
    std::shared_ptr<GLFont> font =
        assetManager.loadAsset<GLFont>("resources/fonts/arial.ttf");
    std::shared_ptr<GLFont> font12 =
        assetManager.loadAsset<GLFont>("resources/fonts/arial.ttf");
#else
    std::shared_ptr<GLFont> font =
        assetManager.loadAsset<GLFont>("resources/fonts/pt-sans-48.bff");
    std::shared_ptr<GLFont> font12 =
        assetManager.loadAsset<GLFont>("resources/fonts/pt-sans-48.bff");
#endif

    //  Bottom pannel

    UIElement bottomPannel(mft::vec2i(0, 0),
        assetManager.loadAsset<Texture>("resources/UI/defaultUI-dark.png", "UI"));
    bottomPannel.setSize(mft::vec2i(1280, 140));

    UIElement animationPannel(mft::vec2i(200, 0),
        assetManager.loadAsset<Texture>("resources/UI/defaultUI-dark.png", "UI"));
    animationPannel.setSize(mft::vec2i(500, 140));

    //    Design bars
    UIElement animationDetailsPannel(mft::vec2i(0, 0),
        assetManager.loadAsset<Texture>("resources/UI/defaultUI-dark.png", "UI"));
    animationDetailsPannel.setSize(mft::vec2i(312, 140));
    for (int slot = 0; slot < 3; slot++)
    {
        UIElement nameSlot1(mft::vec2i(0, slot * 35),
            assetManager.loadAsset<Texture>("resources/UI/defaultUI-clear.png", "UI"));
        nameSlot1.setSize(mft::vec2i(99, 34));
        if (slot == 2)
            nameSlot1.texts.push_back(UIText("Name", mft::vec2i(14, 10), 16.0f));
        else if (slot == 1)
            nameSlot1.texts.push_back(UIText("Type", mft::vec2i(14, 10), 16.0f));
        else if (slot == 0)
            nameSlot1.texts.push_back(UIText("Duration (ms)", mft::vec2i(14, 10), 16.0f));
        animationDetailsPannel.addChild(std::make_shared<UIElement>(nameSlot1));
        UIElement valueSlot1(mft::vec2i(100, slot * 35),
            assetManager.loadAsset<Texture>("resources/UI/defaultUI.png", "UI"));
        valueSlot1.setSize(mft::vec2i(100, 34));
        animationDetailsPannel.addChild(std::make_shared<UIElement>(valueSlot1));
        UIElement nameSlot2(mft::vec2i(201, slot * 35),
            assetManager.loadAsset<Texture>("resources/UI/defaultUI-clear.png", "UI"));
        nameSlot2.setSize(mft::vec2i(99, 34));
        if (slot == 2)
            nameSlot2.texts.push_back(UIText("Ticks per second", mft::vec2i(14, 10), 16.0f));
        else if (slot == 1)
            nameSlot2.texts.push_back(UIText("Time", mft::vec2i(14, 10), 16.0f));
        else if (slot == 0)
            nameSlot2.texts.push_back(UIText("Status", mft::vec2i(14, 10), 16.0f));
        animationDetailsPannel.addChild(std::make_shared<UIElement>(nameSlot2));
        UIElement valueSlot2(mft::vec2i(301, slot * 35),
            assetManager.loadAsset<Texture>("resources/UI/defaultUI.png", "UI"));
        valueSlot2.setSize(mft::vec2i(100, 34));
        animationDetailsPannel.addChild(std::make_shared<UIElement>(valueSlot2));
    }

    UIElement animationDetailsPannelLeftBorder(mft::vec2i(0, 0),
        assetManager.loadAsset<Texture>("resources/UI/defaultUI-clear.png", "UI"));
    animationDetailsPannelLeftBorder.setSize(mft::vec2i(6, 104));
    animationDetailsPannel.addChild(std::make_shared<UIElement>(animationDetailsPannelLeftBorder));
    UIElement animationDetailsPannelMiddleBorder(mft::vec2i(201, 0),
        assetManager.loadAsset<Texture>("resources/UI/defaultUI-clear.png", "UI"));
    animationDetailsPannelMiddleBorder.setSize(mft::vec2i(6, 104));
    animationDetailsPannel.addChild(std::make_shared<UIElement>(animationDetailsPannelMiddleBorder));
    UIElement animationDetailsPannelTopBorder(mft::vec2i(0, 105),
        assetManager.loadAsset<Texture>("resources/UI/defaultUI-clearer.png", "UI"));
    animationDetailsPannelTopBorder.setSize(mft::vec2i(401, 34));
    animationDetailsPannelTopBorder.texts.push_back(UIText("Animation details",
        mft::vec2i(160, 10), 16.0f));
    animationDetailsPannel.addChild(std::make_shared<UIElement>(animationDetailsPannelTopBorder));

    animationPannel.addChild(std::make_shared<UIElement>(animationDetailsPannel));

    //  Control pannel
    UIElement animationControlPannel(mft::vec2i(402, 0),
        assetManager.loadAsset<Texture>("resources/UI/defaultUI-dark.png", "UI"));
    animationControlPannel.setSize(mft::vec2i(310, 140));

    for (int slot = 0; slot < 3; slot++)
    {
        UIElement left(mft::vec2i(0, slot * 35),
            assetManager.loadAsset<Texture>("resources/UI/defaultUI-clear.png", "UI"));
        left.setSize(mft::vec2i(23, 34));
        animationControlPannel.addChild(std::make_shared<UIElement>(left));
        UIElement center(mft::vec2i(24, slot * 35),
            assetManager.loadAsset<Texture>("resources/UI/defaultUI-clear.png", "UI"));
        center.setSize(mft::vec2i(263, 34));
        if (slot == 2)
        {
            Button button(mft::vec2i(100, 2),
                assetManager.loadAsset<Texture>("resources/UI/defaultUI.png", "UI"),
                assetManager.loadAsset<Texture>("resources/UI/defaultUI-clearer.png", "UI"),
                assetManager.loadAsset<Texture>("resources/UI/defaultUI-clearer.png", "UI"));
            button.setText("Play");
            button.setFont(font12);
            button.setAllSizes(mft::vec2i(30, 30));
            std::function<void()> func = PlayAnimation;
            std::shared_ptr<ActionWrapper> action =
                std::shared_ptr<ActionWrapper>(new Action(func));
            button.onRelease = action;
            center.addChild(std::make_shared<Button>(button));

            button.setPos(mft::vec2i(148, 2));
            button.setText("Stop");
            button.setFont(font12);
            button.setAllSizes(mft::vec2i(30, 30));
            func = ResetObjectPose;
            action = std::shared_ptr<ActionWrapper>(new Action(func));
            button.onRelease = action;
            center.addChild(std::make_shared<Button>(button));
        }
        else if (slot == 1)
        {
            center.texts.push_back(UIText("Speed", mft::vec2i(50, 8), 20.0f));
            UIElement N(mft::vec2i(100, 3),
                assetManager.loadAsset<Texture>("resources/UI/defaultUI-A-Dark.png", "UI"));
            N.setSize(mft::vec2i(60, 28));
            std::stringstream str;
            str << std::fixed << std::setprecision(1) << 1.0f;
            N.texts.push_back(UIText(str.str(), mft::vec2i(20, 6), 20.0f));
            center.addChild(std::make_shared<UIElement>(N));

            Button button(mft::vec2i(170, 6),
                assetManager.loadAsset<Texture>("resources/UI/defaultUI.png", "UI"),
                assetManager.loadAsset<Texture>("resources/UI/defaultUI-clearer.png", "UI"),
                assetManager.loadAsset<Texture>("resources/UI/defaultUI-clearer.png", "UI"));
            button.setText("+");
            button.setFont(font12);
            button.setAllSizes(mft::vec2i(22, 22));
            std::function<void()> func = IncreaseAnimationSpeed;
            std::shared_ptr<ActionWrapper> action =
                std::shared_ptr<ActionWrapper>(new Action(func));
            button.onRelease = action;
            center.addChild(std::make_shared<Button>(button));

            button.setPos(mft::vec2i(198, 6));
            button.setText("-");
            button.setFont(font12);
            button.setAllSizes(mft::vec2i(22, 22));
            func = DecreaseAnimationSpeed;
            action = std::shared_ptr<ActionWrapper>(new Action(func));
            button.onRelease = action;
            center.addChild(std::make_shared<Button>(button));
        }
        animationControlPannel.addChild(std::make_shared<UIElement>(center));
        UIElement right(mft::vec2i(288, slot * 35),
            assetManager.loadAsset<Texture>("resources/UI/defaultUI-clear.png", "UI"));
        right.setSize(mft::vec2i(23, 34));
        animationControlPannel.addChild(std::make_shared<UIElement>(right));
    }
    UIElement animationControlPannelLeftBorder(mft::vec2i(0, 0),
        assetManager.loadAsset<Texture>("resources/UI/defaultUI-clear.png", "UI"));
    animationControlPannelLeftBorder.setSize(mft::vec2i(6, 104));
    animationControlPannel.addChild(std::make_shared<UIElement>(animationControlPannelLeftBorder));
    UIElement animationControlPannelTopBorder(mft::vec2i(0, 105),
        assetManager.loadAsset<Texture>("resources/UI/defaultUI-clearer.png", "UI"));
    animationControlPannelTopBorder.setSize(mft::vec2i(311, 34));
    animationControlPannelTopBorder.texts.push_back(UIText("Control",
        mft::vec2i(140, 10), 16.0f));
    UIElement animationControlPannelRightBorder(mft::vec2i(305, 0),
        assetManager.loadAsset<Texture>("resources/UI/defaultUI-clear.png", "UI"));
    animationControlPannelRightBorder.setSize(mft::vec2i(6, 104));
    animationControlPannel.addChild(std::make_shared<UIElement>(animationControlPannelRightBorder));

    animationControlPannel.addChild(std::make_shared<UIElement>(animationControlPannelTopBorder));

    animationPannel.addChild(std::make_shared<UIElement>(animationControlPannel));

    bottomPannel.addChild(std::make_shared<UIElement>(animationPannel));

    ui.registerElement(std::make_shared<UIElement>(bottomPannel));
}

void UpdateAnimationTimeText( void )
{
    if (selectedObject == nullptr)
        return ;
    std::shared_ptr<UIElement> animationDetailsPannel =
		ui.elements[3]->getChild(0)->getChild(0);
    std::stringstream str;
    str << std::fixed << std::setprecision(1) << selectedObject->getCurrentTime();
    animationDetailsPannel->getChild(7)->texts.clear();
    animationDetailsPannel->getChild(7)->texts.push_back(UIText(str.str(),
        mft::vec2i(26, 10), 16.0f));
}

void UpdateAnimationStatusText( void )
{
    std::shared_ptr<UIElement> animationDetailsPannel =
		ui.elements[3]->getChild(0)->getChild(0);
    std::string str;
    if (selectedObject == nullptr)
        str = "Stopped";
    else if (selectedObject->getAnimationState() == AnimationState::Playing)
        str = "Playing";
    else if (selectedObject->getAnimationState() == AnimationState::Paused)
        str = "Paused";
    else if (selectedObject->getAnimationState() == AnimationState::Stopped)
        str = "Stopped";
    animationDetailsPannel->getChild(3)->texts.clear();
    animationDetailsPannel->getChild(3)->texts.push_back(UIText(str,
        mft::vec2i(26, 10), 16.0f));
}

void UpdateAnimationPannel( void )
{
    UpdateAnimationList();
    UpdateAnimationSpeedText();
    //ResetObjectPose();

    if (selectedAnimation == nullptr)
        return ;
    std::stringstream str;
    str << std::fixed << std::setprecision(1);
    std::shared_ptr<UIElement> animationDetailsPannel =
		ui.elements[3]->getChild(0)->getChild(0);

    //  Name
    animationDetailsPannel->getChild(9)->texts.clear();
    animationDetailsPannel->getChild(9)->texts.push_back(UIText(
        selectedAnimation->getName(), mft::vec2i(26, 10), 16.0f));

    //  Type
    str.str("");
    if (selectedAnimation->getType() == Animation::AnimType::Skeletal)
        str << "Skeletal";
    else if (selectedAnimation->getType() == Animation::AnimType::Solid)
        str << "Solid";
    animationDetailsPannel->getChild(5)->texts.clear();
    animationDetailsPannel->getChild(5)->texts.push_back(UIText(str.str(),
        mft::vec2i(26, 10), 16.0f));

    //  Duration
    str.str("");
    str << selectedAnimation->getDuration();
    animationDetailsPannel->getChild(1)->texts.clear();
    animationDetailsPannel->getChild(1)->texts.push_back(UIText(str.str(),
        mft::vec2i(26, 10), 16.0f));

    //  Ticks per second
    str.str("");
    str << selectedAnimation->getTicksPerSecond();
    animationDetailsPannel->getChild(11)->texts.clear();
    animationDetailsPannel->getChild(11)->texts.push_back(UIText(
        selectedAnimation->getName(), mft::vec2i(26, 10), 16.0f));

    //  Time
    UpdateAnimationTimeText();

    //  Status
    UpdateAnimationStatusText();
}
