#include "glad.hpp"

#include <glm/fwd.hpp>
#include <glm/detail/type_vec3.hpp>
#include <vector>

template <typename T> // Actually not all T's are welcome here
class RenderableBatch {
    private:
    std::vector<T> batch;
    GLuint vbo = 0, vao = 0;

    public:
    // Default ctor
    RenderableBatch(const std::vector<T> &_batch) 
        : batch{_batch} 
    {
        init_gl_handles();
    }

    // Move ctor from vec
    RenderableBatch(std::vector<T> &&_batch) 
        : batch{std::move(_batch)} 
    {
        init_gl_handles();
    }

    // Copy ctor
    RenderableBatch(const RenderableBatch &rhs) 
        : batch{rhs.batch} 
    {
        init_gl_handles();
    }

    // Move ctor
    RenderableBatch(RenderableBatch &&rhs) 
        : batch{std::move(rhs.batch)}, 
          vbo{rhs.vbo}, 
          vao{rhs.vao}
    {
        rhs.vbo = 0;
        rhs.vao = 0;
    }

    // Copy assign
    RenderableBatch &operator=(const RenderableBatch &rhs) {
        if (this == &rhs)
            return *this;

        batch = rhs.batch;
        if (vao == 0 && vbo == 0) // Moved-from state, needs init for gl handles
            init_gl_handles();    // Also change buffer data according the new vec
        else // We already have allocated vao and vbo, just
             // change buffer data to reflect current vec
            bind_buffer_data(); 

        return *this;
    }

    // Move assign
    RenderableBatch &operator=(RenderableBatch &&rhs) {
        if (this == &rhs)
            return *this;

        batch = std::move(rhs.batch);
        delete_gl_handles();
        vbo = rhs.vbo;
        rhs.vbo = 0;
        vao = rhs.vao;
        rhs.vao = 0;

        return *this;
    }

    // Dtor
    ~RenderableBatch() {
        delete_gl_handles();
    }
    
    private:
    void init_gl_handles() {        
        // Any way to reduce the occurence of magic numbers?
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                              0, nullptr);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &vbo);
        bind_buffer_data();
    }

    void bind_buffer_data() {
        if (vbo == 0)
            return;

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 
                     (GLsizeiptr)(batch.size() * sizeof(T)), 
                     batch.data(), GL_STATIC_DRAW);
    }

    void delete_gl_handles() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }
};

