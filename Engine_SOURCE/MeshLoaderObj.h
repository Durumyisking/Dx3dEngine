#ifndef MESHLOADER_OBJ
#define MESHLOADER_OBJ

#include <string>

class rcMeshLoaderObj
{
public:
	rcMeshLoaderObj();
	~rcMeshLoaderObj();
	
	bool load(const std::string& fileName);

	const float* getVerts() const { return m_verts; }
	const float* getNormals() const { return m_normals; }
	const int* getTris() const { return m_tris; }
	int getVertCount() const { return m_vertCount; }
	int getTriCount() const { return m_triCount; }
	const std::string& getFileName() const { return m_filename; }

private:
	// Explicitly disabled copy constructor and copy assignment operator.
	rcMeshLoaderObj(const rcMeshLoaderObj&);
	rcMeshLoaderObj& operator=(const rcMeshLoaderObj&);
	
	void addVertex(float x, float y, float z, int& cap);
	void addTriangle(int a, int b, int c, int& cap);
	
	std::string m_filename;
	float m_scale;	
	float* m_verts;
	int* m_tris;
	float* m_normals;
	int m_vertCount;
	int m_triCount;
};

#endif // MESHLOADER_OBJ
