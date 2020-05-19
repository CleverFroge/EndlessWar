#include <stack>
#include "FrogEngine.h"
#include "algorithm"
using namespace FrogEngine;

FbxManager* Model::pManager = nullptr;

Node* Model::LoadModel(std::string path)
{
	std::string directory = path.substr(0, path.find_last_of('/') + 1);

	if (!pManager)
	{
		pManager = FbxManager::Create();
	}
	FbxScene* pScene = FbxScene::Create(pManager, "My Scene");
	FbxImporter* pImporter = FbxImporter::Create(pManager, "");
	pImporter->Initialize(path.c_str());
	pImporter->Import(pScene);

	Node* ret = ProcessNode(directory, pScene->GetRootNode());
	pScene->Destroy();
	return ret;
}

Node* Model::ProcessNode(std::string directory, FbxNode* pNode)
{
	Node* ret = new Node();
	ret->name = pNode->GetName();
	int attr_count = pNode->GetNodeAttributeCount();
	for (size_t i = 0; i < attr_count; i++)
	{
		FbxNodeAttribute* attr = pNode->GetNodeAttributeByIndex(i);
		if (attr->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh* pMesh = static_cast<FbxMesh*>(attr);
			Mesh* mesh = ProcessMesh(directory, pMesh);
			ret->AddMesh(mesh);
		}
	}
	FbxVector4 t, s, r;
	t = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	s = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	r = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxAMatrix geometry(t, r, s);
	glm::mat4 g;
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			g[i][j] = geometry[i][j];
		}
	}
	ret->geomerty = g;
	
	auto local = pNode->EvaluateLocalTransform(FbxNode::eSourcePivot);
	ret->LocalPosition = local.GetT();
	ret->LocalScale = local.GetS();
	ret->SetLocalEulerAngles(local.GetR());

//	ret->Print();

	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		ret->AddChild(ProcessNode(directory, pNode->GetChild(i)));
	}
	return ret;
}

Vertex Model::GetVertex(FbxMesh* pMesh, int polygonIndex, int polygonVertexIndex)
{
	Vertex vertex;
	FbxVector4* pControlPoints = pMesh->GetControlPoints();	
	int controlPointIndex = pMesh->GetPolygonVertex(polygonIndex, polygonVertexIndex);		

	auto controlPoint = pControlPoints[controlPointIndex].mData;
	vertex.Position.SetX(controlPoint[0]);
	vertex.Position.SetY(controlPoint[1]);
	vertex.Position.SetZ(controlPoint[2]);

	if (pMesh->GetElementNormalCount() > 0)
	{
		FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(0);
		FbxVector4 normal;
		if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			switch (leNormal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				normal = leNormal->GetDirectArray().GetAt(polygonIndex * 3 + polygonVertexIndex);
				break;
			case FbxGeometryElement::eIndexToDirect:
			{
				int id = leNormal->GetIndexArray().GetAt(polygonIndex * 3 + polygonVertexIndex);
				normal = leNormal->GetDirectArray().GetAt(id);
			}
			break;
			default:
				break; // other reference modes not shown here!
			}
		}
		vertex.Normal.SetX(normal.mData[0]);
		vertex.Normal.SetY(normal.mData[1]);
		vertex.Normal.SetZ(normal.mData[2]);
	}

	if (pMesh->GetElementTangentCount() > 0)
	{
		FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(0);
		FbxVector4 Tangent;
		if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			switch (leTangent->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				Tangent = leTangent->GetDirectArray().GetAt(polygonIndex * 3 + polygonVertexIndex);
				break;
			case FbxGeometryElement::eIndexToDirect:
			{
				int id = leTangent->GetIndexArray().GetAt(polygonIndex * 3 + polygonVertexIndex);
				Tangent = leTangent->GetDirectArray().GetAt(id);
			}
			break;
			default:
				break; // other reference modes not shown here!
			}
		}
		vertex.Tangent.SetX(Tangent.mData[0]);
		vertex.Tangent.SetY(Tangent.mData[1]);
		vertex.Tangent.SetZ(Tangent.mData[2]);
	}
	if (pMesh->GetElementUVCount() != 0)
	{
		FbxGeometryElementUV* leUV = pMesh->GetElementUV(0);
		FbxVector2 uv;
		switch (leUV->GetMappingMode())
		{
		default:
			break;
		case FbxGeometryElement::eByControlPoint:
			switch (leUV->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				uv = leUV->GetDirectArray().GetAt(controlPointIndex);
				break;
			case FbxGeometryElement::eIndexToDirect:
			{
				int id = leUV->GetIndexArray().GetAt(controlPointIndex);
				uv = leUV->GetDirectArray().GetAt(id);
			}
			break;
			default:
				break; // other reference modes not shown here!
			}
			break;

		case FbxGeometryElement::eByPolygonVertex:
		{
			int lTextureUVIndex = pMesh->GetTextureUVIndex(polygonIndex, polygonVertexIndex);
			switch (leUV->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			case FbxGeometryElement::eIndexToDirect:
			{
				uv = leUV->GetDirectArray().GetAt(lTextureUVIndex);
			}
			break;
			default:
				break; // other reference modes not shown here!
			}
		}
		break;

		case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
		case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
		case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
			break;
		}
		vertex.TexCoord.SetX(uv.mData[0]);
		vertex.TexCoord.SetY(1-uv.mData[1]);
	}
	return vertex;
}

Mesh* Model::ProcessMesh(std::string directory, FbxMesh* pMesh)
{
	std::vector<Vertex> vertices;
	
	int polygonNum = pMesh->GetPolygonCount();
	for (size_t polygonIndex = 0; polygonIndex < polygonNum; polygonIndex++)
	{
		int polygonVertexNum = pMesh->GetPolygonSize(polygonIndex);
		//n边形有n-2个三角形
		for (size_t triangleIndex = 1; triangleIndex <= polygonVertexNum - 2; triangleIndex++)
		{
			vertices.push_back(GetVertex(pMesh, polygonIndex, 0));
			vertices.push_back(GetVertex(pMesh, polygonIndex, triangleIndex));
			vertices.push_back(GetVertex(pMesh, polygonIndex, triangleIndex + 1));
		}
	}

	Mesh* result = Mesh::Create(vertices);
	FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial();
	if (lMaterialElement)
	{
		auto str = pMesh->GetNode()->GetName();
		FbxSurfaceMaterial* lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(0));
		if (lMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
		{
			Material* material = Material::Create();			
			material->SetDiffuseTexture(LoadMaterialTexture(directory, lMaterial, FbxSurfaceMaterial::sDiffuse));
			material->SetSpecularTexture(LoadMaterialTexture(directory, lMaterial, FbxSurfaceMaterial::sSpecular));
			material->SetNormalTexture(LoadMaterialTexture(directory, lMaterial, FbxSurfaceMaterial::sNormalMap));
			material->shininess = ((FbxSurfacePhong*)lMaterial)->Shininess;
			result->material = material;
		}
	}
	return result;
}

Texture2D* Model::LoadMaterialTexture(std::string directory, FbxSurfaceMaterial* pMaterial, const char* type)
{
	Texture2D* texture = nullptr;
	FbxProperty pProperty;
	pProperty = pMaterial->FindProperty(type);
	int lNbTextures = pProperty.GetSrcObjectCount<FbxTexture>();
	std::string mapName;
	if (lNbTextures > 0)
	{
		for (int j = 0; j < lNbTextures; ++j)
		{
			FbxTexture* pTexture = pProperty.GetSrcObject<FbxTexture>(j);
			if (pTexture)
			{
				FbxFileTexture* pFileTexture = FbxCast<FbxFileTexture>(pTexture);
				mapName = pFileTexture->GetFileName();
				mapName = mapName.substr(mapName.find_last_of('\\') + 1, mapName.length());
				texture = Texture2D::Create((directory + mapName).c_str(), false);
			}
		}
	}
	return texture;
}