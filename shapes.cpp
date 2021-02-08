#include "shapes.h"
#include <cmath>

namespace shapes {

    std::vector<unsigned int> generateIndices(int count) {
        std::vector<unsigned int> indices(count);
        for (unsigned int i = 0; i < count; i++) indices.push_back(i);
        return std::move(indices);
    }

    Shape sphere(float radius, int sectors, int stacks) {
        float x, y, z, xy;

        float sectorStep = 2.0f * M_PI / sectors;
        float stackStep = M_PI / stacks;
        float sectorAngle, stackAngle;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        for (unsigned int i = 0; i <= stacks; i++) {
            stackAngle = M_PI / 2.0f - (float) i * stackStep;
            xy = radius * cosf(stackAngle);
            z = radius * sinf(stackAngle);

            for (unsigned int j = 0; j <= sectors; j++) {
                sectorAngle = (float) j * sectorStep;

                x = xy * cosf(sectorAngle);
                y = xy * sinf(sectorAngle);

                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
            }
        }

        unsigned int k1, k2;
        for (unsigned int i = 0; i < stacks; i++) {
            k1 = i * (sectors + 1);
            k2 = k1 + sectors + 1;

            for (unsigned int j = 0; j < sectors; j++, k1++, k2++) {
                if (i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);

                    if (i != stacks - 1) {
                        indices.push_back(k1 + 1);
                        indices.push_back(k2);
                        indices.push_back(k2 + 1);
                    }
                }
            }
        }

        return { .vertices = std::move(vertices), .indices = std::move(indices) };
    }

}