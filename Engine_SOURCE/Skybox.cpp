#include "Skybox.h"
#include "GraphicDevice.h"

Skybox::Skybox()
{
    struct Vertex2
    {
        float x, y, z;
    };
    std::vector<Vertex2> vertices;
    //Front
    vertices.emplace_back(Vertex2{-0.5, -0.5, 0.5});
    vertices.emplace_back(Vertex2{-0.5, 0.5, 0.5});
    vertices.emplace_back(Vertex2{0.5, 0.5, 0.5});
    vertices.emplace_back(Vertex2{0.5, 0.5, 0.5});
    vertices.emplace_back(Vertex2{0.5, -0.5, 0.5});
    vertices.emplace_back(Vertex2{-0.5, -0.5, 0.5});
    vertices.emplace_back(Vertex2{0.5, -0.5, 0.5});
    vertices.emplace_back(Vertex2{0.5, 0.5, 0.5});
    vertices.emplace_back(Vertex2{0.5, 0.5, -0.5});
    vertices.emplace_back(Vertex2{0.5, 0.5, -0.5});
    vertices.emplace_back(Vertex2{0.5, -0.5, -0.5});
    vertices.emplace_back(Vertex2{0.5, -0.5, 0.5});
    vertices.emplace_back(Vertex2{0.5, -0.5, -0.5});
    vertices.emplace_back(Vertex2{0.5, 0.5, -0.5});
    vertices.emplace_back(Vertex2{-0.5, 0.5, -0.5});
    vertices.emplace_back(Vertex2{-0.5, 0.5, -0.5});
    vertices.emplace_back(Vertex2{-0.5, -0.5, -0.5});
    vertices.emplace_back(Vertex2{0.5, -0.5, -0.5});
    vertices.emplace_back(Vertex2{-0.5, -0.5, -0.5});
    vertices.emplace_back(Vertex2{-0.5, 0.5, -0.5});
    vertices.emplace_back(Vertex2{-0.5, 0.5, 0.5});
    vertices.emplace_back(Vertex2{-0.5, 0.5, 0.5});
    vertices.emplace_back(Vertex2{-0.5, -0.5, 0.5});
    vertices.emplace_back(Vertex2{-0.5, -0.5, -0.5});
    vertices.emplace_back(Vertex2{-0.5, 0.5, 0.5});
    vertices.emplace_back(Vertex2{-0.5, 0.5, -0.5});
    vertices.emplace_back(Vertex2{0.5, 0.5, -0.5});
    vertices.emplace_back(Vertex2{0.5, 0.5, -0.5});
    vertices.emplace_back(Vertex2{0.5, 0.5, 0.5});
    vertices.emplace_back(Vertex2{-0.5, 0.5, 0.5});
    vertices.emplace_back(Vertex2{-0.5, -0.5, -0.5});
    vertices.emplace_back(Vertex2{-0.5, -0.5, 0.5});
    vertices.emplace_back(Vertex2{0.5, -0.5, 0.5});
    vertices.emplace_back(Vertex2{0.5, -0.5, 0.5});
    vertices.emplace_back(Vertex2{0.5, -0.5, -0.5});
    vertices.emplace_back(Vertex2{-0.5, -0.5, -0.5});

    vertex_stride = sizeof(Vertex2);
    vertex_offset = 0;

    D3D11_BUFFER_DESC vertex_buff_descr = {};
    vertex_buff_descr.ByteWidth = sizeof(Vertex2) * vertices.size();
    vertex_buff_descr.Usage = D3D11_USAGE_DEFAULT;
    vertex_buff_descr.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertex_buff_descr.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA sr_data = { 0 };
    sr_data.pSysMem = vertices.data();

    HRESULT hr = GetDevice()->CreateBuffer(
        &vertex_buff_descr,
        (vertices.data()) ? &sr_data : nullptr,
        &vertex_buffer_ptr);
    assert(SUCCEEDED(hr));
}

Skybox::~Skybox()
{

}

void Skybox::Draw()
{
    if (!vertex_buffer_ptr)
    {
        return;
    }

    GetDevice()->GetDeviceContext()->IASetVertexBuffers(
        0,
        1,
        &vertex_buffer_ptr,
        &vertex_stride,
        &vertex_offset);

    GetDevice()->GetDeviceContext()->Draw(36, 0);
}