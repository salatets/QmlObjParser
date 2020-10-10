#include <FloatingHorizon.h>

FloatingHorizon::FloatingHorizon(Mesh m, QOpenGLContext* m_context){
    // TODO fix after mesh iterator

    size_t vert_size = 0;

    for(auto p = m.getNodesBegin(); p != m.getNodesEnd(); ++p){
        vert_size += (*p).getSize();
    }

    vertices.reserve(vert_size * 3);
    this->size = vert_size;
    size_t indent = 0;
    // TODO memcpy
    for(auto p = m.getNodesBegin(); p != m.getNodesEnd(); ++p){
        for(size_t i = indent, j = 0; i < (indent + (*p).getSize()); ++i){
            vertices[i * 3] = (*p).getData()[j * 3];
            vertices[i * 3 + 1] = (*p).getData()[j * 3 + 1];
            vertices[i * 3 + 2] = (*p).getData()[j * 3 + 2];
            ++j;
        }
        indent += (*p).getSize();
    }

    if(m_context == nullptr)
        return;

    m_funcs = m_context->versionFunctions<QOpenGLFunctions_4_3_Core>();

    if (!m_funcs) {
      qWarning("Could not obtain OpenGL versions object");
      exit(1);
    }

    m_funcs->initializeOpenGLFunctions();
}

void FloatingHorizon::setShader(QOpenGLShaderProgram* shader){
    this->shader = shader;
    dirty = true;
}

void FloatingHorizon::initBuffers(){
    Q_ASSERT(shader == nullptr);

    bool success = vao.create();
    Q_ASSERT(success);

    vao.bind();

    vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    success = vbo.create();
    Q_ASSERT(success);
    vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);

    success = vbo.bind();
    Q_ASSERT(success);
    vbo.allocate(&vertices[0], this->size * 6 * sizeof(float)); // x y z r g b
    shader->setAttributeBuffer("position", GL_FLOAT, 0, 3, 6);
    shader->enableAttributeArray("position");
    shader->setAttributeBuffer("color", GL_FLOAT, 3, 3, 6);
    shader->enableAttributeArray("color");

    vbo.release();
    vao.release();

    dirty = false;
}

void FloatingHorizon::clearHorizons(int width){
    lower_horizon.reserve(width);
    higher_horizon.reserve(width);

    for(int& hor : lower_horizon){
        hor = std::numeric_limits<float>::max();
    }

    for(int& hor : higher_horizon){
        hor = std::numeric_limits<float>::lowest();
    }
}

auto FloatingHorizon::getPointsToDraw(QMatrix4x4 proj, QMatrix4x4 view, int width, int height){
    std::vector<std::array<QVector3D,2>> draw_vertices;
    draw_vertices.reserve(this->size);

    // Transform vertices to screen space
    for(size_t i = 0; i < this->size; ++i){
        QVector4D vec( vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2], 1.0f );
        std::array<int,2> point;

        QVector4D proj_vec = proj * view * vec;

        if (abs(vec.w()) > std::numeric_limits<float>::epsilon())
        {
            proj_vec.setX(proj_vec.x() / proj_vec.w());
            proj_vec.setY(proj_vec.y() / proj_vec.w());
        }
        // MAYBE add another rounding
        point[0] = width * (proj_vec.x() + 1) / 2;
        point[1] = height * (proj_vec.y() + 1) /2;
        if (point[0] < 0)
            point[0] = 0;
        else if (point[0] >= width)
            point[0] = width - 1;

        // check horizons
        bool dn = false;
        if (point[1] > higher_horizon[point[0]])
        {
            higher_horizon[point[0]] = point[1];
            draw_vertices.emplace_back(vec.toVector3DAffine(), 0,255,0);
            dn = true;
        }

        if (point[1] < lower_horizon[point[0]])
        {
            lower_horizon[point[0]] = point[1];
            if (!dn)
            {
                draw_vertices.emplace_back(vec.toVector3DAffine(), 255,0,0);
            }

         }
    }

    draw_vertices.shrink_to_fit();

    return draw_vertices;
}

void writeToVBO(void* vbo_data,const std::vector<std::array<QVector3D,2>>& points, size_t index){
    static_cast<float *>(vbo_data)[index * 6] = points[index][1].x();
    static_cast<float *>(vbo_data)[index * 6 + 1] = points[index][1].y();
    static_cast<float *>(vbo_data)[index * 6 + 2] = points[index][1].z();
    static_cast<float *>(vbo_data)[index * 6 + 3] = points[index][2].x();
    static_cast<float *>(vbo_data)[index * 6 + 4] = points[index][2].y();
    static_cast<float *>(vbo_data)[index * 6 + 5] = points[index][2].z();
}

void FloatingHorizon::draw(QMatrix4x4 proj, QMatrix4x4 view, int width, int height){
    if(dirty)
        return;

    clearHorizons(width);
    auto points = getPointsToDraw(proj,view, width, height);

    shader->bind();
    vao.bind();

    m_funcs->glViewport(0, 0, width, height);

    m_funcs->glDisable(GL_DEPTH_TEST);

    void* vbo_data = vbo.map(QOpenGLBuffer::WriteOnly);

    for(size_t i = 0; i < points.size(); ++i){
        writeToVBO(vbo_data, points, i);
    }


    m_funcs->glDrawArrays(GL_TRIANGLES, 0, points.size());

    vao.release();
    shader->release();

}
