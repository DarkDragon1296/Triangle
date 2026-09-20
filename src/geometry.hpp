#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

struct triangle {
    glm::vec3 dots[3];
};

int intersect_dim(struct triangle &tr1, struct triangle &tr2);

#endif