#pragma once
#include "def.h"
#include "CommonInclude.h"
#include <tchar.h>

#define	ROOT_PATH		"Root"
#define	RESOURCE_PATH	"Resource"
#define	SAVE_PATH		"Save"
#define	SCENE_SAVE_PATH	"SceneSave"
#define	LAYER_SAVE_PATH	"LayerSave"
#define	MESH_SAVE_PATH	"MeshSave"
#define	OBJ_SAVE_PATH	"ObjSave"
#define	SOUND_PATH		"Sound"
#define	FONT_PATH		"Font"

class PathMgr
{
	SINGLE(PathMgr)
public:
	bool Initialize();
	bool AddPath(const std::string& Name, const std::wstring& Path,
		const std::string& BasePathName = ROOT_PATH);
	const std::wstring& FindPath(const std::string& Name);
	void ResetPath();
private:
	std::unordered_map<std::string, std::wstring> mPathMap;
};
