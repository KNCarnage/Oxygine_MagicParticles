#pragma once
#include "oxygine/res/Resource.h"
#include "oxygine/res/ResAtlas.h"
#include "magic.h"

namespace oxygine
{
    class ResMagicParticles;
    class ResMagicEmitter
    {
    public:
        ResMagicEmitter(const ResMagicParticles* res, const std::string& name, HM_EMITTER em);
        ~ResMagicEmitter();

        const std::string&          getName() const { return _name; }
        HM_EMITTER                  getEmitter() const {return _em;}
        const ResMagicParticles*    getRes() const {return _res;}

    private:
        const ResMagicParticles* _res;
        HM_EMITTER _em;
        std::string _name;
    };

    class ResMagicParticles: public Resource
    {
    public:
        static void init();
        static void release();
        static Resource* create(CreateResourceContext& context);
        static void setGlobalScale(float scale);

        ResMagicParticles();
        ~ResMagicParticles();

        void init(const char* folder, const char* file);

        const ResMagicEmitter*  getEmitterByName(const std::string& name, error_policy ep = ep_show_error) const;
        const ResMagicEmitter*  getEmitter(int index) const;
        int                     getNumEmitters() const;
        spNativeTexture         getTexture(int i) const;

    protected:
        void _load(LoadResourcesContext* context);
        void _unload();

    private:
        static bool findPred(const ResMagicEmitter* em, const std::string& name);
        static bool comparePred(const ResMagicEmitter* em1, const ResMagicEmitter* em2);

        ResAtlas _atlas;

        typedef std::vector<ResMagicEmitter*> emitters;
        emitters _emitters;
    };
}