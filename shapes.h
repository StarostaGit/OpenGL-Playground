#ifndef PLAYGROUND_SHAPES_H
#define PLAYGROUND_SHAPES_H

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <vector>
#include <memory>

namespace shapes {

    struct Shape {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        std::shared_ptr<VertexArray> va = nullptr;
        std::shared_ptr<VertexBuffer> vb = nullptr;
        std::shared_ptr<IndexBuffer> ib = nullptr;
        std::shared_ptr<VertexBufferLayout> layout = nullptr;
    };

    std::vector<unsigned int> generateIndices(int count);
    Shape sphere(float radius, int sectors, int stacks);

}

#endif //PLAYGROUND_SHAPES_H
