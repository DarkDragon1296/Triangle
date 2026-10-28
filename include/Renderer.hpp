#pragma once

#include "RenderableBatch.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "geometry.hpp"

namespace render {

class Renderer {
    using TrBatch    = RenderableBatch<gmt::Triangle,  GL_TRIANGLES>;
    using IntTrBatch = RenderableBatch<gmt::Triangle,  GL_TRIANGLES>;
    using IntSegBatch  = RenderableBatch<gmt::Segment3D, GL_LINES>;

    public:
    TrBatch triangles;
    IntTrBatch int_tr;
    IntSegBatch int_seg;
    Camera camera;
    Shader shader;

    Renderer(TrBatch &&triangles_, IntTrBatch &&int_tr_, IntSegBatch &&int_seg_,
             Camera camera_, Shader &&shader_); 

    void draw() const; 
    void clear() const;

    private:
    void configure_gl_state() const;
    void init_matrices() const;
    void apply_camera() const;
};

}
