#pragma once

#include "RenderableBatch.hpp"
#include "camera.hpp"
#include "Shader.hpp"
#include "geometry.hpp"

namespace TOP_LEVEL_NAMESPACE {

class Renderer {
    using TriangleBatch = RenderableBatch<Triangle,  GL_TRIANGLES>;
    using SegmentBatch  = RenderableBatch<Segment3D, GL_LINES>;

    public:
    TriangleBatch triangles;
    SegmentBatch  segments;
    Camera camera;
    Shader shader;

    void draw() const; 
    void configure_gl_state() const;
};

}
