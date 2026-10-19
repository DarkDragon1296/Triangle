#pragma once

#include "RenderableBatch.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "geometry.hpp"

namespace render {

class Renderer {
    using TriangleBatch = RenderableBatch<gmt::Triangle,  GL_TRIANGLES>;
    using SegmentBatch  = RenderableBatch<gmt::Segment3D, GL_LINES>;

    public:
    TriangleBatch triangles;
    SegmentBatch  segments;
    Camera camera;
    Shader shader;

    Renderer(TriangleBatch &&triangles_, SegmentBatch &&segments_,
             Camera camera_, Shader &&shader_); 

    void draw() const; 
    void clear() const;

    private:
    void configure_gl_state() const;
    void init_matrices() const;
    void apply_camera() const;
};

}
