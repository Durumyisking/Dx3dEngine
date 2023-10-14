#include "InputGeom.h"
#include "NaviMesh.h"
#include "PathMgr.h"
//#include "Recast.h"
//#include "DetourNavMesh.h"

static bool intersectSegmentTriangle(const float* sp, const float* sq,
	const float* a, const float* b, const float* c,
	float& t)
{
	float v, w;
	float ab[3], ac[3], qp[3], ap[3], norm[3], e[3];
	rcVsub(ab, b, a);
	rcVsub(ac, c, a);
	rcVsub(qp, sp, sq);

	// Compute triangle normal. Can be precalculated or cached if
	// intersecting multiple segments against the same triangle
	rcVcross(norm, ab, ac);

	// Compute denominator d. If d <= 0, segment is parallel to or points
	// away from triangle, so exit early
	float d = rcVdot(qp, norm);
	if (d <= 0.0f) return false;

	// Compute intersection t value of pq with plane of triangle. A ray
	// intersects iff 0 <= t. Segment intersects iff 0 <= t <= 1. Delay
	// dividing by d until intersection has been found to pierce triangle
	rcVsub(ap, sp, a);
	t = rcVdot(ap, norm);
	if (t < 0.0f) return false;
	if (t > d) return false; // For segment; exclude this code line for a ray test

	// Compute barycentric coordinate components and test if within bounds
	rcVcross(e, qp, ap);
	v = rcVdot(ac, e);
	if (v < 0.0f || v > d) return false;
	w = -rcVdot(ab, e);
	if (w < 0.0f || v + w > d) return false;

	// Segment/ray intersects triangle. Perform delayed division
	t /= d;

	return true;
}

static char* parseRow(char* buf, char* bufEnd, char* row, int len)
{
	bool start = true;
	bool done = false;
	int n = 0;
	while (!done && buf < bufEnd)
	{
		char c = *buf;
		buf++;
		// multirow
		switch (c)
		{
		case '\n':
			if (start) break;
			done = true;
			break;
		case '\r':
			break;
		case '\t':
		case ' ':
			if (start) break;
			// else falls through
		default:
			start = false;
			row[n++] = c;
			if (n >= len - 1)
				done = true;
			break;
		}
	}
	row[n] = '\0';
	return buf;
}



InputGeom::InputGeom() :
	mMesh(0),
	mbHasBuildSettings(false),
	mOffMeshConCount(0),
	mVolumeCount(0)
{
}

InputGeom::~InputGeom()
{
	delete mMesh;
}

bool InputGeom::loadMesh(const std::wstring& filepath)
{
	if (mMesh)
	{
		delete mMesh;
		mMesh = 0;
	}
	mOffMeshConCount = 0;
	mVolumeCount = 0;

	mMesh = new Mesh();
	if (!mMesh)
	{
		return false;
	}
	//if (!mMesh->load(filepath))
	{
		return false;
	}

	//rcCalcBounds(mMesh->getVerts(), mMesh->getVertCount(), mMeshBMin, mMeshBMax);

	return true;
}

bool InputGeom::loadGeomSet(const std::wstring& filepath)
{
	char* buf = 0;
	FILE* fp = _wfopen(filepath.c_str(), L"rb");
	if (!fp)
	{
		return false;
	}
	if (fseek(fp, 0, SEEK_END) != 0)
	{
		fclose(fp);
		return false;
	}

	long bufSize = ftell(fp);
	if (bufSize < 0)
	{
		fclose(fp);
		return false;
	}
	if (fseek(fp, 0, SEEK_SET) != 0)
	{
		fclose(fp);
		return false;
	}
	buf = new char[bufSize];
	if (!buf)
	{
		fclose(fp);
		return false;
	}
	size_t readLen = fread(buf, bufSize, 1, fp);
	fclose(fp);
	if (readLen != 1)
	{
		delete[] buf;
		return false;
	}

	mOffMeshConCount = 0;
	mVolumeCount = 0;
	delete mMesh;
	mMesh = 0;

	char* src = buf;
	char* srcEnd = buf + bufSize;
	char row[512];
	while (src < srcEnd)
	{
		// Parse one row
		row[0] = '\0';
		src = parseRow(src, srcEnd, row, sizeof(row) / sizeof(char));
		if (row[0] == 'f')
		{
			// File name.
			const char* name = row + 1;
			// Skip white spaces
			while (*name && isspace(*name))
				name++;
			if (*name)
			{
				if (!loadMesh(ConvertToW_String(name)))
				{
					delete[] buf;
					return false;
				}
			}
		}
		else if (row[0] == 'c')
		{
			// Off-mesh connection
			if (mOffMeshConCount < MAX_OFFMESH_CONNECTIONS)
			{
				float* v = &mOffMeshConVerts[mOffMeshConCount * 3 * 2];
				int bidir, area = 0, flags = 0;
				float rad;
				sscanf(row + 1, "%f %f %f  %f %f %f %f %d %d %d",
					&v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &rad, &bidir, &area, &flags);
				mOffMeshConRads[mOffMeshConCount] = rad;
				mOffMeshConDirs[mOffMeshConCount] = (unsigned char)bidir;
				mOffMeshConAreas[mOffMeshConCount] = (unsigned char)area;
				mOffMeshConFlags[mOffMeshConCount] = (unsigned short)flags;
				mOffMeshConCount++;
			}
		}
		else if (row[0] == 'v')
		{
			// Convex volumes
			if (mVolumeCount < MAX_VOLUMES)
			{
				ConvexVolume* vol = &mVolumes[mVolumeCount++];
				sscanf(row + 1, "%d %d %f %f", &vol->nverts, &vol->area, &vol->hmin, &vol->hmax);
				for (int i = 0; i < vol->nverts; ++i)
				{
					row[0] = '\0';
					src = parseRow(src, srcEnd, row, sizeof(row) / sizeof(char));
					sscanf(row, "%f %f %f", &vol->verts[i * 3 + 0], &vol->verts[i * 3 + 1], &vol->verts[i * 3 + 2]);
				}
			}
		}
		else if (row[0] == 's')
		{
			// Settings
			mbHasBuildSettings = true;
			sscanf(row + 1, "%f %f %f %f %f %f %f %f %f %f %f %f %f %d %f %f %f %f %f %f %f",
				&mBuildSetting.cellSize,
				&mBuildSetting.cellHeight,
				&mBuildSetting.agentHeight,
				&mBuildSetting.agentRadius,
				&mBuildSetting.agentMaxClimb,
				&mBuildSetting.agentMaxSlope,
				&mBuildSetting.regionMinSize,
				&mBuildSetting.regionMergeSize,
				&mBuildSetting.edgeMaxLen,
				&mBuildSetting.edgeMaxError,
				&mBuildSetting.vertsPerPoly,
				&mBuildSetting.detailSampleDist,
				&mBuildSetting.detailSampleMaxError,
				&mBuildSetting.partitionType,
				&mBuildSetting.navMeshBMin[0],
				&mBuildSetting.navMeshBMin[1],
				&mBuildSetting.navMeshBMin[2],
				&mBuildSetting.navMeshBMax[0],
				&mBuildSetting.navMeshBMax[1],
				&mBuildSetting.navMeshBMax[2],
				&mBuildSetting.tileSize);
		}
	}

	delete[] buf;

	return true;
}

bool InputGeom::Load(const std::wstring& filepath)
{
	size_t extensionPos = filepath.find_last_of('.');
	if (extensionPos == std::wstring::npos)
		return false;

	std::wstring extension = filepath.substr(extensionPos);
	std::transform(extension.begin(), extension.end(), extension.begin(), tolower);

	if (extension == L".gset")
		return loadGeomSet(filepath);
	if (extension == L".obj")
		return loadMesh(filepath);

	return false;
}

bool InputGeom::SaveGeomSet(const BuildSettings* settings)
{
	if (!mMesh) return false;

	std::wstring meshSavePath = GETSINGLE(PathMgr)->FindPath(MESH_SAVE_PATH);
	std::wstring meshName = mMesh->GetName();

	// Change extension
	std::wstring filepath = meshSavePath + meshName;
	size_t extPos = filepath.find_last_of('.');
	if (extPos != std::string::npos)
		filepath = filepath.substr(0, extPos);

	filepath += L".gset";

	FILE* fp = _wfopen(filepath.c_str(), L"w");
	if (!fp) return false;

	// Store mesh filename.
	fprintf(fp, "f %s\n", meshName.c_str());

	// Store settings if any
	if (settings)
	{
		fprintf(fp,
			"s %f %f %f %f %f %f %f %f %f %f %f %f %f %d %f %f %f %f %f %f %f\n",
			settings->cellSize,
			settings->cellHeight,
			settings->agentHeight,
			settings->agentRadius,
			settings->agentMaxClimb,
			settings->agentMaxSlope,
			settings->regionMinSize,
			settings->regionMergeSize,
			settings->edgeMaxLen,
			settings->edgeMaxError,
			settings->vertsPerPoly,
			settings->detailSampleDist,
			settings->detailSampleMaxError,
			settings->partitionType,
			settings->navMeshBMin[0],
			settings->navMeshBMin[1],
			settings->navMeshBMin[2],
			settings->navMeshBMax[0],
			settings->navMeshBMax[1],
			settings->navMeshBMax[2],
			settings->tileSize);
	}

	// Store off-mesh links.
	for (int i = 0; i < mOffMeshConCount; ++i)
	{
		const float* v = &mOffMeshConVerts[i * 3 * 2];
		const float rad = mOffMeshConRads[i];
		const int bidir = mOffMeshConDirs[i];
		const int area = mOffMeshConAreas[i];
		const int flags = mOffMeshConFlags[i];
		fprintf(fp, "c %f %f %f  %f %f %f  %f %d %d %d\n",
			v[0], v[1], v[2], v[3], v[4], v[5], rad, bidir, area, flags);
	}

	// Convex volumes
	for (int i = 0; i < mVolumeCount; ++i)
	{
		ConvexVolume* vol = &mVolumes[i];
		fprintf(fp, "v %d %d %f %f\n", vol->nverts, vol->area, vol->hmin, vol->hmax);
		for (int j = 0; j < vol->nverts; ++j)
			fprintf(fp, "%f %f %f\n", vol->verts[j * 3 + 0], vol->verts[j * 3 + 1], vol->verts[j * 3 + 2]);
	}

	fclose(fp);

	return true;
}

static bool isectSegAABB(const float* sp, const float* sq,
	const float* amin, const float* amax,
	float& tmin, float& tmax)
{
	static const float EPS = 1e-6f;

	float d[3];
	d[0] = sq[0] - sp[0];
	d[1] = sq[1] - sp[1];
	d[2] = sq[2] - sp[2];
	tmin = 0.0;
	tmax = 1.0f;

	for (int i = 0; i < 3; i++)
	{
		if (fabsf(d[i]) < EPS)
		{
			if (sp[i] < amin[i] || sp[i] > amax[i])
				return false;
		}
		else
		{
			const float ood = 1.0f / d[i];
			float t1 = (amin[i] - sp[i]) * ood;
			float t2 = (amax[i] - sp[i]) * ood;
			if (t1 > t2) { float tmp = t1; t1 = t2; t2 = tmp; }
			if (t1 > tmin) tmin = t1;
			if (t2 < tmax) tmax = t2;
			if (tmin > tmax) return false;
		}
	}

	return true;
}

void InputGeom::AddOffMeshConnection(const float* spos, const float* epos, const float rad,
	unsigned char bidir, unsigned char area, unsigned short flags)
{
	if (mOffMeshConCount >= MAX_OFFMESH_CONNECTIONS) return;
	float* v = &mOffMeshConVerts[mOffMeshConCount * 3 * 2];
	mOffMeshConRads[mOffMeshConCount] = rad;
	mOffMeshConDirs[mOffMeshConCount] = bidir;
	mOffMeshConAreas[mOffMeshConCount] = area;
	mOffMeshConFlags[mOffMeshConCount] = flags;
	mOffMeshConId[mOffMeshConCount] = 1000 + mOffMeshConCount;
	rcVcopy(&v[0], spos);
	rcVcopy(&v[3], epos);
	mOffMeshConCount++;
}

void InputGeom::DeleteOffMeshConnection(int i)
{
	mOffMeshConCount--;
	float* src = &mOffMeshConVerts[mOffMeshConCount * 3 * 2];
	float* dst = &mOffMeshConVerts[i * 3 * 2];
	rcVcopy(&dst[0], &src[0]);
	rcVcopy(&dst[3], &src[3]);
	mOffMeshConRads[i] = mOffMeshConRads[mOffMeshConCount];
	mOffMeshConDirs[i] = mOffMeshConDirs[mOffMeshConCount];
	mOffMeshConAreas[i] = mOffMeshConAreas[mOffMeshConCount];
	mOffMeshConFlags[i] = mOffMeshConFlags[mOffMeshConCount];
}

void InputGeom::AddConvexVolume(const float* verts, const int nverts,
	const float minh, const float maxh, unsigned char area)
{
	if (mVolumeCount >= MAX_VOLUMES) return;
	ConvexVolume* vol = &mVolumes[mVolumeCount++];
	memset(vol, 0, sizeof(ConvexVolume));
	memcpy(vol->verts, verts, sizeof(float) * 3 * nverts);
	vol->hmin = minh;
	vol->hmax = maxh;
	vol->nverts = nverts;
	vol->area = area;
}

void InputGeom::DeleteConvexVolume(int i)
{
	mVolumeCount--;
	mVolumes[i] = mVolumes[mVolumeCount];
}
