#pragma once
#include "oxygine/actor/Actor.h"
#include "magic.h"

namespace oxygine
{
    class ResMagicParticles;
    class ResMagicEmitter;
    DECLARE_SMART(MagicEmitter, spMagicEmitter);
    class MagicEmitter: public Actor
    {
    public:
        MagicEmitter();
        ~MagicEmitter();

        HM_EMITTER getHM() const {return _emitter;}
        const ResMagicEmitter* getResMagicEmitter() const {return _res;}

        void    setInterrupt(bool v);
        void    setLoopMode(int);
        void    setAutoDetach(bool);
        void    setMagicScale(float s);
        void    setEmitter(const ResMagicEmitter* res, bool makeDublicate = true);

        void    setRelative2Actor(Actor* a) { _relative2actor = a; }


    protected:
        bool updateEmitter(timeMS dt);
        void doUpdate(const UpdateState& us);
        void doRender(const RenderState& rs);

        Actor* _relative2actor;
        const ResMagicEmitter* _res;
        HM_EMITTER _emitter;
        bool _detach;
    };
}
