#include "MagicEmitter.h"
#include "ResMagicParticles.h"
#include "oxygine/RenderState.h"
#include "oxygine/actor/Stage.h"
#include "oxygine/core/Renderer.h"
#include "oxygine/AnimationFrame.h"
#include "oxygine/core/NativeTexture.h"
#include "oxygine/STDRenderer.h"

#if OXYGINE_RENDERER > 2
#include "oxygine/Material.h"
#else
#endif

namespace oxygine
{
    MagicEmitter::MagicEmitter(): _emitter(0), _res(0), _detach(false), _relative2actor(0)
    {

    }

    MagicEmitter::~MagicEmitter()
    {
        setEmitter(0, 0);
    }

    void MagicEmitter::setEmitter(const ResMagicEmitter* res, bool makeDublicate)
    {
        _res = res;
        if (_emitter)
        {
            Magic_UnloadEmitter(_emitter);
            _emitter = 0;
        }

        if (res)
        {
            HM_EMITTER em = res->getEmitter();
            _emitter = makeDublicate ? Magic_DuplicateEmitter(em) : em;
            Magic_SetRandomMode(_emitter, true);//bug workaround (already set random)
        }
    }

    bool MagicEmitter::updateEmitter(timeMS dt)
    {
        Transform t = computeGlobalTransform(_relative2actor);
        MAGIC_POSITION pos;
        pos.x = t.x;
        pos.y = t.y;
        Magic_SetEmitterPosition(_emitter, &pos);

        bool res = Magic_Update(_emitter, dt);
        return res;
    }

    void    MagicEmitter::setMagicScale(float s)
    {
        Magic_SetScale(_emitter, s);
    }

    void MagicEmitter::setLoopMode(int mode)
    {
        OX_ASSERT(_emitter);
        Magic_SetLoopMode(_emitter, mode);
    }

    void MagicEmitter::setAutoDetach(bool d)
    {
        _detach = d;
    }

    void MagicEmitter::setInterrupt(bool v)
    {
        Magic_SetInterrupt(_emitter, v);
    }

    void MagicEmitter::doUpdate(const UpdateState& us)
    {
        if (!updateEmitter(us.dt))
        {
            if (_detach)
                detach();
        }
    }

    void MagicEmitter::doRender(const RenderState& rs)
    {
        //if (rs.renderer->isMasked())
        //  return;

#if OXYGINE_RENDERER > 2
        STDRenderer* renderer = STDRenderer::instance;
		Material::null->apply();
#else
        STDRenderer* renderer = safeCast<STDRenderer*>(rs.renderer);
#endif

        const ResMagicParticles* resmp = _res->getRes();
        if (_relative2actor)
        {
            Transform t;
            t.identity();
            if (_relative2actor)
            {
                t = computeGlobalTransform(_relative2actor);
            }



            MAGIC_RENDERING rendering;
            Magic_CreateFirstRenderedParticlesList(_emitter, &rendering);
            while (rendering.count)
            {
                if (rendering.intense)
					rsCache().setBlendMode(blend_add);
                else
					rsCache().setBlendMode(blend_premultiplied_alpha);
				rsCache().setTexture(0, resmp->getTexture(rendering.texture_id));

                Vector2 pos;
                for (int i = 0; i < rendering.count; i++)
                {
                    MAGIC_PARTICLE_VERTEXES vertexes;
                    Magic_GetNextParticleVertexes(&vertexes);

                    const char* cp = (const char*)&vertexes.color;
                    Color c(cp[2], cp[1], cp[0], cp[3]);
                    c = c.premultiplied();

                    unsigned int color = c.rgba();


                    vertexPCT2 vr[4];
                    vertexPCT2* v = vr;
                    v->z = 0;
                    v->u = vertexes.u1;
                    v->v = vertexes.v1;
                    v->color = color;

                    pos = Vector2(vertexes.vertex1.x, vertexes.vertex1.y);
                    pos = t.transform(pos);
                    v->x = pos.x;
                    v->y = pos.y;
                    ++v;

                    v->x = vertexes.vertex2.x;
                    v->y = vertexes.vertex2.y;
                    v->z = 0;
                    v->u = vertexes.u2;
                    v->v = vertexes.v2;
                    v->color = color;

                    pos = Vector2(vertexes.vertex2.x, vertexes.vertex2.y);
                    pos = t.transform(pos);
                    v->x = pos.x;
                    v->y = pos.y;
                    ++v;

                    v->x = vertexes.vertex4.x;
                    v->y = vertexes.vertex4.y;
                    v->z = 0;
                    v->u = vertexes.u4;
                    v->v = vertexes.v4;
                    v->color = color;
                    pos = Vector2(vertexes.vertex4.x, vertexes.vertex4.y);
                    pos = t.transform(pos);
                    v->x = pos.x;
                    v->y = pos.y;
                    ++v;

                    v->x = vertexes.vertex3.x;
                    v->y = vertexes.vertex3.y;
                    v->z = 0;
                    v->u = vertexes.u3;
                    v->v = vertexes.v3;
                    v->color = color;
                    pos = Vector2(vertexes.vertex3.x, vertexes.vertex3.y);
                    pos = t.transform(pos);
                    v->x = pos.x;
                    v->y = pos.y;
                    ++v;

                    renderer->addVertices(vr, sizeof(vr));
                }
				renderer->flush();
                Magic_CreateNextRenderedParticlesList(&rendering);
            }
        }
        else
        {
            MAGIC_RENDERING rendering;
            Magic_CreateFirstRenderedParticlesList(_emitter, &rendering);
            while (rendering.count)
            {
                if (rendering.intense)
					rsCache().setBlendMode(blend_add);
                else
					rsCache().setBlendMode(blend_premultiplied_alpha);
				rsCache().setTexture(0, resmp->getTexture(rendering.texture_id));

                for (int i = 0; i < rendering.count; i++)
                {
                    MAGIC_PARTICLE_VERTEXES vertexes;
                    Magic_GetNextParticleVertexes(&vertexes);

                    const char* cp = (const char*)&vertexes.color;
                    Color c(cp[2], cp[1], cp[0], cp[3]);
                    c = c.premultiplied();

                    unsigned int color = c.rgba();


                    vertexPCT2 vr[4];
                    vertexPCT2* v = vr;
                    v->x = vertexes.vertex1.x;
                    v->y = vertexes.vertex1.y;
                    v->z = 0;
                    v->u = vertexes.u1;
                    v->v = vertexes.v1;
                    v->color = color;
                    ++v;

                    v->x = vertexes.vertex2.x;
                    v->y = vertexes.vertex2.y;
                    v->z = 0;
                    v->u = vertexes.u2;
                    v->v = vertexes.v2;
                    v->color = color;
                    ++v;

                    v->x = vertexes.vertex4.x;
                    v->y = vertexes.vertex4.y;
                    v->z = 0;
                    v->u = vertexes.u4;
                    v->v = vertexes.v4;
                    v->color = color;
                    ++v;

                    v->x = vertexes.vertex3.x;
                    v->y = vertexes.vertex3.y;
                    v->z = 0;
                    v->u = vertexes.u3;
                    v->v = vertexes.v3;
                    v->color = color;
                    ++v;

                    renderer->addVertices(vr, sizeof(vr));
                }
				renderer->flush();
                Magic_CreateNextRenderedParticlesList(&rendering);
            }
        }
    }


    /*
    TweenMagicEmitter::TweenMagicEmitter(const ResMagicParticles *res, const char *name, bool looped):_res(), _emitter(0)
    {
        _res = res;
        _emitter = Magic_DuplicateEmitter(_res->getEmitterByName(name));
        if (looped)
            Magic_SetLoopMode(_emitter, MAGIC_LOOP);
    }



    TweenMagicEmitter::TweenMagicEmitter(const ResMagicParticles *res, HM_EMITTER em, bool makeDublicate):_res(), _emitter(0)
    {
        _res = res;
        _emitter = makeDublicate ? Magic_DuplicateEmitter(em) : em;
    }

    bool TweenMagicEmitter::start(Actor &actor)
    {
        _client = &actor;
        MagicEmitter *em = safeCast<MagicEmitter*>(&actor);
        //em->setEmitter(_res, _emitter, false);

        return false;
    }

    bool TweenMagicEmitter::update(Actor &actor, const UpdateState &us)
    {
        MagicEmitter *em = safeCast<MagicEmitter*>(&actor);
        bool res = !em->updateEmitter(us.dt);
        if (res)
        {
            _done = true;
            return itDone(actor, us);
        }

        return res;
        return false;
    }

    */
}