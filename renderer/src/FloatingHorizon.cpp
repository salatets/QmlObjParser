#include <FloatingHorizon.h>
#include <array>

void FloatingHorizon::setMesh(MeshRoot mesh){
    // todo add loading full model
    m_model = mesh;

    size_t size = mesh.front().getSize();

    vertices.clear();
    vertices.reserve(size);

    // FIXME first two vertex is broken if front() is used
    auto vertex_data = reinterpret_cast<const QVector3D*>(mesh.cbegin()->getData());

    vertices.assign(vertex_data, vertex_data + size);

    isInit = false;
}

void FloatingHorizon::init_buffers(){
    if(!vao.isCreated()){
        bool success = vao.create();
        Q_ASSERT(success);
    }

    vao.bind();

    if (!vbo.isCreated()){
        vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        bool success = vbo.create();
        Q_ASSERT(success);
        vbo.setUsagePattern(QOpenGLBuffer::StreamDraw);
    }

    bool success = vbo.bind();
    Q_ASSERT(success);

    auto* program = getShader(1).get();

    vbo.allocate(nullptr, vertices.size() * sizeof(QVector3D) * 2); // coord color
    program->setAttributeBuffer("position", GL_FLOAT, 0, 3,6 * sizeof(GLfloat));
    program->enableAttributeArray("position");
    program->setAttributeBuffer("color", GL_FLOAT, 3 * sizeof(GLfloat), 3,6 * sizeof(GLfloat));
    program->enableAttributeArray("color");

    vbo.release();
    vao.release();

    isInit = true;
}

auto FloatingHorizon::getPointsToDraw(QMatrix4x4 proj, GLsizei width, GLsizei height, int point_size){
    std::vector<std::array<QVector3D,2>> draw_vertices;
    draw_vertices.reserve(vertices.size());

    std::vector<std::pair<size_t,QVector4D>> proj_vecs;

    for(size_t i = 0; i < vertices.size(); ++i){
        QVector4D proj_vec = proj * vertices[i].toVector4D();

        proj_vecs.emplace_back(i,proj_vec);
    }

    std::sort(proj_vecs.begin(), proj_vecs.end(), [](auto a, auto b){return a.second.z() < b.second.z();});

    // Transform vertices to screen space
    for(size_t i = 0; i < vertices.size(); ++i){
        int point[2];

        // MAYBE add another rounding
        point[0] = width * (proj_vecs[i].second.x() + 1) / 2;
        point[1] = height * (proj_vecs[i].second.y() + 1) /2;
        if (point[0] < 0)
            point[0] = 0;
        else if (point[0] >= width)
            point[0] = width - 1;

        // check horizons
        bool hi = false;
        bool acc = false;

        // radius
        for(int j = point[0] - point_size + 1; j < point[0] + point_size; ++j){
            if(j < 0 || j >= width)
                continue;

            if(higher_horizon[j] < point[1]){
                higher_horizon[j] = point[1];
                acc = true;
                hi = true;
            }

            if(lower_horizon[j] > point[1]){
                lower_horizon[j] = point[1];
                acc = true;
            }
        }

        if (acc){
            if(hi){
                draw_vertices.emplace_back(std::array<QVector3D,2>{{
                                                                       vertices[proj_vecs[i].first], QVector3D(255,0,0)}}
                                           );
            }else{
                draw_vertices.emplace_back(std::array<QVector3D,2>{{
                                                                       vertices[proj_vecs[i].first], QVector3D(0,0,255)}}
                                           );
            }
        }


    }

    draw_vertices.shrink_to_fit();

    return draw_vertices;
}

void FloatingHorizon::clearHorizons(int width, int height){
    lower_horizon.clear();
    higher_horizon.clear();

    lower_horizon.reserve(width);
    higher_horizon.reserve(width);

    for(int i = 0; i < width; ++i){
        lower_horizon.push_back(height);
    }

    for(int i = 0; i < width; ++i){
        higher_horizon.push_back(0);
    }
}

// FIXME light shader render after light pos move
void FloatingHorizon::paint(const std::function<std::tuple<GLsizei, GLsizei, QMatrix4x4>(const MeshRoot &)> & get_param)
{   
    auto params = get_param(m_model);

    auto* program = getShader(1).get();

    program->bind();

    program->setUniformValue("model",std::get<2>(params));

    glEnable(GL_PROGRAM_POINT_SIZE);

    clearHorizons(std::get<0>(params), std::get<1>(params));

    auto vc_draw = getPointsToDraw(std::get<2>(params), std::get<0>(params), std::get<1>(params), 4);

    vao.bind();
    vbo.bind();
    vbo.write(0,&vc_draw.data()[0],vc_draw.size() * 6 * sizeof(float));
    vbo.release();

    glDrawArrays(GL_POINTS, 0, vc_draw.size());

    vao.release();
    program->release();
}

void FloatingHorizon::afterSetShader(int){
    isInit=false;
}
