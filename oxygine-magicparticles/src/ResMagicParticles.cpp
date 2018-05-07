#include "ResMagicParticles.h"
#include "oxygine/core/file.h"
#include "oxygine/res/Resources.h"
#include "oxygine/res/CreateResourceContext.h"
#include "oxygine/utils/stringUtils.h"

namespace oxygine
{
    float _globalScale = 1.0f;
    ResMagicEmitter::ResMagicEmitter(const ResMagicParticles* res, const std::string& name, HM_EMITTER em) : _res(res), _em(em), _name(name)
    {

    }
    ResMagicEmitter::~ResMagicEmitter()
    {
        Magic_UnloadEmitter(_em);
    }

    void ResMagicParticles::init()
    {
        Resources::registerResourceType(ResMagicParticles::create, "magicparticles");
    }

    void ResMagicParticles::release()
    {

    }

    void ResMagicParticles::setGlobalScale(float scale)
    {
        _globalScale = scale;
    }

    Resource* ResMagicParticles::create(CreateResourceContext& context)
    {
        ResMagicParticles* rs = new ResMagicParticles();
        std::string file = context.walker.getNode().attribute("file").value();

        rs->setName(Resource::extractID(context.walker.getNode(), file, ""));
        char head[255];
        char tail[255];
        path::split(file.c_str(), head, tail);
        rs->init((context.walker.getCurrentFolder() + head).c_str(), tail);
        setNode(rs, context.walker.getNode());
        context.resources->add(rs);

        return rs;
    }

    ResMagicParticles::ResMagicParticles()
    {

    }

    ResMagicParticles::~ResMagicParticles()
    {
        for (emitters::iterator i = _emitters.begin(); i != _emitters.end(); ++i)
        {
            ResMagicEmitter* em = *i;
            delete em;
        }
    }

    void ResMagicParticles::init(const char* folder, const char* fileName)
    {
        char path[255];
        safe_sprintf(path, "%s/%s", folder, fileName);

        file::buffer bf;
        file::read(path, bf);
        HM_FILE file = Magic_OpenFileInMemory(reinterpret_cast<const char*>(bf.getData()));

        int numAtlasses = Magic_GetStaticAtlasCount(file);
        for (int i = 0; i < numAtlasses; ++i)
        {
            MAGIC_STATIC_ATLAS at;
            Magic_GetStaticAtlas(file, i, &at);
            safe_sprintf(path, "%s/%s", folder, at.file);
            _atlas.addAtlas(TF_R8G8B8A8, path, "", at.width, at.height);
        }

        MAGIC_FIND_DATA find;
        const char* name = Magic_FindFirst(file, &find, MAGIC_FOLDER | MAGIC_EMITTER);
        while (name)
        {
            if (find.animate)
            {
                HM_EMITTER hm = Magic_LoadEmitter(file, name);
                Magic_SetRandomMode(hm, true);
                Magic_SetScale(hm, _globalScale);
                ResMagicEmitter* r = new ResMagicEmitter(this, name, hm);
                _emitters.push_back(r);
            }
            else
            {
                //LoadFolder(file,name);
            }

            name = Magic_FindNext(file, &find);
        }

        sort(_emitters.begin(), _emitters.end(), comparePred);

        Magic_CloseFile(file);
    }

    void ResMagicParticles::_load(LoadResourcesContext* context)
    {
        _atlas.load(context);
    }

    void ResMagicParticles::_unload()
    {
        _atlas.unload();
    }

    bool ResMagicParticles::findPred(const ResMagicEmitter* em, const std::string& name)
    {
        return em->getName() < name;
    }

    bool ResMagicParticles::comparePred(const ResMagicEmitter* em1, const ResMagicEmitter* em2)
    {
        return em1->getName() < em2->getName();
    }

    const ResMagicEmitter* ResMagicParticles::getEmitterByName(const std::string& name, error_policy ep) const
    {
        emitters::const_iterator it = lower_bound(_emitters.begin(), _emitters.end(), name, findPred);
        if (it != _emitters.end())
        {
            const ResMagicEmitter* em = *it;
            if (em->getName() == name)
                return em;
        }

        handleErrorPolicy(ep, "can't find emitter %s", name.c_str());
        return 0;
    }

    const ResMagicEmitter* ResMagicParticles::getEmitter(int index) const
    {
        const ResMagicEmitter* em = _emitters[index];
        return em;
    }

    int ResMagicParticles::getNumEmitters() const
    {
        return (int)_emitters.size();
    }

    spNativeTexture ResMagicParticles::getTexture(int i) const
    {
        return _atlas.getAtlas(i).base;
    }
}