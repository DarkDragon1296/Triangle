#include "Renderer.hpp"

namespace render {

Renderer::Renderer(TriangleBatch &&triangles_, SegmentBatch &&segments_,
                   Camera camera_, Shader &&shader_) 
    : triangles{std::move(triangles_)},
      segments{std::move(segments_)},
      camera{camera_},
      shader{std::move(shader_)}
{
    configure_gl_state();
    shader.use();
    init_matrices();
}

void Renderer::clear() const {
  glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw() const {
    shader.use();
    apply_camera();

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 8.0f);
    glUniform1i(shader.get_uniform_location("isIntersection"), true);
    segments.draw();

    glDisable(GL_POLYGON_OFFSET_FILL);
    glUniform1i(shader.get_uniform_location("isIntersection"), false);
    triangles.draw();
}

void Renderer::configure_gl_state() const {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glLineWidth(10.0f);
    glEnable(GL_LINE_SMOOTH);
}

void Renderer::apply_camera() const {
    glUniformMatrix4fv(shader.get_uniform_location("view"), 
                       1, GL_FALSE, glm::value_ptr(camera.view));
}


void Renderer::init_matrices() const {
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            (float)WIDTH / (float)HEIGHT,
                                            0.1f, 100.0f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    apply_camera();
    glUniformMatrix4fv(shader.get_uniform_location("model"), 
                       1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.get_uniform_location("projection"), 
                       1, GL_FALSE, glm::value_ptr(projection));
}

}
