#include "oxygine-framework.h"
#include <functional>
#include "../../oxygine-magicparticles/src/MagicEmitter.h"
#include "../../oxygine-magicparticles/src/ResMagicParticles.h"

using namespace oxygine;

//it is our resources
//in real project you would have more than one Resources declarations.
//It is important on mobile devices with limited memory and you would load/unload them
Resources gameResources;


class MainActor : public Actor
{
public:
	spTextField _text;
	spMagicEmitter _emmiter;

	MainActor()
	{
		setSize(getStage()->getSize());

		addEventListener(TouchEvent::CLICK, CLOSURE(this, &MainActor::clicked));
	}

	spMagicEmitter runEmitter(int i, const Vector2 &pos)
	{
		ResMagicParticles *rs = gameResources.getT<ResMagicParticles>("effects");
		const ResMagicEmitter *mpem = rs->getEmitter(rand() % rs->getNumEmitters());

		spMagicEmitter em = new MagicEmitter;

		em->attachTo(getStage());
		em->setPosition(pos);
		em->setEmitter(mpem);
		em->setAutoDetach(true);

		return em;
	}

	void clicked(Event *event)
	{
		TouchEvent *te = safeCast<TouchEvent*>(event);

		runEmitter(rand() % 10, te->localPosition);
	}
};

//declare spMainActor as intrusive_ptr holder of MainActor
typedef oxygine::intrusive_ptr<MainActor> spMainActor;
//you could use DECLARE_SMART preprocessor definition it does the same:
//DECLARE_SMART(MainActor, spMainActor)

void example_preinit() {}

//called from main.cpp
void example_init()
{
	
	ResMagicParticles::init();

	//load xml file with resources definition
	gameResources.loadXML("res.xml");

	spSprite logo = new Sprite;
	logo->setResAnim(gameResources.getResAnim("logo2"));
	logo->setPosition(getStage()->getSize() - logo->getSize());
	logo->attachTo(getStage());

	spTextField text = new TextField;
	text->setText("click to create particles");
	text->setVAlign(TextStyle::VALIGN_MIDDLE);
	text->setHAlign(TextStyle::HALIGN_MIDDLE);
	text->setPosition(getStage()->getSize() / 2);
	text->attachTo(getStage());


	text = new TextField;
	text->setText("Magic Particles Demo. http://astralax.com/");
	text->setVAlign(TextStyle::VALIGN_BASELINE);
	text->setHAlign(TextStyle::HALIGN_LEFT);
	text->setX(3);
	text->setY(getStage()->getHeight() - 3);
	text->attachTo(getStage());

	text = new TextField;
	text->setText(Magic_GetVersion());
	text->setVAlign(TextStyle::VALIGN_BASELINE);
	text->setHAlign(TextStyle::HALIGN_MIDDLE);
	text->setX(getStage()->getWidth() / 2);
	text->setY(getStage()->getHeight() - 3);
	text->attachTo(getStage());

	
	//lets create our client code simple actor
	//prefix 'sp' here means it is intrusive Smart Pointer
	//it would be deleted automatically when you lost ref to it	
	spMainActor actor = new MainActor;
	//and add it to RootActor as child
	getStage()->addChild(actor);
}


//called each frame from main.cpp
void example_update()
{
}

//called each frame from main.cpp
void example_destroy()
{
    //free previously loaded resources
    gameResources.free();
}
