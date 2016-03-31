#include <iostream>
#include <string>
#include <vector>
#include <fbxsdk.h>

#include "Utils.h"
#include "SPMWriter.h"

namespace sp { namespace utils {

	static std::vector<String> SplitString(const String& string, const String& delimiters)
	{
		size_t start = 0;
		size_t end = string.find_first_of(delimiters);

		std::vector<String> result;

		while (end <= String::npos)
		{
			result.emplace_back(string.substr(start, end - start));

			if (end == String::npos)
				break;

			start = end + 1;
			end = string.find_first_of(delimiters, start);
		}

		return result;
	}

	static std::vector<String> SplitString(const String& string, const char delimiter)
	{
		return SplitString(string, String(1, delimiter));
	}

	static std::vector<String> SplitStringIntoTokens(const String& string)
	{
		return SplitString(string, " \t\n");
	}

} }

using namespace sp::tools;
using namespace sp::utils;

static std::vector<Vertex> s_Vertices;
static std::vector<uint> s_Indices;

static std::unordered_map<Vertex, uint> s_IndexMapping;

static vec3* s_RawPositions;
static uint s_Attributes;

static String s_Name, s_OutputPath;

static void InsertVertex(const vec3& position, const vec3& normal, const vec2& uv, const vec3& binormal, const vec3& tangent)
{
	Vertex vertex = { position, normal, uv, binormal, tangent };
	auto lookup = s_IndexMapping.find(vertex);
	if (lookup != s_IndexMapping.end())
	{
		s_Indices.push_back(lookup->second);
	}
	else
	{
		uint index = s_Vertices.size();
		s_IndexMapping[vertex] = index;
		s_Indices.push_back(index);
		s_Vertices.push_back(vertex);
	}
}

void ProcessControlPoints(const FbxMesh* mesh)
{
	uint count = mesh->GetControlPointsCount();
	s_RawPositions = new vec3[count];
	for (uint i = 0; i < count; i++)
	{
		vec3 position;
		position.x = static_cast<float>(mesh->GetControlPointAt(i).mData[0]);
		position.y = static_cast<float>(mesh->GetControlPointAt(i).mData[1]);
		position.z = static_cast<float>(mesh->GetControlPointAt(i).mData[2]);
		s_RawPositions[i] = position;
	}
	s_Attributes |= (1 * POSITION);
}

vec3 ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	if (mesh->GetElementNormalCount() < 1)
		std::cout << "No normals!" << std::endl;

	vec3 result;
	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
	switch (vertexNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
			result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		default:
			std::cout << "Error: Invalid vertex reference mode!" << std::endl;
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
			result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		default:
			std::cout << "Error: Invalid vertex reference mode!" << std::endl;
		}
		break;
	}
	s_Attributes |= NORMAL;
	return result;
}

vec2 ReadUV(const FbxMesh* inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer)
{
	if (inUVLayer >= 2 || inMesh->GetElementUVCount() <= inUVLayer)
		std::cout << "Invalid UV Layer Number" << std::endl;

	const FbxGeometryElementUV* vertexUV = inMesh->GetElementUV(inUVLayer);

	vec2 result;
	switch (vertexUV->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			result.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexUV->GetIndexArray().GetAt(inCtrlPointIndex);
			result.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
		}
		break;
		default:
			std::cout << "Error: Invalid vertex reference mode!" << std::endl;
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			result.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[0]);
			result.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[1]);
		}
		break;
		default:
			std::cout << "Error: Invalid vertex reference mode!" << std::endl;
		}
		break;
	}
	s_Attributes |= UV;
	return result;
}

vec3 ReadBinormal(FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	if (mesh->GetElementBinormalCount() < 1)
		std::cout << "Invalid Binormal Number" << std::endl;

	FbxGeometryElementBinormal* vertexBinormal = mesh->GetElementBinormal(0);
	vec3 result;
	switch (vertexBinormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexBinormal->GetIndexArray().GetAt(controlPointIndex);
			result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		default:
			std::cout << "Error: Invalid vertex reference mode!" << std::endl;
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexBinormal->GetIndexArray().GetAt(vertexCounter);
			result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		default:
			std::cout << "Error: Invalid vertex reference mode!" << std::endl;
		}
		break;
	}
	s_Attributes |= BINORMAL;
	return result;
}

vec3 ReadTangent(FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	if (mesh->GetElementTangentCount() < 1)
		std::cout << "Invalid Tangent Number" << std::endl;

	FbxGeometryElementTangent* vertexTangent = mesh->GetElementTangent(0);
	vec3 result;
	switch (vertexTangent->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexTangent->GetIndexArray().GetAt(controlPointIndex);
			result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		default:
			std::cout << "Error: Invalid vertex reference mode!" << std::endl;
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexTangent->GetIndexArray().GetAt(vertexCounter);
			result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		default:
			std::cout << "Error: Invalid vertex reference mode!" << std::endl;
		}
		break;
	}
	s_Attributes |= TANGENT;
	return result;
}

// Save up data into GPU buffers.
bool Initialize(FbxMesh* mesh)
{
	if (!mesh->GetNode())
		return false;

	const int polygonCount = mesh->GetPolygonCount();
	std::cout << "Importing " << polygonCount << " polygons." << std::endl;
	ProcessControlPoints(mesh);
	uint triCount = mesh->GetPolygonCount();
	int vertexCounter = 0;

	for (uint i = 0; i < triCount; i++)
	{
		vec3 tangent;
		vec3 binormal;
		vec2 uv;

		for (uint j = 0; j < 3; j++)
		{
			int controlPointIndex = mesh->GetPolygonVertex(i, j);
			vec3& position = s_RawPositions[controlPointIndex];
			vec3 normal = ReadNormal(mesh, controlPointIndex, vertexCounter);
			vec3 binormal = ReadBinormal(mesh, controlPointIndex, vertexCounter);
			vec3 tangent = ReadTangent(mesh, controlPointIndex, vertexCounter);

			// for (uint t = 0; t < 1; t++) // TODO: Support multiple layers
			vec2 uv = ReadUV(mesh, controlPointIndex, mesh->GetTextureUVIndex(i, j), 0);

			InsertVertex(position, normal, uv, binormal, tangent);
			vertexCounter++;
		}
	}

#if SP_DEBUG
	std::cout << s_Vertices.size() << " vertices:" << std::endl;
	for (Vertex& v : s_Vertices)
	{
		std::cout << "{" << std::endl;
		std::cout << "\t" << v.position.x << ", " << v.position.y << ", " << v.position.z << std::endl;
		std::cout << "\t" << v.normal.x << ", " << v.normal.y << ", " << v.normal.z << std::endl;
		std::cout << "\t" << v.uv.x << ", " << v.uv.y << std::endl;
		std::cout << "\t" << v.binormal.x << ", " << v.binormal.y << ", " << v.binormal.z << std::endl;
		std::cout << "\t" << v.tangent.x << ", " << v.tangent.y << ", " << v.tangent.z << std::endl;
		std::cout << "}" << std::endl;
	}
	std::cout << std::endl;
	std::cout << s_Indices.size() << " indices:" << std::endl;
	for (uint i : s_Indices)
	{
		std::cout << i << std::endl;
	}
#endif

	SPMWriter writer(s_Name, s_Attributes, s_Vertices, s_Indices);
	writer.Write(s_OutputPath);
	return true;
}

static FbxDouble3 GetMaterialProperty(const FbxSurfaceMaterial * pMaterial,
	const char * pPropertyName,
	const char * pFactorPropertyName,
	uint& pTextureName)
{
	FbxDouble3 lResult(0, 0, 0);
	const FbxProperty lProperty = pMaterial->FindProperty(pPropertyName);
	const FbxProperty lFactorProperty = pMaterial->FindProperty(pFactorPropertyName);
	if (lProperty.IsValid() && lFactorProperty.IsValid())
	{
		lResult = lProperty.Get<FbxDouble3>();
		double lFactor = lFactorProperty.Get<FbxDouble>();
		if (lFactor != 1)
		{
			lResult[0] *= lFactor;
			lResult[1] *= lFactor;
			lResult[2] *= lFactor;
		}
	}

	if (lProperty.IsValid())
	{
		const int lTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
		if (lTextureCount)
		{
			const FbxFileTexture* lTexture = lProperty.GetSrcObject<FbxFileTexture>();
			if (lTexture && lTexture->GetUserDataPtr())
			{
				pTextureName = *(static_cast<uint*>(lTexture->GetUserDataPtr()));
			}
		}
	}

	return lResult;
}

// Cache for FBX material
class MaterialCache
{
public:
	MaterialCache() : mShinness(0) {}
	~MaterialCache() {}

	bool Initialize(const FbxSurfaceMaterial* pMaterial)
	{
		const FbxDouble3 lEmissive = GetMaterialProperty(pMaterial,
			FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor, mEmissive.mTextureName);
		mEmissive.mColor[0] = static_cast<float>(lEmissive[0]);
		mEmissive.mColor[1] = static_cast<float>(lEmissive[1]);
		mEmissive.mColor[2] = static_cast<float>(lEmissive[2]);

		const FbxDouble3 lAmbient = GetMaterialProperty(pMaterial,
			FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor, mAmbient.mTextureName);
		mAmbient.mColor[0] = static_cast<float>(lAmbient[0]);
		mAmbient.mColor[1] = static_cast<float>(lAmbient[1]);
		mAmbient.mColor[2] = static_cast<float>(lAmbient[2]);

		const FbxDouble3 lDiffuse = GetMaterialProperty(pMaterial,
			FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor, mDiffuse.mTextureName);
		mDiffuse.mColor[0] = static_cast<float>(lDiffuse[0]);
		mDiffuse.mColor[1] = static_cast<float>(lDiffuse[1]);
		mDiffuse.mColor[2] = static_cast<float>(lDiffuse[2]);

		const FbxDouble3 lSpecular = GetMaterialProperty(pMaterial,
			FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor, mSpecular.mTextureName);
		mSpecular.mColor[0] = static_cast<float>(lSpecular[0]);
		mSpecular.mColor[1] = static_cast<float>(lSpecular[1]);
		mSpecular.mColor[2] = static_cast<float>(lSpecular[2]);

		FbxProperty lShininessProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
		if (lShininessProperty.IsValid())
		{
			double lShininess = lShininessProperty.Get<FbxDouble>();
			mShinness = static_cast<float>(lShininess);
		}

		return true;
	}
private:
	struct ColorChannel
	{
		uint mTextureName;
		float mColor[4];
		ColorChannel() : mTextureName(0)
		{
			mColor[0] = 0.0f;
			mColor[1] = 0.0f;
			mColor[2] = 0.0f;
			mColor[3] = 1.0f;
		}
	};
	ColorChannel mEmissive;
	ColorChannel mAmbient;
	ColorChannel mDiffuse;
	ColorChannel mSpecular;
	float mShinness;
};

static void LoadNode(FbxNode* node, FbxAnimLayer* anim)
{
	int materialCount = node->GetMaterialCount();
	for (int i = 0; i < materialCount; i++)
	{
		FbxSurfaceMaterial* material = node->GetMaterial(i);
		if (material != nullptr && !material->GetUserDataPtr())
		{
			FbxAutoPtr<MaterialCache> materialCache(new MaterialCache());
			if (materialCache->Initialize(material))
				material->SetUserDataPtr(materialCache.Release());
		}
	}

	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
	if (nodeAttribute)
	{
		// Bake mesh as VBO(vertex buffer object) into GPU.
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh* mesh = node->GetMesh();
			if (mesh && !mesh->GetUserDataPtr())
				Initialize(mesh);
		}
		// Bake light properties.
		else if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eLight)
		{
			std::cout << "Note: Lights are currently not supported" << std::endl;
		}
	}

	const int childCount = node->GetChildCount();
	for (int i = 0; i < childCount; i++)
		LoadNode(node->GetChild(i), anim);
}

void LoadScene(FbxScene * pScene, FbxAnimLayer * pAnimLayer, const String& path)
{
	const int textureCount = pScene->GetTextureCount();
	for (int ti = 0; ti < textureCount; ti++)
	{
#if 0
		FbxTexture * lTexture = pScene->GetTexture(lTextureIndex);
		FbxFileTexture * lFileTexture = FbxCast<FbxFileTexture>(lTexture);
		if (lFileTexture && !lFileTexture->GetUserDataPtr())
		{
			// Try to load the texture from absolute path
			const FbxString lFileName = lFileTexture->GetFileName();

			// Only TGA textures are supported now.
			if (lFileName.Right(3).Upper() != "TGA")
			{
				FBXSDK_printf("Only TGA textures are supported now: %s\n", lFileName.Buffer());
				continue;
			}

			uint lTextureObject = 0;
			bool lStatus = LoadTextureFromFile(lFileName, lTextureObject);

			const FbxString lAbsFbxFileName = FbxPathUtils::Resolve(pFbxFileName);
			const FbxString lAbsFolderName = FbxPathUtils::GetFolderName(lAbsFbxFileName);
			if (!lStatus)
			{
				// Load texture from relative file name (relative to FBX file)
				const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lFileTexture->GetRelativeFileName());
				lStatus = LoadTextureFromFile(lResolvedFileName, lTextureObject);
			}

			if (!lStatus)
			{
				// Load texture from file name only (relative to FBX file)
				const FbxString lTextureFileName = FbxPathUtils::GetFileName(lFileName);
				const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lTextureFileName);
				lStatus = LoadTextureFromFile(lResolvedFileName, lTextureObject);
			}

			if (!lStatus)
			{
				FBXSDK_printf("Failed to load texture file: %s\n", lFileName.Buffer());
				continue;
			}

			if (lStatus)
			{
				GLuint * lTextureName = new GLuint(lTextureObject);
				lFileTexture->SetUserDataPtr(lTextureName);
			}
		}
#endif
	}

	LoadNode(pScene->GetRootNode(), pAnimLayer);
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: SPMBuild fbx_filename [output_filename]" << std::endl;
		return -1;
	}
	String path = argv[1];
	auto split = SplitString(path, "/\\");
	s_Name = split.back();

	if (argc > 2)
		s_OutputPath = argv[2];
	else
		s_OutputPath = s_Name.substr(0, s_Name.size() - 4) + ".spm";

	s_Attributes = 0;

	FbxManager* manager = FbxManager::Create();
	FbxScene* scene = FbxScene::Create(manager, "Scene");
	FbxImporter* importer = FbxImporter::Create(manager, "");

	int format = -1;
	if (!manager->GetIOPluginRegistry()->DetectReaderFileFormat(path.c_str(), format))
	{
		// Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
		format = manager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");
	}
	if (importer->Initialize(path.c_str(), format))
		std::cout << "Importing " << path << "..." << std::endl;
	else
		std::cout << "Unable to open file " << path << std::endl;

	if (importer->Import(scene))
	{
		FbxAxisSystem sceneAxisSystem = scene->GetGlobalSettings().GetAxisSystem();
		FbxSystemUnit sceneSystemUnit = scene->GetGlobalSettings().GetSystemUnit();

		FbxAxisSystem::MayaYUp.ConvertScene(scene);
		FbxGeometryConverter geometryConverter(manager);
		geometryConverter.Triangulate(scene, true);

		LoadScene(scene, nullptr, path);
	}
	return 0;
}